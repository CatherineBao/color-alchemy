#include "Model.h"
#include <QPainter>
#include <QFileDialog>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMessageBox>

Model::Model(QObject *parent)
    : QObject{parent}
{
    frames.resize(1);
    //frames[0].resize(1);
    addLayer();
}

void Model::addLayer() {
    Layer newLayer;
    totalLayersCreated++;
    newLayer.name = QString("Layer %1").arg(totalLayersCreated);
    newLayer.visible = true;
    frames[currentFrameIndex].append(newLayer);
    currentLayerIndex = frames[currentFrameIndex].size() - 1;
    emit layersChanged();
}

void Model::deleteLayer(int index) {
    if(frames[currentFrameIndex].size() > 1 && index >= 0 && index < frames[currentFrameIndex].size()) {
        QString layerName = frames[currentFrameIndex][index].name;
        frames[currentFrameIndex].remove(index);
        currentLayerIndex = qMin(currentLayerIndex, frames[currentFrameIndex].size() - 1);
        emit layersChanged();
        emit redrawCanvas(renderCurrentFrame());
    }
}

void Model::setCurrentLayer(int index) {
    if(index >= 0 && index < frames[currentFrameIndex].size() && index != currentLayerIndex) {
        currentLayerIndex = index;
        emit currentLayerChanged(index);
    }
}

QString Model::getLayerName(int index) const {
    if(index >= 0 && index < frames[currentFrameIndex].size()) {
        return frames[currentFrameIndex][index].name;
    }
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
        emit redrawCanvas(renderCurrentFrame());
    }
}

bool Model:: isLayerVisible(int index) const {
    if(index >= 0 && index < frames[currentFrameIndex].size()) {
        return frames[currentFrameIndex][index].visible;
    }
    return false;
}

void Model::addFrame() {
    QVector<Layer> newFrame;
    for(const Layer& layer : frames[currentFrameIndex]) {
        Layer newLayer;
        newLayer.name = layer.name;
        newLayer.visible = layer.visible;
        newLayer.image = QImage(layer.image.width(), layer.image.height(), layer.image.format());
        newLayer.image = layer.image.copy();
        newFrame.append(newLayer);
    }
    frames.insert(currentFrameIndex + 1, newFrame);
    setCurrentFrame(currentFrameIndex + 1);
    emit framesChanged();
    emit currentFrameChanged(currentFrameIndex);
}

void Model::deleteFrame(int index) {
    if(frames.size() > 1 && index >= 0 && index < frames.size()) {
        frames.remove(index);
        currentFrameIndex = qMin(currentFrameIndex, frames.size() - 1);
        emit framesChanged();
    }
}

void Model::setCurrentFrame(int index) {
    if(index >= 0 && index < frames.size() && index != currentFrameIndex) {
        currentFrameIndex = index;
    }
}

void Model::saveJSON(){
    QString fileName = QFileDialog::getSaveFileName(nullptr, "Save Sprites", "", "JSON Files (*.json)");
    if (fileName.isEmpty()) return;

    QJsonArray upperArray;

    for (const auto& frameList : frames) {
        QJsonArray framesArray;
        for(const auto& frame : frameList){
            qDebug() << frame.image;
            framesArray.append(frame.toJson());
        }
        upperArray.append(framesArray);
    }

    QJsonObject rootObj;
    rootObj["frames"] = upperArray;
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

    QJsonArray upperArray = rootObj["frames"].toArray();
    for (const QJsonValue& frameListValue : upperArray) {
        QJsonArray framesArray = frameListValue.toArray();

        QVector<Model::Layer> layerList;
        for (const QJsonValue& frameValue : framesArray) {
            QJsonObject frameObj = frameValue.toObject();
            Model::Layer layer = Model::Layer::fromJson(frameObj);
            layerList.append(layer);
        }

        layersData.append(layerList);
    }

    frames = layersData;
    emit redrawCanvas(renderCurrentFrame());
}

QImage Model::renderFrame(int index) const {
    QImage result(GRID_WIDTH, GRID_HEIGHT, QImage::Format_ARGB32);
    result.fill(Qt::transparent);
    QPainter painter(&result);

    if (index > frames.size() - 1 || index < 0) {
        return result;
    }

    for(const Layer& layer : frames[index]) {
        if(layer.visible) {
            painter.drawImage(0, 0, layer.image);
        }
    }

    return result;
}

QImage Model::renderCurrentFrame() const {
    return renderFrame(currentFrameIndex);
}

void Model::drawPixel(const QPoint &pos)
{
    int x = pos.x() / PIXEL_SIZE;
    int y = pos.y() / PIXEL_SIZE;

    if (x < 0 || x >= GRID_WIDTH || y < 0 || y >= GRID_HEIGHT)
        return;

    QPainter painter(&frames[currentFrameIndex][currentLayerIndex].image);
    painter.setPen(Qt::NoPen);

    int drawWidth = qMin(currentToolWidth, GRID_WIDTH - x);

    if (!isPen) {
        painter.setCompositionMode(QPainter::CompositionMode_Clear);
        painter.setBrush(Qt::transparent);
    } else {
        painter.setBrush(currentToolColor);
    }

    painter.drawRect(x, y, drawWidth, drawWidth);

    emit redrawCanvas(renderCurrentFrame());
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

    if (isPen)
        currentToolWidth = size;
}

void Model::changeEraserSize(int size) {
    currentEraserWidth = size;

    if (!isPen)
        currentToolWidth = size;
}

void Model::changePenColor(const QColor &color){
    currentPenColor = color;
    this->setPen();
}

void Model::updateEverything() {
    emit redrawCanvas(renderCurrentFrame());
    emit framesChanged();
    emit currentFrameChanged(currentFrameIndex);
    void layersChanged();
    void layerVisibilityChanged(int index);
    void layerNameChanged(int index);
    void currentLayerChanged(int index);
    void framesChanged();
}
