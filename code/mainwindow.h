#include "define_the_const.h"

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include<QMap>
#include<QList>
#include<QUrl>
#include<QTimer>
#include<QEvent>
#include<QMouseEvent>
#include<QContextMenuEvent>
#include<QMenu>
#include <stdio.h>
#include <stdarg.h>
#include "logwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
extern float size;
class MainWindow;
class role_paint;
class light_paint;
class pool_paint;
struct LIGHT;
extern struct LIGHT light;
extern int dur;//阶段 0：战斗 1：抽卡 2：结算
}

namespace my_log_ {
extern QString tmp_log;
void my_log(const char format[256],...);
}

QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

protected:
    void paintEvent(QPaintEvent* event)override;
    void mousePressEvent(QMouseEvent *event)override;
    void keyPressEvent(QKeyEvent *event)override;
    void wheelEvent(QWheelEvent *event)override;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    int main();
    void all_update();

private:
    Ui::MainWindow *ui;
    Ui::role_paint *rolePaint;
    Ui::light_paint *lightPaint;
    Ui::pool_paint *poolPaint;
    // 声明全局日志窗口实例（关键：全局作用域）
    LogWindow *g_log;

private:
    const QString backboard1="source/backboard.png";
    QString backboard=backboard1;
};


//role_paint
class Ui::role_paint : public QWidget
{
    Q_OBJECT

public:
    explicit role_paint(QWidget *parent = nullptr);  // 添加构造函数声明
    ~role_paint();

protected:
    void paintEvent(QPaintEvent* event)override;

private:
    int *role_path_l;
    QString path_role;
    QString path_card;

private:
    const int L_role_place[max_of_a_team][2]={{1248,564},{1004,564},{758,564},{517,564},{271,564},{27,564}};
    const int R_role_place[max_of_a_team][2]={{1600,564},{1844,564},{2090,564},{2331,564},{2577,564},{2821,564}};
    const int L_card_place[max_of_a_team][2]={{1223,1149},{1002,1149},{777,1149},{556,1149},{335,1149},{114,1149}};
    const int R_card_place[max_of_a_team][2]={{1626,1149},{1847,1149},{2068,1149},{2289,1149},{2514,1149},{2735,1149}};
    const int place_to_card[num_of_attribute][2]={{90,290},{93,390},{75,530}};

private:
    const char all_role_path[all_role_number][30]={
        "source/roles/zhanshi.png",
        "source/roles/fashi.png",
        "source/roles/mushi.png",
        "source/roles/sheshou.png"
    };
    const char all_card_path[all_role_number][40]={
        "source/roles/zhanshi_simple.png",
        "source/roles/fashi_simple.png",
        "source/roles/mushi_simple.png",
        "source/roles/sheshou_simple.png"
    };

};

class Ui::light_paint : public QWidget
{

public:
    explicit light_paint(QWidget *parent = nullptr);  // 添加构造函数声明
    ~light_paint();

private:
    const int L_card_place[max_of_a_team][2]={{1223,1149},{1002,1149},{777,1149},{556,1149},{335,1149},{114,1149}};
    const int card_pool_place[max_of_card_pool][2]={{1608,1167},{2050,1167},{2480,1167}};

protected:
    void paintEvent(QPaintEvent* event)override;

};

struct Ui::LIGHT{
    int s;//开关 0：关 1：正常开 2：放大
    int type;//类型1->左边，2->右边,0->没有
    int num;//第几个
    int name;
};

class Ui::pool_paint : public QWidget
{

public:
    explicit pool_paint(QWidget *parent = nullptr);  // 添加构造函数声明
    ~pool_paint();

protected:
    void paintEvent(QPaintEvent* event)override;

private:
    QString simple_path;
    QString big_path;

private:
    const int card_pool_place[max_of_card_pool][2]={{1608,1167},{2050,1167},{2480,1167}};

private:
    QString backboard="source/window/window1.png";
    const char simple[all_role_number][40]={
        "source/roles/zhanshi_simple2.png",
        "source/roles/fashi_simple2.png",
        "source/roles/mushi_simple2.png",
        "source/roles/sheshou_simple2.png"
    };
    const char big[all_role_number][40]={
        "source/roles/zhanshi_mianbang.png",
        "source/roles/fashi_mianbang.png",
        "source/roles/mushi_mianbang.png",
        "source/roles/sheshou_mianbang.png"
    };
};

#endif // MAINWINDOW_H


