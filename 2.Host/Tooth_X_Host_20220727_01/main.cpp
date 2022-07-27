#include "widget.h"
#include <qdebug.h>

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();
    qDebug()<<"成功启动";
    return a.exec();
}
