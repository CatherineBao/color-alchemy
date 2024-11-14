#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QImage>

class Model : public QObject
{
    Q_OBJECT
public:
    explicit Model(QObject *parent = nullptr);

    QImage pixelData;
    struct Layer {
        QString name;
        QImage image;
        bool visible = true;
    };

    void resizePixelGrid(int width, int height);

    void addLayer();
    void deleteLayer(int index);

    void setLayerName(int index, const QString& name);
    void setLayerVisibility(int index, bool visible);
    bool isLayerVisible(int index) const;
    void setCurrentLayer(int index);

    int getPixelWidth() const { return pixelData.width(); }
    int getPixelHeight() const { return pixelData.height(); }

    QString getLayerName(int index) const;

    void addFrame();
    void deleteFrame(int index);
    void setCurrentFrame(int index);

    int getCurrentLayer() const { return currentLayer; }
    int getLayerCount() const { return frames[currentFrame].size(); }
    int getCurrentFrame() const { return currentFrame; }
    int getFrameCount() const { return frames.size(); }
    const QVector<Layer>& getCurrentFrameLayers() const { return frames[currentFrame]; }

signals:
    void pixelGridChanged();
    void layersChanged();
    void layerVisibilityChanged(int index);
    void layerNameChanged(int index);
    void currentLayerChanged(int index);
    void framesChanged();
    void currentFrameChanged(int index);

private:
    int pixelWidth;
    int pixelHeight;
    QVector<QVector<Layer>> frames;
    int currentFrame = 0;
    int currentLayer = 0;
    int totalLayersCreated = 0;

};
#endif // MODEL_H
