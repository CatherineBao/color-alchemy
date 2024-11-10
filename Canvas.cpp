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
    painter.drawRect(x * pixelSize, y * pixelSize, pixelSize, pixelSize);
    update(QRect(x * pixelSize, y * pixelSize, pixelSize, pixelSize));
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
    currentToolColor = currentPenColor;
}

void Canvas::setEraser() {
    currentToolColor = backgroundColor;
}
