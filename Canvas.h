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

    void setGridCellSize(int size);
    QColor penColor() const { return myPenColor; }
    int penWidth() const { return myPenWidth; }

public slots:
    void setPen();
    void setEraser();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    void drawPixel(const QPoint &pos);
    void initializeImage(int width, int height);

    bool drawing = false;
    int gridWidth = 36;
    int gridHeight = 36;
    int gridCellSize = 10;
    int myPenWidth = 1;
    QColor currentPenColor = Qt::black;
    QColor myPenColor = Qt::black;
    QColor backgroundColor = Qt::white;

    QPoint lastPoint;
    QImage image;
};

#endif // CANVAS_H
