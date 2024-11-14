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
/// \brief The Model class - the backend for the Sprite Editor application
///
class Model : public QObject
{
    Q_OBJECT
public:
    explicit Model(QObject *parent = nullptr, int width = 36, int height = 36);

    ///
    /// \brief The Layer struct - stores a layer
    ///
    struct Layer {
        ///
        /// \brief name - the name of the layer
        ///
        QString name;

        ///
        /// \brief image - the image contained within the layer
        ///
        QImage image;

        ///
        /// \brief visible - whether the layer is visible
        ///
        bool visible;

        ///
        /// \brief Layer - constructor for Layer
        ///
        Layer(int width, int height) : name(""), image(width, height, QImage::Format_ARGB32), visible(true) {
            image.fill(Qt::transparent);
        }

        ///
        /// \brief toJson - serializes a layer to Json
        /// \return the serialized layer
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
        /// \brief fromJson - deserializes a layer from Json
        /// \param jsonObj - the Json object to deserialize
        /// \return the deserialized layer
        ///
        static Layer fromJson(const QJsonObject& jsonObj, int width, int height) {
            Layer layer(width, height);

            layer.name = jsonObj["name"].toString();
            QString base64Image = jsonObj["image"].toString();
            QByteArray byteArray = QByteArray::fromBase64(base64Image.toLatin1());

            layer.image.loadFromData(byteArray, "PNG");
            layer.visible = jsonObj["visible"].toBool();
            return layer;
        }
    };

    ///
    /// \brief addLayer - creates a new layer and updates internal structure
    ///
    void addLayer();

    ///
    /// \brief deleteLayer - deletes an old layer and updates internal structure
    /// \param index - the index of the layer to dleet
    ///
    void deleteLayer(int index);

    ///
    /// \brief getCurrentLayer
    /// \return
    ///
    int getCurrentLayer() const { return currentLayerIndex; }

    ///
    /// \brief setCurrentLayer - changes the currently selected layer
    /// \param index - the index of the layer to select
    ///
    void setCurrentLayer(int index);

    ///
    /// \brief getLayerName - fetches the name of a given layer
    /// \param index - the index of the layer
    /// \return the name of the layer at index
    ///
    QString getLayerName(int index) const;

    int getLayerCount() const { return frames[currentFrameIndex].size(); }

    ///
    /// \brief setLayerName - changes the name of a layer
    /// \param index - the index of the layer to change
    /// \param name - the new name of the layer
    ///
    void setLayerName(int index, const QString& name);

    ///
    /// \brief setLayerVisibility - toggles the visiblity of a layer
    /// \param index - the index of the layer to toggle
    /// \param visible - true if visible, false otherwise
    ///
    void setLayerVisibility(int index, bool visible);

    ///
    /// \brief isLayerVisible - checks whether a layer is visible
    /// \param index - the index of the layer to check
    /// \return true if the layer at index is visible, false otherwise
    ///
    bool isLayerVisible(int index) const;



    ///
    /// \brief addFrame - creates a new frame and updates internal structure
    ///
    void addFrame();

    ///
    /// \brief deleteFrame - deletes a frame and updates internal structure
    /// \param index - the index of the frame to delete
    ///
    void deleteFrame(int index);

    ///
    /// \brief getCurrentFrame
    /// \return
    ///
    int getCurrentFrame() const { return currentFrameIndex; }

    ///
    /// \brief setCurrentFrame
    /// \param index
    ///
    void setCurrentFrame(int index);

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

    Model& operator=(const Model& other);


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
    int canvasWidth = 36;

    ///
    /// \brief GRID_HEIGHT
    ///
    int canvasHeight = 36;

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

    void setGridSize(int width, int height);

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
