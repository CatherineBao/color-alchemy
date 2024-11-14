///
/// Style checked by: Catherine Bao
///

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
    /// \param index - the index of the layer to delete
    ///
    void deleteLayer(int index);

    ///
    /// \brief getCurrentLayer - gets the current layer selected
    /// \return the index of the current layer
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

    ///
    /// \brief getLayerCount - gets the number of layers in the current frame
    /// \return - the number of layers in the current frame
    ///
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
    /// \brief getCurrentFrame - gets the frame you currently have selected
    /// \return - the index of the frame you have selected
    ///
    int getCurrentFrame() const { return currentFrameIndex; }

    ///
    /// \brief setCurrentFrame - changes the currently selected frame
    /// \param index - the index of the layer to select
    ///
    void setCurrentFrame(int index);

    ///
    /// \brief getFrameCount - gets the number of frames in the project
    /// \return - the number of frames in the proejct
    ///
    int getFrameCount() const { return frames.size(); }

    ///
    /// \brief renderFrame - builds an image for a frame with the given contents
    /// \param index - the index of the image to draw
    /// \return - the finished image
    ///
    QImage renderFrame(int index) const;

    ///
    /// \brief renderCurrentFrame - renders the frame currently selected
    /// \return - the finished image of the frame
    ///
    QImage renderCurrentFrame() const;

    ///
    /// \brief saveJSON - saves the sprite to a json file
    ///
    void saveJSON();

    ///
    /// \brief loadJSON - loads a sprite from a json file
    ///
    void loadJSON();

    ///
    /// \brief operator = - sets a model to be the same as another model
    /// \param other - the other model to copy from
    /// \return - a new model equal to the other model
    ///
    Model& operator=(const Model& other);


signals:
    void redrawCanvas(QImage);
    void layersChanged();
    void layerVisibilityChanged(int index);
    void layerNameChanged(int index);
    void currentLayerChanged(int index);
    void framesChanged();
    void currentFrameChanged(int index);
    void resize();
    void setSize(int width, int height);

private:
    ///
    /// \brief frames - all of the frames in the project
    ///
    QVector<QVector<Layer>> frames;

    ///
    /// \brief PIXEL_SIZE - the size of each pixel
    ///
    const int PIXEL_SIZE = 10;

    ///
    /// \brief GRID_WIDTH - the width of the grid
    ///
    int canvasWidth = 36;

    ///
    /// \brief GRID_HEIGHT - the height of the grid
    int canvasHeight = 36;

    ///
    /// \brief isPen - If the tool is a pen
    ///
    bool isPen = true;

    ///
    /// \brief currentToolWidth - the current size of the tool
    ///
    int currentToolWidth = 1;

    ///
    /// \brief currentPenWidth - current size of the pen
    ///
    int currentPenWidth = 1;

    ///
    /// \brief currentEraserWidth - current size of the eraser
    ///
    int currentEraserWidth = 1;

    ///
    /// \brief currentFrameIndex - the index of the current frame
    ///
    int currentFrameIndex = 0;

    ///
    /// \brief currentLayerIndex - the index of the current layer
    ///
    int currentLayerIndex = 0;

    ///
    /// \brief totalLayersCreated - the number of total layers created
    ///
    int totalLayersCreated = 0;

    ///
    /// \brief updateCanvas - updates the canvas to the latest draw state
    ///
    void updateCanvas();

    ///
    /// \brief renderFrameInternal - builds an image for a frame with the given contents
    /// \param out - the output image
    /// \param index - the index of the frame
    /// \param opacity - the opacity of the image
    ///
    void renderFrameInternal(QImage& out, int index, qreal opacity) const;

    ///
    /// \brief currentToolColor - the color of the current tool
    ///
    QColor currentToolColor = Qt::black;

    ///
    /// \brief currentPenColor - the color of the current pen
    ///
    QColor currentPenColor = Qt::black;

    ///
    /// \brief updateEverything - updates everything in the code
    ///
    void updateEverything();

    ///
    /// \brief setGridSize - sets the size of the grid
    /// \param width - the new width
    /// \param height - the new height
    ///
    void setGridSize(int width, int height);

public slots:
    ///
    /// \brief setPen - sets the tool to the pen
    ///
    void setPen();

    ///
    /// \brief setEraser - sets the tool to the eraser
    ///
    void setEraser();

    ///
    /// \brief changePenSize - changes the size of the pen
    /// \param size - the new size
    ///
    void changePenSize(int size);

    ///
    /// \brief changeEraserSize - changes the size of the eraser
    /// \param size - the new size
    ///
    void changeEraserSize(int size);

    ///
    /// \brief changePenColor - changes the drawing color
    /// \param color - the new color
    ///
    void changePenColor(const QColor &color);

    ///
    /// \brief drawPixel - draws a pixel on the screen
    /// \param pos - the position of the mouse on where to draw
    ///
    void drawPixel(const QPoint &pos);
};
#endif // MODEL_H
