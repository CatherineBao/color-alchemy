///
/// Style checked by:
///
#include <QColorDialog>
#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "Model.h"
#include "Canvas.h"
#include "AnimationPreview.h"

MainWindow::MainWindow(Model& model, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , model(model)
{
    ui->setupUi(this);
    canvas = qobject_cast<Canvas*>(ui->editor);
    animationPreview = qobject_cast<AnimationPreview*>(ui->animationPreview);
    animationPreview->setModel(&model);

    connect(ui->penToolButton, &QPushButton::clicked, &model, &Model::setPen);
    connect(ui->penToolButton, &QPushButton::clicked, &model, [this](){ui->eraserToolButton->setChecked(false);});
    connect(ui->eraserToolButton, &QPushButton::clicked, &model, &Model::setEraser);
    connect(ui->eraserToolButton, &QPushButton::clicked, &model, [this](){ui->penToolButton->setChecked(false);});

    connect(ui->penSize, &QSpinBox::valueChanged, &model, &Model::changePenSize);
    connect(ui->eraserSize, &QSpinBox::valueChanged, &model, &Model::changeEraserSize);

    connect(ui->colorButton, &QPushButton::clicked, this, &MainWindow::openColorPicker);
    connect(this, &MainWindow::colorChanged, &model, &Model::changePenColor);

    connect(ui->layerList, &QListWidget::itemChanged, this,
        [this](QListWidgetItem* item) {
            int row = ui->layerList->row(item);
            bool visible = item->checkState() == Qt::Checked;
            handleLayerVisibilityToggle(row, visible);
        });

    connect(this, &MainWindow::frameRateChanged, animationPreview, &AnimationPreview::updateFramerate);
    connect(ui->fpsBox, QOverload<int>::of(&QSpinBox::valueChanged), animationPreview, [this](){emit frameRateChanged(ui->fpsBox->value());});

    connect(ui->saveButton, &QPushButton::clicked, &model, &Model::saveJSON);
    connect(ui->loadButton, &QPushButton::clicked, &model, &Model::loadJSON);

    connect(canvas, &Canvas::pixelChanged, &model, &Model::drawPixel);
    connect(&model, &Model::redrawCanvas, canvas, &Canvas::onRedraw);
    connect(ui->resizeButton, &QPushButton::clicked, this, &MainWindow::resizeCanvas);

    setupLayerConnections();
    setupFrameConnections();

    updateLayerDisplay();
    updateFrameDisplay();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::openColorPicker() {
    QColor color = QColorDialog::getColor(Qt::white, this, "Select Color");

    if (color.isValid()) {
        emit colorChanged(color);
        QString style = QString("background-color: %1;").arg(color.name());
        ui->colorButton->setStyleSheet(style);
    }
}

void MainWindow::setupLayerConnections() {
    connect(ui->addLayerButton, &QPushButton::clicked, this, &MainWindow::handleAddLayer);
    connect(ui->deleteLayerButton, &QPushButton::clicked, this, &MainWindow::handleDeleteLayer);
    connect(&model, &Model::layersChanged, this, &MainWindow::updateLayerDisplay);

    connect(&model, &Model::currentLayerChanged, this, &MainWindow::syncLayerSelection);

    connect(ui->layerList, &QListWidget::itemChanged, this, &MainWindow::handleLayerNameEdit);
    connect(&model, &Model::layerVisibilityChanged, this, &MainWindow::onLayerVisibilityChanged);
    connect(&model, &Model::layerNameChanged, this, &MainWindow::onLayerNameChanged);

    connect(ui->layerList, &QListWidget::currentRowChanged, this, [this](int row) {
        if(row >= 0) model.setCurrentLayer(row);
    });
}

void MainWindow::syncLayerSelection() {
    int currentLayer = model.getCurrentLayer();
    if(ui->layerList->currentRow() != currentLayer) ui->layerList->setCurrentRow(currentLayer);
}

void MainWindow::handleAddLayer() {
    model.addLayer();
    updateLayerDisplay();
}

void MainWindow::handleDeleteLayer() {
    int currentLayer = model.getCurrentLayer();
    model.deleteLayer(currentLayer);
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
}

void MainWindow::handleLayerVisibilityToggle(int index, bool visible) {
    model.setLayerVisibility(index, visible);
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
    if(item) item->setText(model.getLayerName(index));
}

void MainWindow::setupFrameConnections() {
    connect(ui->addFrameButton, &QPushButton::clicked, this, &MainWindow::handleAddFrame);
    connect(ui->deleteFrameButton, &QPushButton::clicked, this, &MainWindow::handleDeleteFrame);
    connect(ui->frameSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::handleFrameSelection);

    connect(&model, &Model::framesChanged, this, &MainWindow::updateFrameDisplay);
    connect(&model, &Model::currentFrameChanged, this, &MainWindow::updateLayerDisplay);
}

void MainWindow::handleAddFrame() {
    int oldFrame = model.getCurrentFrame();
    model.setCurrentFrame(oldFrame);
    model.addFrame();
    updateFrameDisplay();
    updateLayerDisplay();
}

void MainWindow::handleDeleteFrame() {
    int currentFrame = model.getCurrentFrame();
    model.deleteFrame(currentFrame);
    updateFrameDisplay();
    updateLayerDisplay();
}

void MainWindow::handleFrameSelection(int index) {
    model.setCurrentFrame(index);
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


void MainWindow::resizeCanvas()
{
    int width = ui->widthSpinBox->value();
    int height = ui->heightSpinBox->value();

    delete ui->editor;
    ui->editor = nullptr;

    Canvas* newCanvas = new Canvas(ui->centralwidget, width, height);

    int canvasWidth = width * 10 + 1;
    int canvasHeight = height * 10 + 1;

    ui->editor = newCanvas;
    ui->editor->setGeometry(QRect(456 - width * 4, 90, canvasWidth, canvasHeight));
    ui->editor->show();

    ui->animationPreview->resizeWindow(canvasWidth, canvasHeight);

    canvas = newCanvas;
    connect(canvas, &Canvas::pixelChanged, &model, &Model::drawPixel);
    connect(&model, &Model::redrawCanvas, canvas, &Canvas::onRedraw);

    model = *new Model(nullptr, width, height);

    ui->editor->update();
}
