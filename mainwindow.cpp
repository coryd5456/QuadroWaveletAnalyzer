#include <QtWidgets>
#include <QAudio>
#include <QAudioInput>
#include <QAudioFormat>
#include <QAudioDevice>
//#include <QtMultimedia/QAudioDevice>
#include <QtMultimedia/QMediaDevices>
//#include <QtWidgets/QMessageBox>
//#include <QtMultimedia/QAudioInput>
//#include <QtMultimedia/QAudioOutput>
//#include <QtMultimedia/QAudioSource>

#include "spectrogram.h"
#include "qspectrogram.h"
#include "qcustomplot.h"
#include "pulsethread.h"
#include "waveletMath.h"
#include "mainwindow.h"
#include "scalogram.h"
#include <iostream>
#include <math.h>
#define LOG(x) std::cout << x
#define dCUP(x) LOG(x)
MainWindow::MainWindow()
    : QMainWindow(new QWidget)
{

    //menu and tool bars
    createActions();
    createActions2();
    //different main windows
    logPlot();
    plot2();
    plot();

    audioDevice();

    //messing around starter widget
    scalogram *Scalogram = new scalogram;
    QMainWindow::setCentralWidget(Scalogram);

    //status bar
    createStatusBar();
}

void MainWindow::audioDevice(){
    //Figure out what this is doing
    const QAudioDevice inputDevice = QMediaDevices::defaultAudioInput();
    //List of Audio Devices
    QList<QAudioDevice> devices;

    /*
    if (inputDevice.isNull()) {
        QMessageBox::warning(nullptr, "audio",
                             "There is no audio input device available.");
        return -1;
    }*/
    m_audioInput = new QAudioInput(inputDevice, this);
    LOG("Data from the microphone (" + inputDevice.description().toStdString() + ')');

    QAudioFormat formatAudio;
    formatAudio.setSampleRate(8000);
    formatAudio.setChannelCount(1);
    formatAudio.setSampleFormat(QAudioFormat::UInt8);

    //m_audioInput.


   LOG("Testing \n");
}

void MainWindow::plot(){
    //QSpinBox *numSelectSpinBox = qobject_cast<QSpinBox *>(sender());
        double a = numSelectSpinBox->value();
        double b = numSelectSpinBox2->value();

    //QString::number(5);
    QCustomPlot *customPlot = new QCustomPlot(this);
    // generate some data:
    QVector<double> x(101), y(101); // initialize with entries 0..100

    for (int i=0; i<101; i++)
    {
      x[i] = (a/40)*cos(2*M_PI*(i/100.0 ))+b/20-0.5; // x goes from -1 to 1
      y[i] = (a/40)*sin(2*M_PI*(i/100.0 ))+0.5; // let's plot a quadratic function
    }
    // create graph and assign data to it:
    customPlot->addGraph();
    customPlot->graph(0)->setData(x, y);
    // give the axes some labels:
    customPlot->xAxis->setLabel("x");
    customPlot->yAxis->setLabel("y");
    // set axes ranges, so we see all data:
    customPlot->xAxis->setRange(-1, 1);
    customPlot->yAxis->setRange(0, 1);
    customPlot->replot();
    QMainWindow::setCentralWidget(customPlot);
    float f[101];
    float g[101];
    for(int i = 0; i<101; i++){
        f[i] = x[i];
        g[i] = y[i];
    }
    dCUP(pNorm(f,101,2.0));
    dCUP("\n");
    float* ptr = DWFT(f,101,g,10);
    dCUP(ptr[(int)b]);
    dCUP("\n");

    dCUP("Boob Size: ");
    dCUP(a);
    dCUP("\n")<<std::endl;
}

void MainWindow::plot2(){
    //QSpinBox *numSelectSpinBox = qobject_cast<QSpinBox *>(sender());
        double a = numSelectSpinBox->value();
        double b = numSelectSpinBox2->value();

    //QString::number(5);
    QCustomPlot *customPlot = new QCustomPlot(this);
    // generate some data:
    QVector<double> x(101), y(101); // initialize with entries 0..100

    for (int i=0; i<101; i++)
    {
      x[i] = (a/40)*cos(2*M_PI*(i/100.0 ))+b/20-0.5; // x goes from -1 to 1
      y[i] = (a/40)*tan(2*M_PI*(i/100.0 ))+0.5; // let's plot a quadratic function
    }
    // create graph and assign data to it:
    customPlot->addGraph();
    customPlot->graph(0)->setData(x, y);
    // give the axes some labels:
    customPlot->xAxis->setLabel("x");
    customPlot->yAxis->setLabel("y");
    // set axes ranges, so we see all data:
    customPlot->xAxis->setRange(-1, 1);
    customPlot->yAxis->setRange(0, 1);
    customPlot->replot();
    QMainWindow::setCentralWidget(customPlot);
    float f[101];
    float g[101];
    for(int i = 0; i<101; i++){
        f[i] = x[i];
        g[i] = y[i];
    }
    dCUP(pNorm(f,101,2.0));
    dCUP("\n");
    float* ptr = DWFT(f,101,g,10);
    dCUP(ptr[(int)b]);
    dCUP("\n");

    dCUP("Boob Size: ");
    dCUP(a);
    dCUP("\n")<<std::endl;
}

void MainWindow::about()
{
   QMessageBox::about(this, tr("About Application"),
            tr("The <b>Application</b> example demonstrates how to "
               "write modern GUI applications using Qt, with a menu bar, "
               "toolbars, and a status bar."));
}
//This is the spectrogram plot that I'm trying to understand
////////////////////Steps to learn////////////////////////
/// \brief MainWindow::logPlot
/// 1. Go throuh Qspectragram to understand how the widget is laid out
/// 2.
/// 3.
/// 4.
/// 5.
/// 6.
/// 7.
///
///
/// /////////////////////////////////////////////////////


void MainWindow::logPlot()
//! [13] //! [14]
{
    //creates new spectragram with the following inputs
    //Spectrogram(
    // unsigned int _sampleRate,
    // unsigned int _sampleLength,
    // unsigned int _samplesPerLine,
    // unsigned int _numLines);
    spectrogram = new Spectrogram(44100, 44100 * 60, 256, 8192);
    //Qspectragram takes in the spectragram and turns it into a QT widget
    spectrogramWidget = new QSpectrogram(spectrogram, this);
    //Qspectragrams are QT widgets
    setCentralWidget(spectrogramWidget);

    //My silly attempts:
    //QString device("NULL");
    //QString device("alsa_input.pci-0000_0a_00.4.analog-stereo");
    //QString device("alsa_output.pci-0000_08_00.1.hdmi-stereo");
    //your choice of device to input into the specragram
    QString device("alsa_input.usb-Generic_Blue_Microphones_2113BAB095L8-00.analog-stereo");
    //uses pulse audio driver to read the device.
    pulseThread = new PulseThread(device, 44100, 1024);
    pulseThread->start();
    //I need to master this connect to use these unctions
    //sends pulse thread to the process data function in Qspectrogram to be used as the data
    // for the fft and audio plots
    //whenever the buffer is filled, then it re-draws the whole thing.
    connect(pulseThread, SIGNAL(bufferFilled(float*,uint)),
            spectrogramWidget, SLOT(processData(float*,uint)));
}

//! [16]

//! [17]
//!
//! This is the menubar and tool bar creator
void MainWindow::createActions()
//! [17] //! [18]
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    QToolBar *fileToolBar = addToolBar(tr("File"));
    const QIcon newIcon = QIcon(":/QEE.png");
    //const QIcon newIcon = QIcon::fromTheme("document-open", QIcon(":/images/open.png"));
    //const QIcon newIcon;
    QAction *newAct = new QAction(newIcon, tr("&Change"), this);
    //newAct->setIcon(QIcon("/home/cory/QtProjects/WaveletAnalyzer/images/QEE.png"));
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, &QAction::triggered, this, &MainWindow::about);
    fileMenu->addAction(newAct);
    fileToolBar->addAction(newAct);

//! [19]
    const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(":/images/open.png"));
    QAction *openAct = new QAction(openIcon, tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, &QAction::triggered, this, &MainWindow::about);
    fileMenu->addAction(openAct);
    fileToolBar->addAction(openAct);
//! [18]

    const QIcon saveIcon = QIcon::fromTheme("document-save", QIcon(":/images/QEE.png"));
    QAction *saveAct = new QAction(saveIcon, tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, &QAction::triggered, this, &MainWindow::about);
    fileMenu->addAction(saveAct);
    fileToolBar->addAction(saveAct);

    numSelectSpinBox = new QSpinBox;
    numSelectSpinBox->setRange(0, 20);
    numSelectSpinBox->setSpecialValueText(tr("0 (cosmetic pen)"));

    numSelectLabel = new QLabel(tr("Pen &W&idth:"));
    numSelectLabel->setBuddy(numSelectSpinBox);
    fileToolBar->addWidget(numSelectSpinBox);
    connect(numSelectSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &MainWindow::plot);
    connect(numSelectSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &MainWindow::plot2);


//! [20]

    fileMenu->addSeparator();

    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    QAction *aboutAct = helpMenu->addAction(tr("&About"), this, &MainWindow::about);
    aboutAct->setStatusTip(tr("Show the application's About box"));

//! [22]

    QAction *aboutQtAct = helpMenu->addAction(tr("About &Qt"), qApp, &QApplication::aboutQt);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));

}
//Change the names of these to match what I am having them do
void MainWindow::createActions2()
//! [17] //! [18]
{
    QIcon::setFallbackSearchPaths(QIcon::fallbackSearchPaths() << ":images/icon-images.qrc");

    QMenu *plotMenu = menuBar()->addMenu(tr("&Plot"));
    QToolBar *plotToolBar = addToolBar(tr("Plot"));
    plotToolBar->setOrientation(Qt::Vertical);
    addToolBar(Qt::LeftToolBarArea,plotToolBar);
    const QIcon spectrumPlotIcon = QIcon::fromTheme("document-new", QIcon(":/images/new.png"));
    QAction *spectrumPlotAct = new QAction(spectrumPlotIcon, tr("&Spectragram"), this);
    spectrumPlotAct->setShortcuts(QKeySequence::New);
    spectrumPlotAct->setStatusTip(tr("Check Spectragram"));
    connect(spectrumPlotAct, &QAction::triggered, this, &MainWindow::logPlot);
    plotMenu->addAction(spectrumPlotAct);
    plotToolBar->addAction(spectrumPlotAct);

//! [19]
    const QIcon plot1Icon = QIcon::fromTheme("document-cut", QIcon(":/images/cut.png"));
    QAction *plot1Act = new QAction(plot1Icon, tr("&Plot1"), this);
    plot1Act->setShortcuts(QKeySequence::Open);
    plot1Act->setStatusTip(tr("Plot primary plot to screen"));
    connect(plot1Act, &QAction::triggered, this, &MainWindow::plot);
    plotMenu->addAction(plot1Act);
    plotToolBar->addAction(plot1Act);
//! [18] //! [19]
/*
    const QIcon plot2Icon = QIcon::fromTheme("document-new", QIcon(":/images/new.png"));
    QAction *plot2Act = new QAction(plot2Icon, tr("&Plot2"), this);
    plot2Act->setShortcuts(QKeySequence::Save);
    plot2Act->setIcon(QIcon(":/images/save.png"));
    plot2Act->setStatusTip(tr("Save the document to disk"));
    connect(plot2Act, &QAction::triggered, this, &MainWindow::plot2);
    plotMenu->addAction(plot2Act);
    plotToolBar->addAction(plot2Act);*/

    const QIcon plot2Icon = QIcon::fromTheme("document-open", QIcon(":/images/open.png"));
    QAction *plot2Act = new QAction(plot2Icon, tr("&Plot2"), this);
    plot2Act->setShortcuts(QKeySequence::Save);
    //plot2Act->setIcon(QIcon(":/images/QEE.png"));
    plot2Act->setStatusTip(tr("Plot secondary plot to screen"));
    connect(plot2Act, &QAction::triggered, this, &MainWindow::plot2);
    plotMenu->addAction(plot2Act);
    plotToolBar->addAction(plot2Act);

    numSelectSpinBox2 = new QSpinBox;
    numSelectSpinBox2->setRange(0, 20);
    numSelectSpinBox2->setSpecialValueText(tr("0 (cosmetic pen)"));

    numSelectLabel2 = new QLabel(tr("Pen &W&idth:"));
    numSelectLabel2->setBuddy(numSelectSpinBox2);
    plotToolBar->addWidget(numSelectSpinBox2);
    connect(numSelectSpinBox2, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &MainWindow::plot);
    connect(numSelectSpinBox2, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &MainWindow::plot2);
//! [20]

    //plotMenu->addSeparator();

    QMenu *helpMenu = menuBar()->addMenu(tr("&About"));
    QAction *aboutAct = helpMenu->addAction(tr("&About"), this, &MainWindow::about);
    aboutAct->setStatusTip(tr("Show the application's About box"));


//! [22]

    QAction *aboutQtAct = helpMenu->addAction(tr("About &Qt"), qApp, &QApplication::aboutQt);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));

}
//! [24]

//! [32]
void MainWindow::createStatusBar()
//! [32] //! [33]
{
    statusBar()->showMessage(tr("Ready"));
}






