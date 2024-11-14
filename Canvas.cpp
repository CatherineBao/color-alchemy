#include "Canvas.h"
#include <QMouseEvent>
#include <QPainter>

Canvas::Canvas(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_StaticContents);

    frames.resize(1);
    frames[0].resize(1);
    frames[0][0].name = "Layer 1";
    initializeImage(360, 360);
}

void Canvas::initializeImage(int width, int height) {
    frames[currentFrame][currentLayer].image = QImage(width, height, QImage::Format_ARGB32);
    frames[currentFrame][currentLayer].image.fill(Qt::transparent);

    gridWidth = width / pixelSize;
    gridHeight = height / pixelSize;

    calculatePixelSize();
    setFixedSize(width, height);

    update();
}

void Canvas::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        drawing = true;
        drawPixel(event->pos());
    }
}

void Canvas::mouseMoveEvent(QMouseEvent *event)
{
    if (drawing)
        drawPixel(event->pos());
}

void Canvas::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && drawing) {
        drawPixel(event->pos());
        drawing = false;
    }
}

void Canvas::drawPixel(const QPoint &pos)
{
    int x = pos.x() / pixelSize;
    int y = pos.y() / pixelSize;

    if (x < 0 || x >= gridWidth || y < 0 || y >= gridHeight)
        return;

    QPainter painter(&frames[currentFrame][currentLayer].image);
    painter.setPen(Qt::NoPen);
    painter.setBrush(currentToolColor);

    int scaledX = x * pixelSize;
    int scaledY = y * pixelSize;
    int drawWidth = qMin(pixelSize * currentToolWidth, pixelSize * gridWidth - x);

    painter.drawRect(scaledX, scaledY, drawWidth, drawWidth);
    update(QRect(scaledX, scaledY, drawWidth, drawWidth));
}

void Canvas::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, fullImage(), dirtyRect);

    // draw grids
    painter.setPen(Qt::gray);
    for (int i = 0; i <= gridWidth; ++i)
        painter.drawLine(i * pixelSize, 0, i * pixelSize, gridHeight * pixelSize);
    for (int i = 0; i <= gridHeight; ++i)
        painter.drawLine(0, i * pixelSize, gridWidth * pixelSize, i * pixelSize);
}

void Canvas::setPen() {
    isPen = true;
    currentToolColor = currentPenColor;
    currentToolWidth = currentPenWidth;
}

void Canvas::setEraser() {
    isPen = false;
    currentToolColor = backgroundColor;
    currentToolWidth = currentEraserWidth;
}

void Canvas::changePenSize(int size) {
    currentPenWidth = size;

    if (isPen)
        currentToolWidth = size;
}

void Canvas::changeEraserSize(int size) {
    currentEraserWidth = size;

    if (!isPen)
        currentToolWidth = size;
}

void Canvas::resizeGrid(int width, int height) {
    if (width <= 0 || height <= 0) return;

    QImage oldImage = fullImage();
    int oldWidth = gridWidth;
    int oldHeight = gridHeight;
    int oldPixelSize = pixelSize;


    gridWidth = width;
    gridHeight = height;
    calculatePixelSize();

    QImage newImage(gridWidth * pixelSize, gridHeight * pixelSize, QImage::Format_ARGB32);
    newImage.fill(backgroundColor);

    if(!oldImage.isNull()) {
        QPainter painter(&newImage);
        painter.setPen(Qt::NoPen);

        for(int x = 0; x < oldWidth; x++) {
            for(int y = 0; y < oldHeight; y++) {
                if(x < gridWidth && y < gridHeight) {
                    QColor pixelColor = oldImage.pixelColor(x * oldPixelSize, y * oldPixelSize);

                    if(pixelColor != backgroundColor) {
                        painter.setBrush(pixelColor);
                        painter.drawRect(x * pixelSize, y * pixelSize, pixelSize, pixelSize);
                    }
                }
            }
        }
    }

    frames[currentFrame][currentLayer].image = newImage;
    setFixedSize(gridWidth * pixelSize, gridHeight * pixelSize);
    update();
}

void Canvas::calculatePixelSize() {
    int widgetWidth = 360;
    int widgetHeight = 360;

    double horizontalSize = static_cast<double>(widgetWidth) / gridWidth;
    double verticalSize = static_cast<double>(widgetHeight) / gridHeight;

    pixelSize = static_cast<int>(floor(qMin(horizontalSize, verticalSize)));
    pixelSize = qMax(pixelSize, MIN_PIXEL_SIZE);
}

void Canvas::changePenColor(const QColor &color){
    currentPenColor = color;
    this->setPen();
}

void Canvas::addLayer() {
    Layer newLayer;
    totalLayersCreated++;
    newLayer.name = QString("Layer %1").arg(totalLayersCreated);
    newLayer.image = QImage(gridWidth * pixelSize, gridHeight * pixelSize, QImage::Format_ARGB32);
    newLayer.image.fill(Qt::transparent);
    newLayer.visible = true;

    frames[currentFrame].append(newLayer);
    currentLayer = frames[currentFrame].size() - 1;
    update();
}

void Canvas::deleteLayer(int index) {
    if(frames[currentFrame].size() > 1 && index >= 0 && index < frames[currentFrame].size()) {
        QString layerName = frames[currentFrame][index].name;
        frames[currentFrame].remove(index);
        currentLayer = qMin(currentLayer, frames[currentFrame].size() - 1);
        update();
    }
}

void Canvas::setCurrentLayer(int index) {
    if(index >= 0 && index < frames[currentFrame].size() && index != currentLayer) {
        currentLayer = index;
        emit currentLayerChanged(index);
    }
}

void Canvas::setLayerVisibility(int index, bool visible) {
    if(index >= 0 && index < frames[currentFrame].size()) {
        frames[currentFrame][index].visible = visible;
        update();
    }
}

void Canvas::setLayerName(int index, const QString& name) {
    if(index >= 0 && index < frames[currentFrame].size()) {
        frames[currentFrame][index].name = name;
    }
}

void Canvas::addFrame() {
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
    currentFrame++;
    update();
}

void Canvas::deleteFrame(int index) {
    if(frames.size() > 1 && index >= 0 && index < frames.size()) {
        frames.remove(index);
        currentFrame = qMin(currentFrame, frames.size() - 1);
        update();
    }
}

void Canvas::setCurrentFrame(int index) {
    if(index >= 0 && index < frames.size() && index != currentFrame) {
        currentFrame = index;
        currentLayer = qMin(currentLayer, frames[currentFrame].size() - 1);
        emit currentLayerChanged(currentLayer);
        update();
    }
}

QImage Canvas::fullImage() const {
    QImage result(gridWidth * pixelSize, gridHeight * pixelSize, QImage::Format_ARGB32);
    result.fill(Qt::transparent);
    QPainter painter(&result);

    for(const Layer& layer : frames[currentFrame]) {
        if(layer.visible) {
            painter.drawImage(0, 0, layer.image);
        }
    }

    return result;
}
