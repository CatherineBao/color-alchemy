#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Model.h"
#include "Canvas.h"

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
    Model& model;

signals:
    void penClicked();
    void changeColor(const QColor &color);

public slots:
    void handleCanvasResize();
    void openColorPicker();
};
#endif // MAINWINDOW_H
