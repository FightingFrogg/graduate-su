/*
 *参数的 TableWidget
 *自动启动 Xming
 *帮助文档的窗口
 *
*/



#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
