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

    ui->widthSpinBox->setValue(model.getPixelWidth());
    ui->heightSpinBox->setValue(model.getPixelHeight());

    ui->widthSpinBox->setRange(1, 256);
    ui->heightSpinBox->setRange(1, 256);

    connect(ui->penToolButton, &QPushButton::clicked, canvas, &Canvas::setPen);
    connect(ui->eraserToolButton, &QPushButton::clicked, canvas, &Canvas::setEraser);

    connect(ui->widthSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this,
            &MainWindow::handleResizeCanvas);
    connect(ui->heightSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this,
            &MainWindow::handleResizeCanvas);
    ui ->penToolButton->setFocus();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleResizeCanvas() {
    int width = ui->widthSpinBox->value();
    int height = ui->heightSpinBox->value();
    model.resizePixelGrid(width, height);
}

void MainWindow::on_penToolButton_clicked()
{
    ui -> penToolButton -> setFocus();
    emit penClicked();
}
