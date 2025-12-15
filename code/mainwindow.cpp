#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QPaintEvent>
#include<QPainter>//绘图
#include<QPixmap>//图片
//#include<QImage>
#include<QCursor>
#include<QMetaEnum>
#include "war.h"
#include "gacha.h"
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
        if(x<=1441*Ui::size && x>=114*Ui::size && y>=1149*Ui::size && y<=1716*Ui::size){//左卡
            if(x>777*Ui::size){
                if(x>1223*Ui::size){//1

                }
                else if(x>1002*Ui::size){//2

                }
                else{//3

                }
            }
            else{
                if(x>556*Ui::size){//4

                }
                else if(x>335*Ui::size){//5

                }
                else{//6

                }
            }
        }
        else if(x>=1608*Ui::size){
            if(y>=1167*Ui::size && x<=2921*Ui::size && y<=1701*Ui::size){//右卡
                if(x<2050*Ui::size){//1

                }
                else if(x<2488*Ui::size){//2

                }
                else{//3

                }
            }
            else if(x>=1639*Ui::size && x<=2211*Ui::size && y>=149*Ui::size && y<=1094*Ui::size){//大卡

            }
            else if(x>=2385*Ui::size && x<=2868*Ui::size && y>=164*Ui::size && y<=849*Ui::size){//按钮
                if(y<=361*Ui::size){//战斗

                }
                else if(y>=410*Ui::size && y<=607*Ui::size){//购买/出售

                }
                else if(y>=652*Ui::size){//升本

                }
            }
            else if(x>=2732*Ui::size && x<=2979*Ui::size && y>=913*Ui::size && y<=1162*Ui::size){//刷新

            }
        }
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    qDebug()<<"按键按下";
    if(event->key()==Qt::Key_Left || event->key()==Qt::Key_A)
    {
        qDebug()<<"检测到Left/A按下";
    }
    if(event->key()==Qt::Key_Right || event->key()==Qt::Key_D)
    {
        qDebug()<<"检测到Righ/D按下";
    }
    if(event->key()==Qt::Key_Minus || event->key()==Qt::Key_Underscore)
    {
        qDebug()<<"检测到-/_按下";
        Ui::size*=0.9523809523809524;
        update();
        rolePaint->update();
        this->setFixedSize(3072*Ui::size,1836*Ui::size);
    }
    if(event->key()==Qt::Key_Plus || event->key()==Qt::Key_Equal)
    {
        qDebug()<<"检测到-/_按下";
        Ui::size*=1.05;
        update();
        rolePaint->update();
        this->setFixedSize(3072*Ui::size,1836*Ui::size);
    }
}

void MainWindow::wheelEvent(QWheelEvent *event)
{

}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    ,rolePaint(nullptr)
    ,lightPaint(nullptr)
{
    // 使用当前时间作为种子
    srand((unsigned)time(NULL));
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
    int i=0;
    for(int k=0;k<role::left_team.num;k++){
            if(role::left_team.each[k].blood>0){
                if(role_path_l!=NULL){
                    path_role=QString(all_role_path[role_path_l[k]]);
                    path_card=QString(all_card_path[role_path_l[k]]);
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
                        painter.drawPixmap(L_card_place[i][0]*Ui::size,L_card_place[i][1]*Ui::size,218*Ui::size,565*Ui::size, pix);
                        QFont font("Arial", 40*Ui::size, QFont::Bold);  // 字体名，大小，粗细
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
                        painter.drawText((L_card_place[i][0]+place_to_card[0][0])*Ui::size,(L_card_place[i][1]+place_to_card[0][1])*Ui::size, QString::number(role::left_team.each[k].blood)+"/"+QString::number(role::all_role_base[role_path_l[k]].blood));
                        painter.drawText((L_card_place[i][0]+place_to_card[1][0])*Ui::size,(L_card_place[i][1]+place_to_card[1][1])*Ui::size, QString::number(role::all_role_base[role_path_l[k]].attack[0])+"d"+QString::number(role::all_role_base[role_path_l[k]].attack[1]));
                        painter.drawText((L_card_place[i][0]+place_to_card[2][0])*Ui::size,(L_card_place[i][1]+place_to_card[2][1])*Ui::size, QString::number(role::left_team.each[i].camp_influence));
                    } else {
                        qDebug() << "Failed to load pixmap:" << this->path_card;
                    }
                }
                ++i;
            }
    }
    free(role_path_l);
    role_path_l=NULL;
}



Ui::light_paint::light_paint(QWidget *parent)
    : QWidget(parent)
{
    // 设置透明背景
    setAttribute(Qt::WA_TranslucentBackground);
    // 可选：设置固定大小
    setFixedSize(3072*Ui::size, 1836*Ui::size);
}

Ui::light_paint::~light_paint()
{

}

void Ui::light_paint::paintEvent(QPaintEvent *event)
{

}

Ui::pool_paint::pool_paint(QWidget *parent)
    : QWidget(parent)
{
    // 设置透明背景
    setAttribute(Qt::WA_TranslucentBackground);
    // 可选：设置固定大小
    setFixedSize(3072*Ui::size, 1836*Ui::size);
}

Ui::pool_paint::~pool_paint()
{

}

void Ui::pool_paint::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    if(!this->backboard.isEmpty()) {//背景
        QPixmap pix;
        //QImage pix;
        // 直接加载资源路径

        if(pix.load(this->backboard)) {
            painter.drawPixmap(0, 0,3072*Ui::size,1836*Ui::size, pix);
        } else {
            qDebug() << "Failed to load pixmap:" << this->backboard;
        }
    }
    int i=0;
    for(int k=0;k<3;k++){
        if(cha::cha[k]>=0){
            simple_path=QString(simple[cha::cha[k]]);
            if(!this->simple_path.isEmpty()) {
                QPixmap pix;
                //QImage pix;
                // 直接加载资源路径
                if(pix.load(this->simple_path)) {
                    painter.drawPixmap(card_pool_place[i][0]*Ui::size,card_pool_place[i][1]*Ui::size,434*Ui::size,534*Ui::size, pix);
                } else {
                    qDebug() << "Failed to load pixmap:" << this->simple_path;
                }
            }
            i++;
        }
    }
    if(Ui::light.s){
        big_path=QString(big[Ui::light.name]);
        if(!this->big_path.isEmpty()) {
            QPixmap pix;
            //QImage pix;
            // 直接加载资源路径
            if(pix.load(this->big_path)) {
                painter.drawPixmap(1639*Ui::size,149*Ui::size,572*Ui::size,945*Ui::size, pix);
            } else {
                qDebug() << "Failed to load pixmap:" << this->big_path;
            }
        }
    }
    if(Ui::light.s==3){
        big_path=QString(big[Ui::light.name]);
        if(!this->big_path.isEmpty()) {
            QPixmap pix;
            //QImage pix;
            // 直接加载资源路径
            if(pix.load(this->big_path)) {
                painter.drawPixmap(1019*Ui::size,10*Ui::size,1083*Ui::size,1787*Ui::size, pix);
            } else {
                qDebug() << "Failed to load pixmap:" << this->big_path;
            }
        }
    }
}


