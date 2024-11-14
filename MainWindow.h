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

///
/// \brief The MainWindow class - contains logic for the main window
///
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    ///
    /// \brief MainWindow constructor for MainWindow
    ///
    MainWindow(Model& model, QWidget *parent = nullptr);

    ~MainWindow();

private:
    ///
    /// \brief ui - the ui file
    ///
    Ui::MainWindow *ui;

    ///
    /// \brief canvas - the drawing canvas
    ///
    Canvas *canvas;

    ///
    /// \brief animationPreview - the animation preview window
    ///
    AnimationPreview *animationPreview;

    ///
    /// \brief model - the model
    ///
    Model& model;

    ///
    /// \brief setupLayerConnections - sets up layer connections
    ///
    void setupLayerConnections();

    ///
    /// \brief setupFrameConnections - sets up frame connections
    ///
    void setupFrameConnections();

signals:
    void penClicked();
    void colorChanged(const QColor &color);
    void frameRateChanged(int fps);

public slots:
    ///
    /// \brief openColorPicker - uses QColorDialog to allow the user to select a color
    ///
    void openColorPicker();

private slots:
    ///
    /// \brief syncLayerSelection - syncs layer selections
    ///
    void syncLayerSelection();

    ///
    /// \brief handleAddLayer -
    ///
    void handleAddLayer();

    ///
    /// \brief handleDeleteLayer
    ///
    void handleDeleteLayer();

    ///
    /// \brief updateLayerDisplay
    ///
    void updateLayerDisplay();

    ///
    /// \brief handleLayerNameEdit
    /// \param item
    ///
    void handleLayerNameEdit(QListWidgetItem* item);

    ///
    /// \brief handleLayerVisibilityToggle
    /// \param index
    /// \param visible
    ///
    void handleLayerVisibilityToggle(int index, bool visible);

    ///
    /// \brief onLayerVisibilityChanged
    /// \param index
    ///
    void onLayerVisibilityChanged(int index);

    ///
    /// \brief onLayerNameChanged
    /// \param index
    ///
    void onLayerNameChanged(int index);

    ///
    /// \brief handleAddFrame
    ///
    void handleAddFrame();

    ///
    /// \brief handleDeleteFrame
    ///
    void handleDeleteFrame();

    ///
    /// \brief updateFrameDisplay
    ///
    void updateFrameDisplay();

    ///
    /// \brief handleFrameSelection
    /// \param index
    ///
    void handleFrameSelection(int index);

};
#endif // MAINWINDOW_H
