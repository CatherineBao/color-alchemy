///
/// Style checked by: Mitch Briles
///
#ifndef ANIMATIONPREVIEW_H
#define ANIMATIONPREVIEW_H

#include <QWidget>
#include <QTimer>
#include <QPaintEvent>
#include "Model.h"

///
/// \brief The AnimationPreview class - handles the animation preview window
///
class AnimationPreview : public QWidget
{
    Q_OBJECT

public:
    ///
    /// \brief AnimationPreview - constructor for animation preview
    ///
    explicit AnimationPreview(QWidget *parent = nullptr);

    ///
    /// \brief setModel - sets the internal model variable
    ///
    void setModel(Model* _model);

private:
    ///
    /// \brief timer - timer used to display animation
    ///
    QTimer* timer;

    ///
    /// \brief frameIndex - index of current frame
    ///
    int frameIndex;

    ///
    /// \brief frame - current frame
    ///
    QImage frame;

    ///
    /// \brief model - reference to the model
    ///
    Model* model;

    ///
    /// \brief windowSize - the size of the window
    ///
    QSize windowSize;

    ///
    /// \brief nextFrame - moves to the next frame
    ///
    void nextFrame();

protected:
    void paintEvent(QPaintEvent *event) override;

public slots:
    ///
    /// \brief updateFramerate - sets the framerate to fps
    /// \param fps
    ///
    void updateFramerate(int fps);

    ///
    /// \brief resizeWindow - resizes the window to width, height
    /// \param width
    /// \param height
    ///
    void resizeWindow(int width, int height);
};

#endif // ANIMATIONPREVIEW_H
