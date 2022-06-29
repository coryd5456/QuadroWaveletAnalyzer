#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QLabel>
#include <QSpinBox>
#include <QAbstractSlider>
#include <QSlider>
#include <QThread>
#include <QtCore/QIODevice> //This library contains code on how to read data from an Input/Output Device?
#include "spectrogram.h"
#include "qspectrogram.h"
#include "pulsethread.h"
#include "scalogram.h"
#include "taskhandler.h"
#include "threadtask.h"
#include "audiodatathread.h"

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QPlainTextEdit;
class QSessionManager;
class QAudioInput;
class QAudioSource;
class QAudioFormat;
class QIODevice;
class audioData;
class scalogram;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow();
    void testData();
    void threadDataRecord();
    void threadScalogramPlot();
    void MainThreadTasks();
    void startThreadTest();
    void stopThreadTest();

    QThread* thread = new QThread(this);
    QThread* thread2 = new QThread(this);
    QThread* thread3 = new QThread(this);
    TaskHandler *myTask;
public slots:
    void onNumberChange(int);



private slots:
    void about();
    void logPlot();
    void audioDevice();
    void audioDevice2();
    void testData2();
    void moveAudioDataToScalogram();
    void changeTFResolution();
    void moveDataBuffertoCalculator();




private:
    void plot();
    void plot2();
    void createActions();
    void createActions2();
    void createStatusBar();

//variables
public:
    PulseThread *pulseThread;
    threadTask* gaborTask;
    audioDataThread* audioTask;
private:
    scalogram *Scalogram = new scalogram;
    QLabel *numSelectLabel;
    QSpinBox *numSelectSpinBox;
    QLabel *numSelectLabel2;
    QSpinBox *numSelectSpinBox2;
    Spectrogram *spectrogram;
    QSpectrogram *spectrogramWidget;
    QSlider *numGaborSlider;

    QAudioInput *m_audioInput = nullptr;
    QAudioSource *m_audioSource = nullptr;
    audioData *m_device = nullptr;
    int counter = 0;
    float MW_X[1024];
/*public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();*/
};
#endif // MAINWINDOW_H
