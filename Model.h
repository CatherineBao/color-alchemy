#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QImage>

class Model : public QObject
{
    Q_OBJECT
public:
    explicit Model(QObject *parent = nullptr);

    void resizePixelGrid(int width, int height);

    void addLayer();
    void deleteLayer(int index);

    void setLayerName(int index, const QString& name);
    void setLayerVisibility(int index, bool visible);
    bool isLayerVisible(int index) const;

    int getPixelWidth() const { return pixelData.width(); }
    int getPixelHeight() const { return pixelData.height(); }

    int getCurrentLayer() const { return currentLayer; }
    int getLayerCount() const { return layers.size(); }
    QString getLayerName(int index) const;

    void addFrame();
    void deleteFrame(int index);
    void setCurrentFrame(int index);
    int getCurrentFrame() const { return currentFrame; }
    int getFrameCount() const { return frames.size(); }

signals:
    void pixelGridChanged();
    void layersChanged();
    void layerVisibilityChanged(int index);
    void layerNameChanged(int index);
    void framesChanged();
    void currentFrameChanged(int index);

private:
    QImage pixelData;
    struct Layer {
        QString name;
        bool visible = true;
    };

    struct Frame {
        QVector<Layer> layers;
    };

    QVector<Layer> layers;
    int currentLayer = 0;

    QVector<Frame> frames;
    int currentFrame = 0;
};
#endif // MODEL_H
