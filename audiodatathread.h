#ifndef AUDIODATATHREAD_H
#define AUDIODATATHREAD_H

#include "audiodata.h"
#include <QObject>
#include <QThread>
#include <QAudioInput>
#include <QAudioSource>
#include <QAudioOutput>
#include <QAudioFormat>
#include <QAudioDevice>
#include <QtMultimedia/QMediaDevices>

class audioDataThread : public QObject
{
    Q_OBJECT
public:
    explicit audioDataThread(QObject *parent = nullptr);
    void DoSetup(QThread &cThread);

public slots:
    void DoWork();
    void toEmit_datapointReady();
    void toEmit_dataCalcReady();
signals:
    void datapointReady(float *);
    void dataCalcReady();

//public variables
public:
    //const QAudioDevice inputDevice;
    QAudioInput *m_audioInput = nullptr;
    QAudioSource *m_audioSource = nullptr;
    audioData *m_device = nullptr;

};

#endif // AUDIODATATHREAD_H
