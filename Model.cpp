#include "Model.h"
#include <QPainter>
#include <QFileDialog>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMessageBox>

Model::Model(QObject *parent)
    : QObject{parent}
    , pixelData(16, 16, QImage::Format_ARGB32)
{
    pixelData.fill(Qt::transparent);
    frames.resize(1);
    addLayer();
}

void Model::resizePixelGrid(int width, int height) {
    if(width <= 0 || height <= 0) return;

    QImage newImage(width, height, QImage::Format_ARGB32);
    newImage.fill(Qt::transparent);

    QPainter painter(&newImage);
    painter.drawImage(0, 0, pixelData);

    pixelData = newImage;
    emit pixelGridChanged();
}

void Model::addLayer() {
    Layer newLayer;
    totalLayersCreated++;
    newLayer.name = QString("Layer %1").arg(totalLayersCreated);
    newLayer.visible = true;
    frames[currentFrame].append(newLayer);
    currentLayer = frames[currentFrame].size() - 1;
    emit layersChanged();
}

void Model::deleteLayer(int index) {
    if(frames[currentFrame].size() > 1 && index >= 0 && index < frames[currentFrame].size()) {
        QString layerName = frames[currentFrame][index].name;
        frames[currentFrame].remove(index);
        currentLayer = qMin(currentLayer, frames[currentFrame].size() - 1);
        emit layersChanged();
    }
}

void Model::setCurrentLayer(int index) {
    if(index >= 0 && index < frames[currentFrame].size() && index != currentLayer) {
        currentLayer = index;
        emit currentLayerChanged(index);
    }
}

QString Model::getLayerName(int index) const {
    if(index >= 0 && index < frames[currentFrame].size()) {
        return frames[currentFrame][index].name;
    }
    return QString();
}

void Model::setLayerName(int index, const QString& name) {
    if(index >= 0 && index < frames[currentFrame].size()) {
        frames[currentFrame][index].name = name;
        emit layerNameChanged(index);
    }
}

void Model::setLayerVisibility(int index, bool visible) {
    if(index >= 0 && index < frames[currentFrame].size()) {
        frames[currentFrame][index].visible = visible;
        emit layerVisibilityChanged(index);
    }
}

bool Model:: isLayerVisible(int index) const {
    if(index >= 0 && index < frames[currentFrame].size()) {
        return frames[currentFrame][index].visible;
    }
    return false;
}

void Model::addFrame() {
    QVector<Layer> newFrame;
    for(const Layer& layer : frames[currentFrame]) {
        Layer newLayer;
        newLayer.name = layer.name;
        newLayer.visible = layer.visible;
        newLayer.image = QImage(layer.image.width(), layer.image.height(), layer.image.format());
        newLayer.image = layer.image.copy();
        newFrame.append(newLayer);
    }
    frames.insert(currentFrame + 1, newFrame);
    setCurrentFrame(currentFrame + 1);
    emit framesChanged();
    emit currentFrameChanged(currentFrame);
}

void Model::deleteFrame(int index) {
    if(frames.size() > 1 && index >= 0 && index < frames.size()) {
        frames.remove(index);
        currentFrame = qMin(currentFrame, frames.size() - 1);
        emit framesChanged();
    }
}

void Model::setCurrentFrame(int index) {
    if(index >= 0 && index < frames.size() && index != currentFrame) {
        currentFrame = index;
    }
}

void Model::save(){
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


QVector<QVector<Model::Layer>> Model::load() {
    QVector<QVector<Model::Layer>> layersData;

    QString fileName = QFileDialog::getOpenFileName(nullptr, "Load Sprites", "", "JSON Files (*.json)");
    if (fileName.isEmpty()) return layersData;

    QFile loadFile(fileName);
    if (!loadFile.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(nullptr, "Error", "Couldn't open load file.");
        return layersData;
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

    return layersData;
}
