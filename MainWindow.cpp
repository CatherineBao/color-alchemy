#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "Model.h"

MainWindow::MainWindow(Model& model, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , model(model)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
