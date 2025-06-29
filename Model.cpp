///
/// Style checked by: Jonas Regehr
///
#include <QFileDialog>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMessageBox>
#include "Model.h"
#include <QPainter>

Model::Model(QObject *parent, int width, int height)
    : QObject{parent}
    , canvasWidth(width)
    , canvasHeight(height)
{
    frames.resize(1);
    addLayer();
}

void Model::addLayer() {
    Layer newLayer(canvasWidth, canvasHeight);
    totalLayersCreated++;
    newLayer.name = QString("Layer %1").arg(totalLayersCreated);
    newLayer.visible = true;
    frames[currentFrameIndex].append(newLayer);
    currentLayerIndex = frames[currentFrameIndex].size() - 1;

    emit layersChanged();
}

void Model::deleteLayer(int index) {
    if(frames[currentFrameIndex].size() > 1 && index >= 0 && index < frames[currentFrameIndex].size()) {
        frames[currentFrameIndex].remove(index);
        currentLayerIndex = qMin(currentLayerIndex, frames[currentFrameIndex].size() - 1);

        emit layersChanged();
        updateCanvas();
    }
}

void Model::setCurrentLayer(int index) {
    if(index >= 0 && index < frames[currentFrameIndex].size() && index != currentLayerIndex) {
        currentLayerIndex = index;

        emit currentLayerChanged(index);
    }
}

QString Model::getLayerName(int index) const {
    if(index >= 0 && index < frames[currentFrameIndex].size())
        return frames[currentFrameIndex][index].name;

    return QString();
}

void Model::setLayerName(int index, const QString& name) {
    if(index >= 0 && index < frames[currentFrameIndex].size()) {
        frames[currentFrameIndex][index].name = name;
        emit layerNameChanged(index);
    }
}

void Model::setLayerVisibility(int index, bool visible) {
    if(index >= 0 && index < frames[currentFrameIndex].size()) {
        frames[currentFrameIndex][index].visible = visible;

        emit layerVisibilityChanged(index);
        updateCanvas();
    }
}

bool Model::isLayerVisible(int index) const {
    if(index >= 0 && index < frames[currentFrameIndex].size())
        return frames[currentFrameIndex][index].visible;

    return false;
}

void Model::addFrame() {
    QVector<Layer> newFrame;
    frames.insert(currentFrameIndex + 1, newFrame);
    setCurrentFrame(currentFrameIndex + 1);
    addLayer();
    updateCanvas();
    emit framesChanged();
    emit currentFrameChanged(currentFrameIndex);
}

void Model::deleteFrame(int index) {
    if(frames.size() > 1 && index >= 0 && index < frames.size()) {
        frames.remove(index);
        currentFrameIndex = qMin(currentFrameIndex, frames.size() - 1);
        emit framesChanged();
        updateCanvas();
    }
}

void Model::setCurrentFrame(int index) {
    if (index >= 0 && index < frames.size() && index != currentFrameIndex) {
        currentFrameIndex = index;
        updateCanvas();
    }
}

void Model::saveJSON() {
    QString fileName = QFileDialog::getSaveFileName(nullptr, "Save Sprites", "", "JSON Files (*.json)");
    if (fileName.isEmpty()) return;

    QJsonArray upperArray;

    for (const auto& frameList : frames) {
        QJsonArray framesArray;
        for(const auto& frame : frameList){
            framesArray.append(frame.toJson());
        }
        upperArray.append(framesArray);
    }

    QJsonObject rootObj;
    rootObj["frames"] = upperArray;
    rootObj["canvas_width"] = canvasWidth;
    rootObj["canvas_height"] = canvasHeight;

    QJsonDocument saveDoc(rootObj);

    QFile saveFile(fileName);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(nullptr, "Error", "Couldn't open save file.");
        return;
    }

    saveFile.write(saveDoc.toJson());
    saveFile.close();
}

void Model::loadJSON() {
    QVector<QVector<Model::Layer>> layersData;

    QString fileName = QFileDialog::getOpenFileName(nullptr, "Load Sprites", "", "JSON Files (*.json)");
    if (fileName.isEmpty()) return;

    QFile loadFile(fileName);
    if (!loadFile.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(nullptr, "Error", "Couldn't open load file.");
        return;
    }

    QByteArray loadData = loadFile.readAll();
    loadFile.close();

    QJsonDocument loadDoc(QJsonDocument::fromJson(loadData));
    QJsonObject rootObj = loadDoc.object();

    canvasWidth = rootObj["canvas_width"].toInt(canvasWidth);
    canvasHeight = rootObj["canvas_height"].toInt(canvasHeight);

    emit setSize(canvasWidth,canvasHeight);
    emit resize();

    QJsonArray upperArray = rootObj["frames"].toArray();
    for (const QJsonValue& frameListValue : upperArray) {
        QJsonArray framesArray = frameListValue.toArray();

        QVector<Model::Layer> layerList;
        for (const QJsonValue& frameValue : framesArray) {
            QJsonObject frameObj = frameValue.toObject();
            Model::Layer layer = Model::Layer::fromJson(frameObj, canvasWidth, canvasHeight);
            layerList.append(layer);
        }
        layersData.append(layerList);
    }
    frames = layersData;
    updateEverything();
}

void Model::renderFrameInternal(QImage& out, int index, qreal opacity) const {
    if (index > frames.size() - 1 || index < 0) {
        return;
    }

    QPainter painter(&out);
    painter.setOpacity(opacity);

    for(const Layer& layer : frames[index]) {
        if(layer.visible) painter.drawImage(0, 0, layer.image);
    }
}

QImage Model::renderFrame(int index) const {
    QImage result(canvasWidth, canvasHeight, QImage::Format_ARGB32);
    result.fill(Qt::transparent);
    renderFrameInternal(result, index, 1.0);
    return result;
}

QImage Model::renderCurrentFrame() const {
    return renderFrame(currentFrameIndex);
}

void Model::drawPixel(const QPoint &pos) {
    int x = pos.x() / PIXEL_SIZE;
    int y = pos.y() / PIXEL_SIZE;

    if (x < 0 || x >= canvasWidth || y < 0 || y >= canvasHeight)
        return;

    QPainter painter(&frames[currentFrameIndex][currentLayerIndex].image);
    painter.setPen(Qt::NoPen);

    int drawWidth = qMin(currentToolWidth, canvasWidth - x);

    if (!isPen) {
        painter.setCompositionMode(QPainter::CompositionMode_Clear);
        painter.setBrush(Qt::transparent);
    } else {
        painter.setBrush(currentToolColor);
    }

    painter.drawRect(x, y, drawWidth, drawWidth);

    updateCanvas();
}

void Model::setPen() {
    isPen = true;
    currentToolColor = currentPenColor;
    currentToolWidth = currentPenWidth;
}

void Model::setEraser() {
    isPen = false;
    currentToolColor = Qt::transparent;
    currentToolWidth = currentEraserWidth;
}

void Model::changePenSize(int size) {
    currentPenWidth = size;

    if (isPen) currentToolWidth = size;
}

void Model::changeEraserSize(int size) {
    currentEraserWidth = size;

    if (!isPen) currentToolWidth = size;
}

void Model::changePenColor(const QColor &color) {
    currentPenColor = color;
    this->setPen();
}

void Model::updateEverything() {
    emit framesChanged();
    emit currentFrameChanged(currentFrameIndex);
    emit layersChanged();
    updateCanvas();
}

void Model::updateCanvas() {
    QImage result(canvasWidth, canvasHeight, QImage::Format_ARGB32);
    result.fill(Qt::transparent);

    if (currentFrameIndex > 1) {
        renderFrameInternal(result, currentFrameIndex - 2, 0.2);
    }

    if (currentFrameIndex > 0) {
        renderFrameInternal(result, currentFrameIndex - 1, 0.4);
    }

    renderFrameInternal(result, currentFrameIndex, 1);

    emit redrawCanvas(result);
}

Model& Model::operator=(const Model& other) {
    if (this == &other) {
        return *this;
    }

    currentToolWidth = other.currentToolWidth;
    currentPenWidth = other.currentPenWidth;
    currentEraserWidth = other.currentEraserWidth;
    currentToolColor = other.currentToolColor;
    currentPenColor = other.currentPenColor;
    currentFrameIndex = other.currentFrameIndex;
    currentLayerIndex = other.currentLayerIndex;
    totalLayersCreated = other.totalLayersCreated;
    isPen = other.isPen;
    canvasWidth = other.canvasWidth;
    canvasHeight = other.canvasHeight;

    frames = other.frames;

    emit framesChanged();
    emit layersChanged();
    emit currentLayerChanged(currentLayerIndex);
    emit currentFrameChanged(currentFrameIndex);

    return *this;
}
