#include "mainwindow.h"
#include "war.h"
#include "logwindow.h" // 补充：包含LogWindow的头文件
#include <QApplication>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    w.qMain();
    return a.exec();
}
