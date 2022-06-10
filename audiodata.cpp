
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
    //LOG(qreal(uchar(*data) -128) / qreal(128));
    dataPoint = (qreal(uchar(*data) -128) / qreal(128))*qreal(1000);
    emit newDataPoint();
    //Code grabbed from QT audio example
    /*
    static const int resolution = 4;

    if (m_buffer.isEmpty()) {
        m_buffer.reserve(sampleCount);
        for (int i = 0; i < sampleCount; ++i)
            m_buffer.append(QPointF(i, 0));
    }

    int start = 0;
    const int availableSamples = int(maxSize) / resolution;
    if (availableSamples < sampleCount) {
        start = sampleCount - availableSamples;
        for (int s = 0; s < start; ++s){
            m_buffer[s].setY(m_buffer.at(s + availableSamples).y());
            //LOG(m_buffer.at(s + availableSamples).y());
        }
    }

    for (int s = start; s < sampleCount; ++s, data += resolution){
        m_buffer[s].setY(qreal(uchar(*data) -128) / qreal(128));
        LOG(qreal(uchar(*data) -128) / qreal(128));
    }

    m_series->replace(m_buffer);
    return (sampleCount - start) * resolution;
    */
    return 0;
}
