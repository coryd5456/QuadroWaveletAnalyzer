
#include "audiodata.h"
#include "qdebug.h"
#include "timer.h"
#include <iostream>
#include "Instrumentor.h"
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
    InstrumentationTimer timer("Data Collection Event");

    if(SampleCounter ==0){
        bufferTimer.StartDemand();
        SampleCounter = 1;
    } else if(SampleCounter ==1){
        bufferTimer.StopDemand();
        SampleCounter =0;
    }
    //LOG("OMG things happened \n");
    //qInfo()<<QThread::currentThread()<<" This is the thread SAMPLING is runing on \n";

        //LOG("Single data point acquisition time\n");
        //Timer timer2;
    //Need to write to an array instead
    for (int s = 0; s < maxSize; s++, data++){
        m_buffer[s] = (qreal(uchar(*data) -128) / qreal(128))*200.0f;
        //LOG(qreal(uchar(*data) -128) / qreal(128));
        //LOG(int(*data));
    }


    //dataPoint = (qreal(uchar(*data) -128) / qreal(128))*1000.0f;
    LOG(maxSize)<<"\n";
    //Transfer the array over.
    emit newDataPoint(m_buffer);
    /*
    if (count >=63){
        //LOG("emitting data\n");
        //qInfo()<<QThread::currentThread()<<" This is the thread SAMPLING is runing on \n";
      emit newCalcDataPoint();
      count = 0;
    } else{
        count++;
        //LOG(count)<<"\n";
    }*/

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


    //count = count%64;
    //count++;
    return 0;
}
