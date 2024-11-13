#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "Model.h"
#include "Canvas.h"
#include <QColorDialog>

MainWindow::MainWindow(Model& model, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , model(model)
{
    ui->setupUi(this);
    canvas = qobject_cast<Canvas*>(ui->editor);

    ui->penSize->setValue(1);
    ui->eraserSize->setValue(1);
    ui->canvasWidthBox->setValue(36);
    ui->canvasHeightBox->setValue(36);

    ui->canvasWidthBox->setRange(1, 100);
    ui->canvasHeightBox->setRange(1, 100);

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
}

MainWindow::~MainWindow()
{
    delete ui;
}


// NOTE: I'm not sure what this did... but I think I mssed it up
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
