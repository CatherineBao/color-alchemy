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

    struct Layer {
        QString name;
        QImage image;
        bool visible = true;
    };

    void addLayer();
    void deleteLayer(int index);
    void setCurrentLayer(int index);
    void setLayerVisibility(int index, bool visibility);
    void setLayerName(int index, const QString& name);

    void addFrame();
    void deleteFrame(int index);
    void setCurrentFrame(int index);

    QImage fullImage() const;

    const QVector<Layer>& getCurrentFrameLayers() const { return frames[currentFrame]; }
    int getFrameCount() const { return frames.size(); }
    int getCurrentFrame() const { return currentFrame; }
    int getCurrentLayer() const { return currentLayer; }

public slots:
    void setPen();
    void setEraser();
    void changePenSize(int size);
    void changeEraserSize(int size);
    void changePenColor(const QColor &color);

signals:
    void currentLayerChanged(int index);

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

    QVector<QVector<Layer>> frames;
    int currentFrame = 0;
    int currentLayer = 0;
    int totalLayersCreated = 0;
};

#endif // CANVAS_H
