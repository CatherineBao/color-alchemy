///
/// Style checked by:
///
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
    /// \brief syncLayerSelection - syncs layer selections bewteen ui and class
    ///
    void syncLayerSelection();

    ///
    /// \brief handleAddLayer - adds a layer to the model and updates the display
    ///
    void handleAddLayer();

    ///
    /// \brief handleDeleteLayer - deletes a layer from the model and updates the display
    ///
    void handleDeleteLayer();

    ///
    /// \brief updateLayerDisplay - updates the display of the layers
    ///
    void updateLayerDisplay();

    ///
    /// \brief handleLayerNameEdit - changes the name of a layer
    ///
    void handleLayerNameEdit(QListWidgetItem* item);

    ///
    /// \brief handleLayerVisibilityToggle - toggles the visibility of a layer
    /// \param index - the index of the layer to toggle
    /// \param visible - true if visible, false otherwise
    ///
    void handleLayerVisibilityToggle(int index, bool visible);

    ///
    /// \brief onLayerVisibilityChanged - toggles the checkbox for the visiblity of a layer
    /// \param index - the index of the layer to toggle
    ///
    void onLayerVisibilityChanged(int index);

    ///
    /// \brief onLayerNameChanged - changes the name of the layer in the ui
    /// \param index - the index of the layer to change
    ///
    void onLayerNameChanged(int index);

    ///
    /// \brief handleAddFrame - adds a new frame to the model and updates the display
    ///
    void handleAddFrame();

    ///
    /// \brief handleDeleteFrame - deletes a frame from the model and updates the display
    ///
    void handleDeleteFrame();

    ///
    /// \brief updateFrameDisplay - refreshes the frame display
    ///
    void updateFrameDisplay();

    ///
    /// \brief handleFrameSelection - toggles which frame is selected
    /// \param index - the index of the frame to select
    ///
    void handleFrameSelection(int index);

    ///
    /// \brief resizeCanvas
    ///
    void resizeCanvas();

    ///
    /// \brief setSize
    /// \param width
    /// \param height
    ///
    void setSize(int width, int height);

};
#endif // MAINWINDOW_H
