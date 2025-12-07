#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QPaintEvent>
#include<QPainter>//绘图
#include<QPixmap>//图片
//#include<QImage>
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

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button()== Qt::MouseButton::LeftButton)
    {
        qDebug()<<"鼠标左键按下"<<":"<<event->pos();
        int x=event->pos().x();
        int y=event->pos().y();//二叉树搜索
        if(x>1 && x<2 && y>1 && y<2 ){

        }
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    qDebug()<<"按键按下";
    if(event->key()==Qt::Key_Left)
    {
        qDebug()<<"检测到Left按下";
    }
    if(event->key()==Qt::Key_Right)
    {
        qDebug()<<"检测到Right按下";
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    ,rolePaint(nullptr)
    ,lightPaint(nullptr)
{
    ui->setupUi(this);
    rolePaint = new Ui::role_paint(this);
    lightPaint = new Ui::light_paint(this);
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
            //QPixmap pix;
            QImage pix;
            // 直接加载资源路径
            if(pix.load(this->path_role)) {
                // 水平翻转（左右翻折）
                QImage flippedImage = pix.flipped(Qt::Horizontal);

                // 转换为QPixmap绘制
                QPixmap flippedPixmap = QPixmap::fromImage(flippedImage);
                painter.drawPixmap(L_role_place[i][0]*Ui::size,L_role_place[i][1]*Ui::size,224*Ui::size,224*Ui::size, flippedPixmap);
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
                QFont font("Arial", 36*Ui::size, QFont::Bold);  // 字体名，大小，粗细
                // 或者逐个设置
                // font.setFamily("Arial");
                // font.setPointSize(36);
                // font.setBold(true);

                // 设置抗锯齿
                painter.setRenderHint(QPainter::Antialiasing);

                // 应用字体
                painter.setFont(font);

                // 设置白色画笔
                painter.setPen(Qt::white);

                // 绘制数字
                painter.drawText((L_card_place[i][0]+place_to_card[0][0])*Ui::size,(L_card_place[i][1]+place_to_card[0][1])*Ui::size, QString::number(role::left_team.each[i].blood));
                painter.drawText((L_card_place[i][0]+place_to_card[1][0])*Ui::size,(L_card_place[i][1]+place_to_card[1][1])*Ui::size, QString::number(role::left_team.each[i].blood));
                painter.drawText((L_card_place[i][0]+place_to_card[2][0])*Ui::size,(L_card_place[i][1]+place_to_card[2][1])*Ui::size, QString::number(role::all_role_base[role_path_l[i]].attack));
                painter.drawText((L_card_place[i][0]+place_to_card[3][0])*Ui::size,(L_card_place[i][1]+place_to_card[3][1])*Ui::size, QString::number(role::left_team.each[i].camp_influence));
            } else {
                qDebug() << "Failed to load pixmap:" << this->path_card;
            }
        }
    }
}



Ui::light_paint::light_paint(QWidget *parent)
{

}

Ui::light_paint::~light_paint()
{

}

void Ui::light_paint::paintEvent(QPaintEvent *event)
{

}



