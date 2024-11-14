#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include "Model.h"
#include "Canvas.h"
#include "AnimationPreview.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(Model& model, QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Canvas *canvas;
    AnimationPreview *animationPreview;
    Model& model;

    void setupLayerConnections();
    void setupFrameConnections();

signals:
    void penClicked();
    void changeColor(const QColor &color);

public slots:
    void handleCanvasResize();
    void openColorPicker();

private slots:
    void handleAddLayer();
    void handleDeleteLayer();
    void updateLayerDisplay();
    void handleLayerNameEdit(QListWidgetItem* item);
    void handleLayerVisibilityToggle(int index, bool visible);
    void onLayerVisibilityChanged(int index);
    void onLayerNameChanged(int index);

    void handleAddFrame();
    void handleDeleteFrame();
    void updateFrameDisplay();
    void handleFrameSelection(int index);

};
#endif // MAINWINDOW_H
