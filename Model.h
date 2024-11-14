#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QImage>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QBuffer>
#include <QByteArray>

///
/// \brief The Model class
///
class Model : public QObject
{
    Q_OBJECT
public:
    explicit Model(QObject *parent = nullptr);

    ///
    /// \brief The Layer class
    ///
    struct Layer {
        ///
        /// \brief name
        ///
        QString name;

        ///
        /// \brief image
        ///
        QImage image;

        ///
        /// \brief visible
        ///
        bool visible;

        ///
        /// \brief Layer
        ///
        Layer() : name(""), image(GRID_WIDTH, GRID_HEIGHT, QImage::Format_ARGB32), visible(true) {
            image.fill(Qt::transparent);
        }

        ///
        /// \brief toJson
        /// \return
        ///
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

        ///
        /// \brief fromJson
        /// \param jsonObj
        /// \return
        ///
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

    ///
    /// \brief addLayer
    ///
    void addLayer();

    ///
    /// \brief deleteLayer
    /// \param index
    ///
    void deleteLayer(int index);

    ///
    /// \brief setLayerName
    /// \param index
    /// \param name
    ///
    void setLayerName(int index, const QString& name);

    ///
    /// \brief setLayerVisibility
    /// \param index
    /// \param visible
    ///
    void setLayerVisibility(int index, bool visible);

    ///
    /// \brief isLayerVisible
    /// \param index
    /// \return
    ///
    bool isLayerVisible(int index) const;

    ///
    /// \brief setCurrentLayer
    /// \param index
    ///
    void setCurrentLayer(int index);

    ///
    /// \brief getLayerName
    /// \param index
    /// \return
    ///
    QString getLayerName(int index) const;

    ///
    /// \brief addFrame
    ///
    void addFrame();

    ///
    /// \brief deleteFrame
    /// \param index
    ///
    void deleteFrame(int index);

    ///
    /// \brief setCurrentFrame
    /// \param index
    ///
    void setCurrentFrame(int index);

    ///
    /// \brief getCurrentLayer
    /// \return
    ///
    int getCurrentLayer() const { return currentLayerIndex; }

    ///
    /// \brief getLayerCount
    /// \return
    ///
    int getLayerCount() const { return frames[currentFrameIndex].size(); }

    ///
    /// \brief getCurrentFrame
    /// \return
    ///
    int getCurrentFrame() const { return currentFrameIndex; }

    ///
    /// \brief getFrameCount
    /// \return
    ///
    int getFrameCount() const { return frames.size(); }

    ///
    /// \brief renderFrame
    /// \param index
    /// \return
    ///
    QImage renderFrame(int index) const;

    ///
    /// \brief renderCurrentFrame
    /// \return
    ///
    QImage renderCurrentFrame() const;

    ///
    /// \brief saveJSON
    ///
    void saveJSON();

    ///
    /// \brief loadJSON
    ///
    void loadJSON();

signals:
    void redrawCanvas(QImage);
    void layersChanged();
    void layerVisibilityChanged(int index);
    void layerNameChanged(int index);
    void currentLayerChanged(int index);
    void framesChanged();
    void currentFrameChanged(int index);

private:
    ///
    /// \brief frames
    ///
    QVector<QVector<Layer>> frames;

    ///
    /// \brief PIXEL_SIZE
    ///
    const int PIXEL_SIZE = 10;

    ///
    /// \brief GRID_WIDTH
    ///
    static const int GRID_WIDTH = 36;

    ///
    /// \brief GRID_HEIGHT
    ///
    static const int GRID_HEIGHT = 36;

    ///
    /// \brief isPen
    ///
    bool isPen = true;

    ///
    /// \brief currentToolWidth
    ///
    int currentToolWidth = 1;

    ///
    /// \brief currentPenWidth
    ///
    int currentPenWidth = 1;

    ///
    /// \brief currentEraserWidth
    ///
    int currentEraserWidth = 1;

    ///
    /// \brief currentFrameIndex
    ///
    int currentFrameIndex = 0;

    ///
    /// \brief currentLayerIndex
    ///
    int currentLayerIndex = 0;

    ///
    /// \brief totalLayersCreated
    ///
    int totalLayersCreated = 0;

    ///
    /// \brief currentToolColor
    ///
    QColor currentToolColor = Qt::black;

    ///
    /// \brief currentPenColor
    ///
    QColor currentPenColor = Qt::black;

    ///
    /// \brief updateEverything
    ///
    void updateEverything();

public slots:
    ///
    /// \brief setPen
    ///
    void setPen();

    ///
    /// \brief setEraser
    ///
    void setEraser();

    ///
    /// \brief changePenSize
    /// \param size
    ///
    void changePenSize(int size);

    ///
    /// \brief changeEraserSize
    /// \param size
    ///
    void changeEraserSize(int size);

    ///
    /// \brief changePenColor
    /// \param color
    ///
    void changePenColor(const QColor &color);

    ///
    /// \brief drawPixel
    /// \param pos
    ///
    void drawPixel(const QPoint &pos);
};
#endif // MODEL_H
