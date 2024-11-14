#include "AnimationPreview.h"
#include "Canvas.h"
#include <QTimer>
#include <QPainter>
#include <QPaintEvent>
#include <QDebug>

AnimationPreview::AnimationPreview(QWidget *parent, Canvas* canvas)
    : QWidget{parent}
    , frameIndex(0)
    , canvas(canvas)
{
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &AnimationPreview::nextFrame);
    timer->start(1000 / 12);

    painter = new QPainter(this);
}

void AnimationPreview::nextFrame() {
    int frameCount = canvas->getFrameCount();
    if (frameCount == 0) {
        return;
    }

    frameIndex = (frameIndex + 1) % frameCount;
    frame = canvas->renderFrame(frameIndex);

    update();
}

void AnimationPreview::paintEvent(QPaintEvent *event) {
    auto rectThing = event->rect();

    painter->drawImage(rectThing, frame, rectThing);
    qInfo("yup");
}

void AnimationPreview::updateFramerate(int fps) {
    timer->setInterval(1000 / fps);
}

void AnimationPreview::setCanvas(Canvas* _canvas) {
    canvas = _canvas;
}
