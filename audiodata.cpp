
#include "audiodata.h"
#include <iostream>
#define LOG(x) std::cout << x <<"\n"

audioData::audioData(const QAudioDevice &deviceInfo ,QObject *parent) : QIODevice(parent)
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
    //LOG("OMG things happened \n");

    dataPoint = (qreal(uchar(*data) -128) / qreal(128));
    //LOG(dataPoint)<<"\n";
    emit newDataPoint();
    return 0;
}
