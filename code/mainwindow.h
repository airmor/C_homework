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

QT_BEGIN_NAMESPACE
namespace Ui {
extern float size;
class MainWindow;
class role_paint;
struct LIGHT;
extern struct LIGHT light;
}

QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

protected:
    void paintEvent(QPaintEvent* event)override;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Ui::role_paint *rolePaint;

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
    const int L_role_place[max_of_a_team][2]={{1135,548},{880,548},{632,548},{377,548},{5,548},{6,548}};
    const int R_role_place[max_of_a_team][2]={{1,1},{2,2},{3,3},{4,4},{5,5},{6,6}};
    const int L_card_place[max_of_a_team][2]={{1259,1145},{1069,1145},{878,1145},{688,1145},{496,1145},{305,1145}};
    const int R_card_place[max_of_a_team][2]={{1,1},{2,2},{3,3},{4,4},{5,5},{6,6}};
    const int place_to_card[num_of_attribute][2]={{1,1},{2,2},{3,3},{4,4}};
    const int card_pool_place[max_of_card_pool][2]={{1,1},{2,2},{3,3},{4,4},{5,5},{6,6}};

public:
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

struct Ui::LIGHT{
    int* card_type;
};

#endif // MAINWINDOW_H


