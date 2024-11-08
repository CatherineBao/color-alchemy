#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QImage>

class Model : public QObject
{
    Q_OBJECT
public:
    explicit Model(QObject *parent = nullptr);

    void resizePixelGrid(int width, int height);

    int getPixelWidth() const { return pixelData.width(); }
    int getPixelHeight() const { return pixelData.height(); }

signals:
    void pixelGridChanged();

private:
    QImage pixelData;
};
#endif // MODEL_H
