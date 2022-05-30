#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QLabel>
#include <QSpinBox>
#include "spectrogram.h"
#include "qspectrogram.h"
#include "pulsethread.h"

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QPlainTextEdit;
class QSessionManager;
class QAudioInput;
class QAudioSource;
class QAudioFormat;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow();


private slots:
    void about();
    void logPlot();
    void audioDevice();


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
    QLabel *numSelectLabel;
    QSpinBox *numSelectSpinBox;
    QLabel *numSelectLabel2;
    QSpinBox *numSelectSpinBox2;
    Spectrogram *spectrogram;
    QSpectrogram *spectrogramWidget;

    QAudioInput *m_audioInput = nullptr;
    QAudioSource *m_audioSource = nullptr;
/*public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();*/
};
#endif // MAINWINDOW_H
