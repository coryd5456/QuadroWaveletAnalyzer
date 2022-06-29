#include "taskhandler.h"
#include <QMutex>
TaskHandler::TaskHandler(QObject *parent):
    QThread(parent)
{
    Stop = false;
}

TaskHandler::~TaskHandler(){

}

void TaskHandler::run(){
    for(int i = 0; i<1000;i++){
        QMutex mutex;
        mutex.lock();
        if(this->Stop) break;

        mutex.unlock();

        emit numberChanged(i);

        this->msleep(20);

    }
}
