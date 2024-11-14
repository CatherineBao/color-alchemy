#include "Canvas.h"
#include <QMouseEvent>
#include <QPainter>

Canvas::Canvas(QWidget *parent, int width, int height)
    : QWidget(parent)
    , gridWidth(width)
    , gridHeight(height)
    , frame(width, height, QImage::Format_ARGB32)
{
    setAttribute(Qt::WA_StaticContents);
    frame.fill(Qt::transparent);
}

void Canvas::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        drawing = true;
        emit pixelChanged(event->pos());
    }
}

void Canvas::mouseMoveEvent(QMouseEvent *event)
{
    if (drawing)
        emit pixelChanged(event->pos());
}

void Canvas::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && drawing) {
        emit pixelChanged(event->pos());
        drawing = false;
    }
}

void Canvas::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, frame, frame.rect());

    painter.setPen(Qt::gray);

    for (int i = 0; i <= gridWidth; ++i) {
        int x = i * PIXEL_SIZE;
        painter.drawLine(x, 0, x, gridHeight * PIXEL_SIZE);
    }

    for (int j = 0; j <= gridHeight; ++j) {
        int y = j * PIXEL_SIZE;
        painter.drawLine(0, y, gridWidth * PIXEL_SIZE, y);
    }
}

void Canvas::onRedraw(QImage newFrame) {
    frame = newFrame;
    update();
}
