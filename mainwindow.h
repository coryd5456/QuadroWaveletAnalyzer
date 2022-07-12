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
#include <QGridLayout>
#include <QGroupBox>
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
    void changeTFAmplitude();
    void changeTFRError();
    void changeTFValue();
    void changeTFValue2();
    void moveDataBuffertoCalculator();

    void gaborSliderValueChanged1(int);
    void gaborSliderValueChanged2(int);
    void gaborSliderValueChanged3(int);
    void gaborSliderValueChanged4(int);




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


    QGroupBox *GaborSliderGroup1 = new QGroupBox;
    QGridLayout *GaborSliderLayout1 = new QGridLayout;

    QGroupBox *GaborSliderGroup2 = new QGroupBox;
    QGridLayout *GaborSliderLayout2 = new QGridLayout;

    QGroupBox *GaborSliderGroup3 = new QGroupBox;
    QGridLayout *GaborSliderLayout3 = new QGridLayout;

    QGroupBox *GaborSliderGroup4 = new QGroupBox;
    QGridLayout *GaborSliderLayout4 = new QGridLayout;

    QLabel  *numGaborSliderScalingLabel1;
    QLabel  *numGaborSliderScalingLabel2;
    QLabel  *numGaborSliderScalingLabel3;
    QLabel  *numGaborSliderScalingLabel4;

    QLabel  *numGaborSliderScalingValue1;
    QLabel  *numGaborSliderScalingValue2;
    QLabel  *numGaborSliderScalingValue3;
    QLabel  *numGaborSliderScalingValue4;



    QSlider *numGaborSlider;
    QSlider *numGaborSliderScaling1;
    QSlider *numGaborSliderScaling2;
    QSlider *numGaborSliderScaling3;
    QSlider *numGaborSliderScaling4;

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
