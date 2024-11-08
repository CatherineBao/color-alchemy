#include "Model.h"
#include <QPainter>

Model::Model(QObject *parent)
    : QObject{parent}
    , pixelData(16, 16, QImage::Format_ARGB32)
{
    pixelData.fill(Qt::transparent);
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
