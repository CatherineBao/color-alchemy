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

    QImage renderFrame(int index) const;
    QImage renderCurrentFrame() const;

public slots:
    void onRedraw(QImage);

signals:
    void pixelChanged(const QPoint &pos);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    const int PIXEL_SIZE = 10;

    const int GRID_WIDTH = 36;
    const int GRID_HEIGHT = 36;

    void initializeImage(int width, int height);

    QImage frame;
    bool drawing = false;
};

#endif // CANVAS_H
