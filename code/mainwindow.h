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
class MainWindow;
class role_paint;
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

private:
    QString backboard;
    const QString backboard1="ff";
};

class Ui::role_paint : public QMainWindow
{

protected:
    void paintEvent(QPaintEvent* event)override;

private:
    char **role_path;
    QString path;

private:
    const int L_role_place[max_of_a_team][2]={{1,1},{2,2},{3,3},{4,4},{5,5},{6,6}};
    const int R_role_place[max_of_a_team][2]={{1,1},{2,2},{3,3},{4,4},{5,5},{6,6}};

};

#endif // MAINWINDOW_H
