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
#include <QPen>
#include <QBrush>
#include "war.h"
#include "gacha.h"
namespace Ui {
float size=0.5;
struct LIGHT light;
int dur;
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
        int y=event->pos().y();//分治搜索
        if(x<=1441*Ui::size && x>=114*Ui::size && y>=1149*Ui::size && y<=1716*Ui::size){//左卡
            if(x>777*Ui::size){
                if(x>1223*Ui::size){//1
                    if(role::left_team.num>=1){
                        Ui::light.s=1;
                        Ui::light.type=1;
                        Ui::light.num=0;
                        Ui::light.name=role::left_team.each[0].name_number;
                        lightPaint->update();
                    }
                    else{
                        Ui::light.s=0;
                        lightPaint->update();
                    }
                }
                else if(x>1002*Ui::size){//2
                    if(role::left_team.num>=2){
                        Ui::light.s=1;
                        Ui::light.type=1;
                        Ui::light.num=1;
                        Ui::light.name=role::left_team.each[1].name_number;
                        lightPaint->update();
                    }
                    else{
                        Ui::light.s=0;
                        lightPaint->update();
                    }
                }
                else{//3
                    if(role::left_team.num>=3){
                        Ui::light.s=1;
                        Ui::light.type=1;
                        Ui::light.num=2;
                        Ui::light.name=role::left_team.each[2].name_number;
                        lightPaint->update();
                    }
                    else{
                        Ui::light.s=0;
                        lightPaint->update();
                    }
                }
            }
            else{
                if(x>556*Ui::size){//4
                    if(role::left_team.num>=4){
                        Ui::light.s=1;
                        Ui::light.type=1;
                        Ui::light.num=3;
                        Ui::light.name=role::left_team.each[3].name_number;
                        lightPaint->update();
                    }
                    else{
                        Ui::light.s=0;
                        lightPaint->update();
                    }
                }
                else if(x>335*Ui::size){//5
                    if(role::left_team.num>=5){
                        Ui::light.s=1;
                        Ui::light.type=1;
                        Ui::light.num=4;
                        Ui::light.name=role::left_team.each[4].name_number;
                        lightPaint->update();
                    }
                    else{
                        Ui::light.s=0;
                        lightPaint->update();
                    }
                }
                else{//6
                    if(role::left_team.num>=6){
                        Ui::light.s=1;
                        Ui::light.type=1;
                        Ui::light.num=5;
                        Ui::light.name=role::left_team.each[5].name_number;
                        lightPaint->update();
                    }
                    else{
                        Ui::light.s=0;
                        lightPaint->update();
                    }
                }
            }
        }
        else if(x>=1608*Ui::size){
            if(y>=1167*Ui::size && x<=2921*Ui::size && y<=1701*Ui::size){//右卡
                //printf("1 ok\n");
                if(x<2050*Ui::size){//1
                    if(cha::cha[0]>=0){
                        //printf("2 ok");
                        Ui::light.s=1;
                        Ui::light.type=2;
                        Ui::light.num=0;
                        Ui::light.name=cha::cha[0];
                        lightPaint->update();
                    }
                    else{
                        printf("2 no");
                        Ui::light.s=0;
                        lightPaint->update();
                    }
                }
                else if(x<2488*Ui::size){//2
                    if(cha::cha[1]>=0){
                        Ui::light.s=1;
                        Ui::light.type=2;
                        Ui::light.num=1;
                        Ui::light.name=cha::cha[1];
                        lightPaint->update();
                    }
                    else{
                        Ui::light.s=0;
                        lightPaint->update();
                    }
                }
                else{//3
                    if(cha::cha[2]>=0){
                        Ui::light.s=1;
                        Ui::light.type=2;
                        Ui::light.num=2;
                        Ui::light.name=cha::cha[2];
                        lightPaint->update();
                    }
                    else{
                        Ui::light.s=0;
                        lightPaint->update();
                    }
                }
            }
            else if(x>=1639*Ui::size && x<=2211*Ui::size && y>=149*Ui::size && y<=1094*Ui::size){//大卡

            }
            else if(x>=2385*Ui::size && x<=2868*Ui::size && y>=164*Ui::size && y<=849*Ui::size){//按钮
                if(y<=361*Ui::size){//战斗
                    int l=20;
                    char k[]="kkk";
                    g_log->appendLog("zd"+QString(k)+QString::number(l)+"j %1 %2");
                }
                else if(y>=410*Ui::size && y<=607*Ui::size){//购买/出售
                    if(Ui::light.s==1){
                        if(Ui::light.type==1){//出售
                            if(cha::gacha::cha_sell(Ui::light.num)){
                                fight::a_fight::initialize_role();
                                Ui::light.s=0;
                                all_update();
                            }
                            else{
                            }
                        }
                        else if(Ui::light.type==2){//购买
                            if(cha::gacha::cha_buy(Ui::light.num)){
                                fight::a_fight::initialize_role();
                                Ui::light.s=0;
                                all_update();
                            }
                        }
                    }
                }
                else if(y>=652*Ui::size){//升本
                    if(cha::gacha::cha_up()){
                        cha::gacha::re();
                        Ui::light.s=0;
                        all_update();
                    }
                }
                else{
                    Ui::light.s=0;
                    lightPaint->update();
                }
            }
            else if(x>=2732*Ui::size && x<=2979*Ui::size && y>=913*Ui::size && y<=1162*Ui::size){//刷新
                if(cha::gacha::re()){
                    Ui::light.s=0;
                    all_update();
                }
            }
            else{
                Ui::light.s=0;
                lightPaint->update();
            }
        }
        else{
            Ui::light.s=0;
            lightPaint->update();
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
        all_update();
        this->setFixedSize(3072*Ui::size,1836*Ui::size);
    }
    if(event->key()==Qt::Key_Plus || event->key()==Qt::Key_Equal)
    {
        qDebug()<<"检测到+/=按下";
        Ui::size*=1.05;
        all_update();
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
    ,poolPaint(nullptr)
    ,g_log(nullptr)
{
    // 使用当前时间作为种子
    srand((unsigned)time(NULL));
    ui->setupUi(this);
    poolPaint = new Ui::pool_paint(this);
    rolePaint = new Ui::role_paint(this);
    lightPaint = new Ui::light_paint(this);
    this->setFixedSize(3072*Ui::size,1836*Ui::size);
    // 2. 创建日志窗口，将主窗口设为父级
    LogWindow *logWin = new LogWindow(this); // 关键：父对象为 MainWindow
    g_log  = logWin;
    logWin->show();
    for(int kk=0;kk<100;kk++){g_log->appendLog("程序启动功 "+QString::number(kk));}
    Ui::dur=1;
    Ui::light.s=0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::qMain()
{
    /*  测试用
    fight::a_fight::tmp();
    update();
    rolePaint->update();
    Ui::light.s=0;
    lightPaint->update();
    sleep(5);
    //Ui::light.s=1;
    Ui::light.type=1;
    Ui::light.num=2;
    //lightPaint->update();
    */

    cha::gacha::load_card_pool(all_role_number);
    cha::gacha::cha_begin();
    poolPaint->update();
    return 0;

}

void MainWindow::all_update()
{
    update();
    rolePaint->update();
    lightPaint->update();
    poolPaint->update();
    g_log->update();
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
            if(role::left_team.each[k].current_blood>0){
                //printf("hhh");
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
                        painter.drawText((L_card_place[i][0]+place_to_card[0][0])*Ui::size,(L_card_place[i][1]+place_to_card[0][1])*Ui::size, QString::number(role::left_team.each[k].current_blood)+"/"+QString::number(role::all_role_base[role_path_l[k]].blood));
                        painter.drawText((L_card_place[i][0]+place_to_card[1][0])*Ui::size,(L_card_place[i][1]+place_to_card[1][1])*Ui::size, QString::number(role::left_team.each[k].attack));
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
    if(Ui::light.s && Ui::dur==1){
        if(Ui::light.type==1){//左边
            QPainter painter(this);
            // 设置黄色画笔（用于边框）
            QPen pen(Qt::yellow);
            pen.setWidth(3);  // 设置边框宽度
            painter.setPen(pen);

            // 设置透明填充（或者设置黄色填充）
            painter.setBrush(Qt::NoBrush);  // 透明填充
            // 或者使用黄色填充：painter.setBrush(Qt::yellow);

            // 绘制方形方框 (x, y, width, height)
            painter.drawRect((L_card_place[Ui::light.num][0]-3)*Ui::size, (L_card_place[Ui::light.num][1]-3)*Ui::size, 224*Ui::size, 571*Ui::size);
        }
        else if(Ui::light.type==2){//右边
            QPainter painter(this);
            // 设置黄色画笔（用于边框）
            QPen pen(Qt::yellow);
            pen.setWidth(3);  // 设置边框宽度
            painter.setPen(pen);

            // 设置透明填充（或者设置黄色填充）
            painter.setBrush(Qt::NoBrush);  // 透明填充
            // 或者使用黄色填充：painter.setBrush(Qt::yellow);

            // 绘制方形方框 (x, y, width, height)
            painter.drawRect((card_pool_place[Ui::light.num][0]-3)*Ui::size, (card_pool_place[Ui::light.num][1]-3)*Ui::size, 440*Ui::size, 540*Ui::size);
        }
    }
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
    if(Ui::dur==1){
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
        for(int k=0;k<3;k++){
            if(cha::cha[k]>=0){
                simple_path=QString(simple[cha::cha[k]]);
                if(!this->simple_path.isEmpty()) {
                    QPixmap pix;
                    //QImage pix;
                    // 直接加载资源路径
                    if(pix.load(this->simple_path)) {
                        painter.drawPixmap(card_pool_place[k][0]*Ui::size,card_pool_place[k][1]*Ui::size,434*Ui::size,534*Ui::size, pix);
                    } else {
                        qDebug() << "Failed to load pixmap:" << this->simple_path;
                    }
                }
            }
        }
        {
            QFont font("Arial", 70*Ui::size, QFont::Bold);  // 字体名，大小，粗细
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
            painter.drawText(2611*Ui::size,1080*Ui::size,QString::number(cha::coin));//钱
            painter.drawText(2415*Ui::size,1080*Ui::size,QString::number(cha::shop_level));//本
            painter.drawText(2806*Ui::size,1150*Ui::size,QString::number(cha::shop_level));//刷新
            if(Ui::light.s && Ui::light.type==2){
                painter.drawText(2670*Ui::size,538*Ui::size,QString::number(role::all_role_base[cha::cha[Ui::light.num]].level));
            }
            else if(Ui::light.s && Ui::light.type==1){

            }
            painter.drawText(2670*Ui::size,780*Ui::size,QString::number(cha::shop_level));
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
}


namespace my_log_
{
    QString tmp_log;
    void my_log(const char format[],...)
    {
        char buf[512];
        va_list ap;

        va_start(ap, format);
        // 用 vsprintf_s（安全版）处理可变参数的格式化
        vsprintf_s(buf, sizeof(buf), format, ap);
        va_end(ap);

        // 输出日志（可改为写入文件）
        printf("[Log] %s\n", buf);
    }
}
