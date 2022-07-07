#ifndef AUDIODATA_H
#define AUDIODATA_H
#include "timer.h"
#include <QtCore/QIODevice>
#include <QAudioDevice>
#include <QObject>
#include <QThread>

class audioData : public QIODevice
{
    Q_OBJECT
//added slots here?
protected slots:
    qint64 readData(char *data, qint64 maxSize) override;
    qint64 writeData(const char *data, qint64 maxSize) override;

public:
    //explicit audioData(const QAudioDevice &deviceInfo, QObject *parent = nullptr);
    explicit audioData( QObject *parent = nullptr);
    float dataPoint =0.0;
    void DoSetup(QThread &cThread);

    float m_buffer[40];

public slots:


signals:
    void newDataPoint(float buffer[40]);
    void newCalcDataPoint();

private:
    int SampleCounter =0;
    int count = 0;
    int SampleRateCounter = 0;
    Timer bufferTimer;
};

#endif // AUDIODATA_H
