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
/// \brief The AnimationPreview class
///
class AnimationPreview : public QWidget
{
    Q_OBJECT

public:
    ///
    /// \brief AnimationPreview
    /// \param parent
    ///
    explicit AnimationPreview(QWidget *parent = nullptr);

    ///
    /// \brief setModel
    /// \param _model
    ///
    void setModel(Model* _model);

private:
    ///
    /// \brief timer
    ///
    QTimer* timer;

    ///
    /// \brief frameIndex
    ///
    int frameIndex;

    ///
    /// \brief frame
    ///
    QImage frame;

    ///
    /// \brief model
    ///
    Model* model;

    ///
    /// \brief windowSize
    ///
    QSize windowSize;

    ///
    /// \brief nextFrame
    ///
    void nextFrame();

protected:
    ///
    /// \brief paintEvent
    /// \param event
    ///
    void paintEvent(QPaintEvent *event) override;

public slots:
    ///
    /// \brief updateFramerate
    /// \param fps
    ///
    void updateFramerate(int fps);

    ///
    /// \brief resizeWindow
    /// \param width
    /// \param height
    ///
    void resizeWindow(int width, int height);
};

#endif // ANIMATIONPREVIEW_H
