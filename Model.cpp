#include "Model.h"
#include <QPainter>

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
    newLayer.name = QString("Layer %1").arg(layers.size() + 1);
    newLayer.visible = true;
    layers.append(newLayer);
    currentLayer = layers.size() - 1;
    emit layersChanged();
}

void Model::deleteLayer(int index) {
    if(layers.size() > 1 && index >= 0 && index < layers.size()) {
        layers.remove(index);
        currentLayer = qMin(currentLayer, layers.size() - 1);
        emit layersChanged();
    }
}

QString Model::getLayerName(int index) const {
    if(index >= 0 && index < layers.size()) {
        return layers[index].name;
    }
    return QString();
}

void Model::setLayerName(int index, const QString& name) {
    if(index >= 0 && index < layers.size()) {
        layers[index].name = name;
        emit layerNameChanged(index);
    }
}

void Model::setLayerVisibility(int index, bool visible) {
    if(index >= 0 && index < layers.size()) {
        layers[index].visible = visible;
        emit layerVisibilityChanged(index);
    }
}

bool Model:: isLayerVisible(int index) const {
    if(index >= 0 && index < layers.size()) {
        return layers[index].visible;
    }
    return false;
}

void Model::addFrame() {
    Frame newFrame;
    newFrame.layers = frames[currentFrame].layers;
    frames.insert(currentFrame + 1, newFrame);
    setCurrentFrame(currentFrame + 1);
    emit framesChanged();
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
        emit currentFrameChanged(index);
    }
}
