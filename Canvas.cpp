///
/// Style checked by:
///
#include <QMouseEvent>
#include <QPainter>
#include "Canvas.h"

Canvas::Canvas(QWidget *parent)
    : QWidget(parent)
    , frame(GRID_WIDTH, GRID_HEIGHT, QImage::Format_ARGB32)
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

    // draw grids
    painter.setPen(Qt::gray);
    for (int i = 0; i <= GRID_WIDTH; ++i)
        painter.drawLine(i * PIXEL_SIZE, 0, i * PIXEL_SIZE, GRID_HEIGHT * PIXEL_SIZE);
    for (int i = 0; i <= GRID_HEIGHT; ++i)
        painter.drawLine(0, i * PIXEL_SIZE, GRID_WIDTH * PIXEL_SIZE, i * PIXEL_SIZE);
}

void Canvas::onRedraw(QImage newFrame) {
    frame = newFrame;
    update();
}
