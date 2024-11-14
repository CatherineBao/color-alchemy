#ifndef ANIMATIONPREVIEW_H
#define ANIMATIONPREVIEW_H

#include <QWidget>
#include <QTimer>
#include <Canvas.h>
#include <QPaintEvent>

class AnimationPreview : public QWidget
{
    Q_OBJECT
public:
    explicit AnimationPreview(QWidget *parent = nullptr, Canvas* canvas = nullptr);

    void setCanvas(Canvas* _canvas);

private:
    QTimer* timer;
    int frameIndex;
    QImage frame;
    Canvas* canvas;
    QSize windowSize;

    void nextFrame();

protected:
    void paintEvent(QPaintEvent *event) override;

public slots:
    void updateFramerate(int fps);
    void resizeWindow(int width, int height);
};

#endif // ANIMATIONPREVIEW_H
