#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QImage>

///
/// \brief The Canvas class - this class contains logic for the main drawing canvas
///
class Canvas : public QWidget
{
    Q_OBJECT

public:
    ///
    /// \brief Canvas - constructor for Canvas
    ///
    Canvas(QWidget *parent = nullptr, int width = 36, int height = 36);

    ///
    /// \brief renderFrame - return a specific layer
    /// \param index - the index of the layer to return
    /// \return the layer at index
    ///
    QImage renderFrame(int index) const;

    ///
    /// \brief renderCurrentFrame - return the total of the layers
    /// \return - the total of the layers
    ///
    QImage renderCurrentFrame() const;

public slots:
    ///
    /// \brief onRedraw - called when a new pixel is drawn
    ///
    void onRedraw(QImage);

signals:
    void pixelChanged(const QPoint &pos);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    ///
    /// \brief PIXEL_SIZE - size of each pixel
    ///
    const int PIXEL_SIZE = 10;

    ///
    /// \brief GRID_WIDTH - width of the grid
    ///
    int gridWidth;

    ///
    /// \brief GRID_HEIGHT - height of the grid
    ///
    int gridHeight;

    ///
    /// \brief initializeImage - initialize the image
    /// \param width - width of the image
    /// \param height - height of the image
    ///
    void initializeImage(int width, int height);

    ///
    /// \brief frame - the current frame
    ///
    QImage frame;

    ///
    /// \brief drawing - can the user draw
    ///
    bool drawing = false;
};

#endif // CANVAS_H
