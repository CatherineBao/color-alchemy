#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QImage>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QBuffer>
#include <QByteArray>

class Model : public QObject
{
    Q_OBJECT
public:
    explicit Model(QObject *parent = nullptr);

    struct Layer {
        QString name;
        QImage image;
        bool visible;

        Layer() : name(""), image(GRID_WIDTH, GRID_HEIGHT, QImage::Format_ARGB32), visible(true) {
            image.fill(Qt::transparent);
        }

        QJsonObject toJson() const {

            QJsonObject jsonObj;
            jsonObj["name"] = name;

            QByteArray byteArray;
            QBuffer buffer(&byteArray);
            buffer.open(QIODevice::WriteOnly);
            image.save(&buffer, "PNG");
            jsonObj["image"] = QString::fromLatin1(byteArray.toBase64());
            jsonObj["visible"] = visible;
            return jsonObj;

        }

        static Layer fromJson(const QJsonObject& jsonObj) {
            Layer layer;

            layer.name = jsonObj["name"].toString();
            QString base64Image = jsonObj["image"].toString();
            QByteArray byteArray = QByteArray::fromBase64(base64Image.toLatin1());

            layer.image.loadFromData(byteArray, "PNG");
            layer.visible = jsonObj["visible"].toBool();
            return layer;
        }

    };

    void addLayer();
    void deleteLayer(int index);

    void setLayerName(int index, const QString& name);
    void setLayerVisibility(int index, bool visible);
    bool isLayerVisible(int index) const;
    void setCurrentLayer(int index);

    QString getLayerName(int index) const;

    void addFrame();
    void deleteFrame(int index);
    void setCurrentFrame(int index);

    int getCurrentLayer() const { return currentLayerIndex; }
    int getLayerCount() const { return frames[currentFrameIndex].size(); }
    int getCurrentFrame() const { return currentFrameIndex; }
    int getFrameCount() const { return frames.size(); }

    QImage renderFrame(int index) const;
    QImage renderCurrentFrame() const;

    const QVector<Layer>& getCurrentFrameLayers() const { return frames[currentFrameIndex]; }

    void saveJSON();
    void loadJSON();

    QVector<QVector<Layer>> frames;

signals:
    void redrawCanvas(QImage);
    void layersChanged();
    void layerVisibilityChanged(int index);
    void layerNameChanged(int index);
    void currentLayerChanged(int index);
    void framesChanged();
    void currentFrameChanged(int index);

private:
    const int PIXEL_SIZE = 10;

    static const int GRID_WIDTH = 36;
    static const int GRID_HEIGHT = 36;

    bool isPen = true;

    int currentToolWidth = 1;
    int currentPenWidth = 1;
    int currentEraserWidth = 1;

    QColor currentToolColor = Qt::black;
    QColor currentPenColor = Qt::black;

    int currentFrameIndex = 0;
    int currentLayerIndex = 0;
    int totalLayersCreated = 0;

    void updateEverything();

public slots:
    void setPen();
    void setEraser();
    void changePenSize(int size);
    void changeEraserSize(int size);
    void changePenColor(const QColor &color);
    void drawPixel(const QPoint &pos);
};
#endif // MODEL_H
