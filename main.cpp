#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow w;
    //w.resize(1118,612);
    w.resize(1161,612);
    w.show();
    return a.exec();
}
