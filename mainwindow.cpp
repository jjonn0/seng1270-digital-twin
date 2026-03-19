#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QPainter>
#include <Qpixmap>

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPixmap background(":/Stylishback.jpg");

    painter.drawPixmap(this->rect(), background);

    QMainWindow::paintEvent(event);
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPixmap background(":/Stylishback.jpg");

    if (background.isNull()) {
        qDebug() << "FAILED TO LOAD IMAGE";

    // Connect the In-Patients button
    connect(ui->inPatientsButton, &QPushButton::clicked, this, [](){
        QMessageBox::information(nullptr, "In-Patients", "In-Patients management module coming soon!");
    });

    // Connect the Staff Resourcing button
    connect(ui->staffButton, &QPushButton::clicked, this, [](){
        QMessageBox::information(nullptr, "Staff Resourcing", "Staff resourcing module coming soon!");
    });
    }}

MainWindow::~MainWindow()
{
    delete ui;
}
