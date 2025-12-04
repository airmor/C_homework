#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QPaintEvent>
#include<QPainter>//绘图
#include<QPixmap>//图片
#include<QCursor>
#include<QMetaEnum>
#include "war.h"
#define role_wide 1
#define role_high 1
#define role_card_wide 1
#define role_card_high 1

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    if(!this->backboard.isEmpty()) {
        QPixmap pix;
        //QImage pix;
        // 直接加载资源路径

        if(pix.load(this->backboard)) {
            painter.drawPixmap(0, 0, pix);
        } else {
            qDebug() << "Failed to load pixmap:" << this->backboard;
        }
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void Ui::role_paint::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    for(int i=0;i<max_of_a_team;i++){
        if(role_path[i]!=NULL){
            path=QString(role_path[i]);
        }
        else{
            break;
        }
        if(!this->path.isEmpty()) {
            QPixmap pix;
            //QImage pix;
            // 直接加载资源路径

            if(pix.load(this->path)) {
                painter.drawPixmap(0, 0, pix);
            } else {
                qDebug() << "Failed to load pixmap:" << this->path;
            }
        }
    }
}
