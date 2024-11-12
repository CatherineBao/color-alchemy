#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "Model.h"
#include "Canvas.h"

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
    connect(ui->eraserToolButton, &QPushButton::clicked, canvas, &Canvas::setEraser);

    connect(ui->penSize, &QSpinBox::valueChanged, canvas, &Canvas::changePenSize);
    connect(ui->eraserSize, &QSpinBox::valueChanged, canvas, &Canvas::changeEraserSize);

    // NOTE: I'm not sure what this did... but I think I mssed it up
    connect(ui->canvasWidthBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &MainWindow::handleCanvasResize);
    connect(ui->canvasHeightBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &MainWindow::handleCanvasResize);
}

MainWindow::~MainWindow()
{
    delete ui;
}


// NOTE: I'm not sure what this did... but I think I mssed it up
void MainWindow::handleCanvasResize() {
    int width = ui->canvasWidthBox->value();
    int height = ui->canvasHeightBox->value();

    model.resizePixelGrid(width, height);
    canvas->resizeGrid(width, height);
}
