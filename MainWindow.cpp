#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "Model.h"
#include "Canvas.h"
#include "AnimationPreview.h"
#include <QColorDialog>

MainWindow::MainWindow(Model& model, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , model(model)
{
    ui->setupUi(this);
    canvas = qobject_cast<Canvas*>(ui->editor);
    animationPreview = qobject_cast<AnimationPreview*>(ui->animationPreview);
    animationPreview->setCanvas(canvas);

    ui->penSize->setValue(1);
    ui->eraserSize->setValue(1);
    ui->canvasWidthBox->setValue(36);
    ui->canvasHeightBox->setValue(36);

    ui->canvasWidthBox->setRange(1, 100);
    ui->canvasHeightBox->setRange(1, 100);

    setupLayerConnections();
    setupFrameConnections();
    updateLayerDisplay();
    updateFrameDisplay();

    connect(ui->penToolButton, &QPushButton::clicked, canvas, &Canvas::setPen);
    connect(ui->penToolButton, &QPushButton::clicked, canvas, [this](){ui->eraserToolButton->setChecked(false);});
    connect(ui->eraserToolButton, &QPushButton::clicked, canvas, &Canvas::setEraser);
    connect(ui->eraserToolButton, &QPushButton::clicked, canvas, [this](){ui->penToolButton->setChecked(false);});

    connect(ui->penSize, &QSpinBox::valueChanged, canvas, &Canvas::changePenSize);
    connect(ui->eraserSize, &QSpinBox::valueChanged, canvas, &Canvas::changeEraserSize);

    // NOTE: I'm not sure what this did... but I think I mssed it up
    connect(ui->canvasWidthBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &MainWindow::handleCanvasResize);
    connect(ui->canvasHeightBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &MainWindow::handleCanvasResize);

    connect(ui->colorButton, &QPushButton::clicked, this, &MainWindow::openColorPicker);
    connect(this, &MainWindow::changeColor, canvas, &Canvas::changePenColor);

    connect(ui->canvasWidthBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::handleCanvasResize);
    connect(ui->canvasHeightBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::handleCanvasResize);

    connect(ui->layerList, &QListWidget::itemChanged,
        [this](QListWidgetItem* item) {
        int row = ui->layerList->row(item);
        bool visible = item->checkState() == Qt::Checked;
        handleLayerVisibilityToggle(row, visible);
        });

    connect(canvas, &Canvas::gridResized, animationPreview, &AnimationPreview::resizeWindow);
    connect(this, &MainWindow::frameRateChanged, animationPreview, &AnimationPreview::updateFramerate);
    connect(ui->fpsBox, QOverload<int>::of(&QSpinBox::valueChanged), animationPreview, [this](){emit frameRateChanged(ui->fpsBox->value());});
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleCanvasResize() {
    ui->canvasWidthBox->blockSignals(true);
    ui->canvasHeightBox->blockSignals(true);

    int width = ui->canvasWidthBox->value();
    int height = ui->canvasHeightBox->value();

    model.resizePixelGrid(width, height);
    canvas->resizeGrid(width, height);

    ui->canvasWidthBox->blockSignals(false);
    ui->canvasHeightBox->blockSignals(false);
}

void MainWindow::openColorPicker(){

    QColor color = QColorDialog::getColor(Qt::white, this, "Select Color");

    if (color.isValid()) {
        emit changeColor(color);
        QString style = QString("background-color: %1;").arg(color.name());
        ui->currentColorLabel->setStyleSheet(style);
    }
}

void MainWindow::setupLayerConnections() {
    connect(ui->addLayerButton, &QPushButton::clicked, this, &MainWindow::handleAddLayer);
    connect(ui->deleteLayerButton, &QPushButton::clicked, this, &MainWindow::handleDeleteLayer);
    connect(&model, &Model::layersChanged, this, &MainWindow::updateLayerDisplay);

    connect(&model, &Model::currentLayerChanged, this, &MainWindow::syncLayerSelection);
    connect(canvas, &Canvas::currentLayerChanged, this, &MainWindow::syncLayerSelection);

    connect(ui->layerList, &QListWidget::itemChanged, this, &MainWindow::handleLayerNameEdit);
    connect(&model, &Model::layerVisibilityChanged, this, &MainWindow::onLayerVisibilityChanged);
    connect(&model, &Model::layerNameChanged, this, &MainWindow::onLayerNameChanged);

    connect(ui->layerList, &QListWidget::currentRowChanged, [this](int row) {
        if(row >= 0) {
            model.setCurrentLayer(row);
            canvas->setCurrentLayer(row);
        }
    });
}

void MainWindow::syncLayerSelection() {
    int currentLayer = canvas->getCurrentLayer();
    if(ui->layerList->currentRow() != currentLayer) {
        ui->layerList->setCurrentRow(currentLayer);
    }
}

void MainWindow::handleAddLayer() {
    model.addLayer();
    canvas->addLayer();
    updateLayerDisplay();
}

void MainWindow::handleDeleteLayer() {
    int currentLayer = canvas->getCurrentLayer();
    model.deleteLayer(currentLayer);
    canvas->deleteLayer(currentLayer);
    updateLayerDisplay();
}

void MainWindow::updateLayerDisplay() {
    ui->layerList->clear();
    for(int i = 0; i < model.getLayerCount(); ++i) {
        QListWidgetItem* item = new QListWidgetItem();
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsEditable);
        item->setCheckState(model.isLayerVisible(i) ? Qt::Checked : Qt::Unchecked);
        item->setText(model.getLayerName(i));
        ui->layerList->addItem(item);
    }
    ui->layerList->setCurrentRow(model.getCurrentLayer());
}

void MainWindow::handleLayerNameEdit(QListWidgetItem* item) {
    int row = ui->layerList->row(item);
    model.setLayerName(row, item->text());
    canvas->setLayerName(row, item->text());
}

void MainWindow::handleLayerVisibilityToggle(int index, bool visible) {
    model.setLayerVisibility(index, visible);
    canvas->setLayerVisibility(index, visible);
    canvas->update();
}

void MainWindow::onLayerVisibilityChanged(int index) {
    QListWidgetItem* item = ui->layerList->item(index);
    if(item) {
        bool visible = model.isLayerVisible(index);
        item->setCheckState(visible ? Qt::Checked : Qt::Unchecked);
    }
    canvas->update();
}

void MainWindow::onLayerNameChanged(int index) {
    QListWidgetItem* item = ui->layerList->item(index);
    if(item) {
        item->setText(model.getLayerName(index));
    }
}

void MainWindow::setupFrameConnections() {
    connect(ui->addFrameButton, &QPushButton::clicked, this, &MainWindow::handleAddFrame);
    connect(ui->deleteFrameButton, &QPushButton::clicked, this, &MainWindow::handleDeleteFrame);
    connect(ui->frameSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::handleFrameSelection);

    connect(&model, &Model::framesChanged, this, &MainWindow::updateFrameDisplay);
    connect(&model, &Model::currentFrameChanged, canvas, &Canvas::setCurrentFrame);
    connect(&model, &Model::currentFrameChanged, this, &MainWindow::updateLayerDisplay);
}

void MainWindow::handleAddFrame() {
    int oldFrame = canvas->getCurrentFrame();
    model.addFrame();
    canvas->setCurrentFrame(oldFrame);
    canvas->addFrame();
    updateFrameDisplay();
    updateLayerDisplay();
}

void MainWindow::handleDeleteFrame() {
    int currentFrame = model.getCurrentFrame();
    model.deleteFrame(currentFrame);
    canvas->deleteFrame(currentFrame);
    updateFrameDisplay();
    updateLayerDisplay();
}

void MainWindow::handleFrameSelection(int index) {
    model.setCurrentFrame(index);
    canvas->setCurrentFrame(index);
    updateLayerDisplay();
}

void MainWindow::updateFrameDisplay() {
    ui->frameSpinBox->blockSignals(true);
    ui->frameSpinBox->setRange(0, model.getFrameCount() - 1);
    ui->frameSpinBox->setValue(model.getCurrentFrame());
    ui->frameSpinBox->blockSignals(false);

    ui->frameCountLabel->setText(QString("Total Frames: %1").arg(model.getFrameCount() - 1));

    ui->deleteFrameButton->setEnabled(model.getFrameCount() > 1);
}
