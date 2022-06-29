#include "mainwindow.h"
#include <thread>
#include <chrono>
//#include <QThread>
#include <QApplication>
#include <iostream>
#define LOG(x) std::cout << x





int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    //std::thread t2(&quadroTest);
    //std::thread t1(&loopTest);

    //std::thread grabAudioData(&MainWindow::threadScalogramPlot,&w);
    //w.resize(1118,612);
    //QThread* thread = new QThread;
    MainWindow w;
    w.resize(1200,612);//1161
    w.show();
    //connect(thread,&QThread::)
    //thread->start(&threadTest);




    //t1.join();
    //t2.join();


   // std::thread connectData(&MainWindow::threadDataRecord,w);

   // connectData.join();
    //grabAudioData.join();



    return a.exec();
}
