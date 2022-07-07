#include "audiodatathread.h"
#include "Instrumentor.h"
#include "qdebug.h"
#include <iostream>
#define LOG(x) std::cout << x

audioDataThread::audioDataThread(QObject *parent)
{
    /*
    const QAudioDevice inputDevice = QMediaDevices::defaultAudioInput();

    m_audioInput = new QAudioInput(inputDevice);
    LOG("Data from the microphone (" + inputDevice.description().toStdString() + ')');

    QAudioFormat formatAudio;
    formatAudio.setSampleRate(4000);
    formatAudio.setChannelCount(1);
    formatAudio.setSampleFormat(QAudioFormat::UInt8);

    m_audioSource = new QAudioSource(inputDevice, formatAudio);
    m_audioSource->setBufferSize(200);


    m_device = new audioData(inputDevice);
    m_device->open(QIODevice::WriteOnly);
    qInfo()<<QThread::currentThread()<<" This is the thread SAMPLING is runing on \n";
    */
}

void audioDataThread::DoSetup(QThread &cThread){
    connect(&cThread, &QThread::started, this , &audioDataThread::DoWork);
}

void audioDataThread::DoWork()
{
    InstrumentationTimer timer("Worker Thread");
    qInfo()<<QThread::currentThread()<<" This is the thread SAMPLING is runing on \n";
    const QAudioDevice inputDevice = QMediaDevices::defaultAudioInput();

    m_audioInput = new QAudioInput(inputDevice);
    LOG("Data from the microphone (" + inputDevice.description().toStdString() + ')');



    QAudioFormat formatAudio;
    formatAudio.setSampleRate(4000);
    formatAudio.setChannelCount(1);
    formatAudio.setSampleFormat(QAudioFormat::UInt8);
    //formatAudio.bytesPerSample();


    m_audioSource = new QAudioSource(inputDevice, formatAudio);
    m_audioSource->setBufferSize(1);


    qInfo()<<inputDevice.supportedSampleFormats()<<"\n";
    qInfo()<<formatAudio.bytesForDuration(10000)<<"\n";
    qInfo()<<formatAudio.bytesPerFrame()<<"\n";
    qInfo()<<formatAudio.sampleRate()<<"\n";
    qInfo()<<inputDevice.maximumSampleRate()<<"\n";



    //m_device = new audioData(inputDevice);
    m_device = new audioData;
    m_device->open(QIODevice::WriteOnly);

    //Now I need to re-impliment these
    //I can make connects to connects out of this class to the main class.
    //connect(m_device, &audioData::newDataPoint, this, &MainWindow::moveAudioDataToScalogram);
    //connect(m_device, &audioData::newCalcDataPoint,this,&MainWindow::moveDataBuffertoCalculator);

    //emmitters to the main class
    connect(m_device, &audioData::newDataPoint, this, &audioDataThread::toEmit_datapointReady);
    connect(m_device, &audioData::newCalcDataPoint,this,&audioDataThread::toEmit_dataCalcReady);
    //connect(m_device,&QIODevice::readyRead,this,&audioDataThread::toEmit_datapointReady);
    m_audioSource->start(m_device);
    qInfo()<<m_audioSource->format()<<"\n";



}

void audioDataThread::toEmit_datapointReady(float m_buffer[])
{
    //needs to also send the data recorded.
    emit datapointReady(m_buffer);
    //qInfo()<<m_audioSource->bufferSize()<<"\n";
    //LOG("ready to read data")<<"\n";
}

void audioDataThread::toEmit_dataCalcReady()
{
    emit dataCalcReady();
}


