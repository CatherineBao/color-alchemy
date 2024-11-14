#include "AnimationPreview.h"
#include "Model.h"
#include <QTimer>
#include <QPainter>
#include <QPaintEvent>

AnimationPreview::AnimationPreview(QWidget *parent)
    : QWidget{parent}
    , frameIndex(0)
    , model(nullptr)
    , windowSize(209, 209)
{
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &AnimationPreview::nextFrame);
    timer->start(1000 / 12);
}

void AnimationPreview::nextFrame() {
    int frameCount = model->getFrameCount();
    if (frameCount == 0) {
        return;
    }

    frameIndex = (frameIndex + 1) % frameCount;
    frame = model->renderFrame(frameIndex);

    update();
}

void AnimationPreview::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QRect target(event->rect());
    QRect frameRect(frame.rect());
    target.setSize(windowSize);

    painter.drawImage(target, frame, frameRect);
}

void AnimationPreview::updateFramerate(int fps) {
    timer->setInterval(1000 / fps);
}

void AnimationPreview::setModel(Model* _model) {
    model = _model;
}

void AnimationPreview::resizeWindow(int width, int height) {
    float aspect = width / (float)height;

    if (width < height) {
        windowSize.setWidth(209 * aspect);
        windowSize.setHeight(209);
    }
    else {
        windowSize.setWidth(209);
        windowSize.setHeight(209 / aspect);
    }
}
