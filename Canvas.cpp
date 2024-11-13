#include "Canvas.h"
#include <QMouseEvent>
#include <QPainter>

Canvas::Canvas(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_StaticContents);
    initializeImage(360, 360);
}

void Canvas::initializeImage(int width, int height) {
    image = QImage(width, height, QImage::Format_ARGB32);
    image.fill(backgroundColor);
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

    QPainter painter(&image);
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
    painter.drawImage(dirtyRect, image, dirtyRect);

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

    QImage oldImage = image;
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

    image = newImage;
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
