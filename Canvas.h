#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QColor>
#include <QImage>
#include <QPoint>

class Canvas : public QWidget
{
    Q_OBJECT

public:
    Canvas(QWidget *parent = nullptr);

    QColor penColor() const { return currentPenColor; }
    int penWidth() const { return currentPenWidth; }
    void resizeGrid(int width, int height);
    int getGridWidth() const { return gridWidth; }
    int getGridHeight() const { return gridHeight; }

public slots:
    void setPen();
    void setEraser();
    void changePenSize(int size);
    void changeEraserSize(int size);
    void changePenColor(const QColor &color);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    void drawPixel(const QPoint &pos);
    void initializeImage(int width, int height);
    void calculatePixelSize();

    bool drawing = false;
    int gridWidth = 36;
    int gridHeight = 36;
    int pixelSize = 10;
    const int MIN_PIXEL_SIZE = 4;

    bool isPen = true;

    int currentToolWidth = 1;
    int currentPenWidth = 1;
    int currentEraserWidth = 1;

    QColor currentToolColor = Qt::black;
    QColor currentPenColor = Qt::black;
    QColor backgroundColor = Qt::white;

    QImage image;
};

#endif // CANVAS_H
