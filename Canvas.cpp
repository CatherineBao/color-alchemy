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
    int x = pos.x() / gridCellSize;
    int y = pos.y() / gridCellSize;

    if (x < 0 || x >= gridWidth || y < 0 || y >= gridHeight)
        return;

    QPainter painter(&image);
    painter.setPen(Qt::NoPen);
    painter.setBrush(myPenColor);
    painter.drawRect(x * gridCellSize, y * gridCellSize, gridCellSize, gridCellSize);
    update(QRect(x * gridCellSize, y * gridCellSize, gridCellSize, gridCellSize));
}

void Canvas::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, image, dirtyRect);

    painter.setPen(Qt::gray);
    for (int i = 0; i <= gridWidth; ++i)
        painter.drawLine(i * gridCellSize, 0, i * gridCellSize, gridHeight * gridCellSize);
    for (int i = 0; i <= gridHeight; ++i)
        painter.drawLine(0, i * gridCellSize, gridWidth * gridCellSize, i * gridCellSize);
}

void Canvas::setPen() {
    myPenColor = currentPenColor;
}

void Canvas::setEraser() {
    myPenColor = backgroundColor;
}

void Canvas::setGridCellSize(int size) {
    gridCellSize = size;
    update();
}
