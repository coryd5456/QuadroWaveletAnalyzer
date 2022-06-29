
#include "audiodata.h"
#include "qdebug.h"
#include "timer.h"
#include <iostream>
#define LOG(x) std::cout << x <<"\n"

//audioData::audioData(const QAudioDevice &deviceInfo ,QObject *parent) : QIODevice(parent)
audioData::audioData(QObject *parent) : QIODevice(parent)
{
LOG("OMG audioData class was called \n");

}

void newDataPoint(){}



qint64 audioData::readData(char *data, qint64 maxSize)
{
    Q_UNUSED(data);
    Q_UNUSED(maxSize);
    return -1;
}

qint64 audioData::writeData(const char *data, qint64 maxSize)
{
    /*if(SampleCounter ==0){
        bufferTimer.StartDemand();
        SampleCounter = 1;
    } else if(SampleCounter ==1){
        bufferTimer.StopDemand();
        SampleCounter =0;
    }*/
    //LOG("OMG things happened \n");
    //qInfo()<<QThread::currentThread()<<" This is the thread SAMPLING is runing on \n";
    {
        //LOG("Single data point acquisition time\n");
        //Timer timer2;


    dataPoint = (qreal(uchar(*data) -128) / qreal(128))*1000.0;
    //LOG(dataPoint)<<"\n";
    emit newDataPoint();

    if (count >=63){
        //LOG("emitting data\n");
        //qInfo()<<QThread::currentThread()<<" This is the thread SAMPLING is runing on \n";
      emit newCalcDataPoint();
      count = 0;
    } else{
        count++;
        //LOG(count)<<"\n";
    }

    /*
    if(SampleRateCounter == 0){
        Timer sampleTimer;
    } else if(SampleRateCounter == 2000){
        delete sampleTimer;
    }*/
    /*
    if(SampleRateCounter == 2000){
        LOG(SampleRateCounter)<<"\n";
        LOG("Timer")<<"\n";
        timer.Stop();
    }
    SampleRateCounter = SampleRateCounter%2000;
    SampleRateCounter++;
    LOG(SampleRateCounter)<<"\n";
*/
    }

    //count = count%64;
    //count++;
    return 0;
}
