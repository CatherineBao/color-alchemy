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

    connect(ui->penToolButton, &QPushButton::clicked, canvas, &Canvas::setPen);
    connect(ui->eraserToolButton, &QPushButton::clicked, canvas, &Canvas::setEraser);

    connect(ui->penSize, &QSpinBox::valueChanged, canvas, &Canvas::changePenSize);
    connect(ui->eraserSize, &QSpinBox::valueChanged, canvas, &Canvas::changeEraserSize);

    // NOTE: I'm not sure what this did... but I think I mssed it up
    connect(ui->eraserSize, QOverload<int>::of(&QSpinBox::valueChanged),
            this,
            &MainWindow::handleResizeCanvas);
    connect(ui->penSize, QOverload<int>::of(&QSpinBox::valueChanged),
            this,
            &MainWindow::handleResizeCanvas);
}

MainWindow::~MainWindow()
{
    delete ui;
}


// NOTE: I'm not sure what this did... but I think I mssed it up
void MainWindow::handleResizeCanvas() {
    int width = ui->eraserSize->value();
    int height = ui->penSize->value();
    model.resizePixelGrid(width, height);
}
