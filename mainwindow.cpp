#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QPaintEvent>
#include<QPainter>//绘图
#include<QPixmap>//图片
#include<QCursor>
#include<QMetaEnum>

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPixmap pix;
    pix.load("hh.png");
    QPainter p(this);
    p.drawPixmap(100, 100, 100, 100, pix);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPaintEvent *e;
    MainWindow::paintEvent(e);

}

MainWindow::~MainWindow()
{
    delete ui;
}
