#ifndef ANIMATIONPREVIEW_H
#define ANIMATIONPREVIEW_H

#include <QWidget>
#include <QTimer>
#include <Canvas.h>

class AnimationPreview : public QWidget
{
    Q_OBJECT
public:
    explicit AnimationPreview(QWidget *parent = nullptr, Canvas* canvas = nullptr);

    void setCanvas(Canvas* _canvas);

private:
    QTimer* timer;
    QPainter* painter;
    int frameIndex;
    QImage frame;
    Canvas* canvas;

    void nextFrame();

protected:
    void paintEvent(QPaintEvent *event) override;

public slots:
    void updateFramerate(int fps);
};

#endif // ANIMATIONPREVIEW_H
