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

namespace Ui {
float size=0.5;
struct LIGHT light;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    if(!this->backboard.isEmpty()) {
        QPixmap pix;
        //QImage pix;
        // 直接加载资源路径

        if(pix.load(this->backboard)) {
            painter.drawPixmap(0, 0,3072*Ui::size,1836*Ui::size, pix);
        } else {
            qDebug() << "Failed to load pixmap:" << this->backboard;
        }
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    ,rolePaint(nullptr)
{
    ui->setupUi(this);
    rolePaint = new Ui::role_paint(this);
    this->setFixedSize(3072*Ui::size,1836*Ui::size);
    fight::a_fight::tmp();
    update();
    rolePaint->update();
}

MainWindow::~MainWindow()
{
    delete ui;
}

Ui::role_paint::role_paint(QWidget *parent)
    : QWidget(parent)
    , role_path_l(nullptr)
{
    // 设置透明背景
    setAttribute(Qt::WA_TranslucentBackground);
    // 可选：设置固定大小
    setFixedSize(3072*Ui::size, 1836*Ui::size);
}

Ui::role_paint::~role_paint()
{
    // 释放动态分配的内存
    if (role_path_l != nullptr) {
        free(role_path_l);
        role_path_l
            = nullptr;
    }
}

void Ui::role_paint::paintEvent(QPaintEvent *event)
{
    role_path_l=(int *)malloc(role::left_team.num*sizeof(int));
    for(int i=0;i<role::left_team.num;i++){
        role_path_l[i]=role::left_team.each[i].name_number;
    }
    QPainter painter(this);

    for(int i=0;i<role::left_team.num;i++){
        if(role_path_l!=NULL){
            path_role=QString(all_role_path[role_path_l[i]]);
            path_card=QString(all_card_path[role_path_l[i]]);
        }
        else{
            break;
        }
        if(!this->path_role.isEmpty()) {
            QPixmap pix;
            //QImage pix;
            // 直接加载资源路径

            if(pix.load(this->path_role)) {
                painter.drawPixmap(L_role_place[i][0]*Ui::size,L_role_place[i][1]*Ui::size,224*Ui::size,224*Ui::size, pix);
            } else {
                qDebug() << "Failed to load pixmap:" << this->path_role;
            }
        }
        if(!this->path_card.isEmpty()) {
            QPixmap pix;
            //QImage pix;
            // 直接加载资源路径

            if(pix.load(this->path_card)) {
                painter.drawPixmap(L_card_place[i][0]*Ui::size,L_card_place[i][1]*Ui::size,187*Ui::size,567*Ui::size, pix);
            } else {
                qDebug() << "Failed to load pixmap:" << this->path_card;
            }
        }
    }
}


