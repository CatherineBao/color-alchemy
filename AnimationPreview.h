#ifndef ANIMATIONPREVIEW_H
#define ANIMATIONPREVIEW_H

#include <QWidget>
#include <QTimer>
#include "Model.h"
#include <QPaintEvent>

class AnimationPreview : public QWidget
{
    Q_OBJECT
public:
    explicit AnimationPreview(QWidget *parent = nullptr);

    void setModel(Model* _model);

private:
    QTimer* timer;
    int frameIndex;
    QImage frame;
    Model* model;
    QSize windowSize;

    void nextFrame();

protected:
    void paintEvent(QPaintEvent *event) override;

public slots:
    void updateFramerate(int fps);
    void resizeWindow(int width, int height);
};

#endif // ANIMATIONPREVIEW_H
