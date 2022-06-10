#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QLabel>
#include <QSpinBox>
#include <QtCore/QIODevice> //This library contains code on how to read data from an Input/Output Device?
#include "spectrogram.h"
#include "qspectrogram.h"
#include "pulsethread.h"
#include "scalogram.h"

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



private slots:
    void about();
    void logPlot();
    void audioDevice();
    void testData2();
    void moveAudioDataToScalogram();



private:
    void plot();
    void plot2();
    void createActions();
    void createActions2();
    void createStatusBar();

//variables
public:
    PulseThread *pulseThread;
private:
    scalogram *Scalogram = new scalogram;
    QLabel *numSelectLabel;
    QSpinBox *numSelectSpinBox;
    QLabel *numSelectLabel2;
    QSpinBox *numSelectSpinBox2;
    Spectrogram *spectrogram;
    QSpectrogram *spectrogramWidget;

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
