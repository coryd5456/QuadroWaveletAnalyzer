#include <QtWidgets>
#include <QAudio>
#include <QAudioInput>
#include <QAudioSource>
#include <QAudioFormat>
#include <QAudioDevice>
#include <QAbstractSlider>
#include <QSlider>
#include <QTimer>
#include "Instrumentor.h"
//#include <QtMultimedia/QAudioDevice>
#include <QtMultimedia/QMediaDevices>
//#include <QtWidgets/QMessageBox>
//#include <QtMultimedia/QAudioInput>
//#include <QtMultimedia/QAudioOutput>
//#include <QtMultimedia/QAudioSource>

//Adding threading capability
//#include <thread>
//#include <future>
//#include <mutex>
//#include <chrono>
#include <QThread>

#include "threadtask.h"
#include "taskhandler.h"
#include "audiodata.h"
#include "spectrogram.h"
#include "qspectrogram.h"
#include "qcustomplot.h"
#include "pulsethread.h"
#include "waveletMath.h"
#include "mainwindow.h"
#include "scalogram.h"
#include "formantplot.h"
#include <iostream>
#include <math.h>
#include "timer.h"
#define PI 3.14159265
#define W_exp 2.7182818
#define LOG(x) std::cout << x
#define dCUP(x) LOG(x)

std::mutex m;
/////
/// \brief MainWindow::MainWindow
/// Project Tasks
/// 1. add fft capability
/// 2. add fgt/fwt capability
/// 3. add in multithreading RTOS structure to sample data and compute it at the same time.
/// The goal of this is to not screw up my sampling rate by having them both on the same thread
///      -I need audio device sampling in 1 thread
///         each sample needs to send to the buffer that I'm computing on.
///     -Scalogram rendering in a different thread
///
/// 4. Where are things going in RAM, how much RAM do I need?
/// 5. I need to make threading classes aparently
///
/// I think the problem I have going on is that the constructor has to finish constructing in order for the application to start.
/// 1. to test this lets make a button that starts a thread printing to the console.(done)
/// 2. my guess was correct. now to impliment a few things
///     a. impliment a delta-time updator thread
///     b. impliment a sampling thread
///     c. impliment a computation thread
/// ---After these are implimented if Gabor transform needs to be faster impliment async Futures threads
///     d. Make a start recording and stop recording buttom
///
///

MainWindow::MainWindow()
    : QMainWindow(new QWidget)
{
    InstrumentationTimer timer("Main Window");
    QThread::currentThread()->setObjectName("Main Thread");
    LOG("Main Thread name: ")<< QThread::currentThread() << "\n";
    //menu and tool bars
    createActions();
    createActions2();
    //different main windows
    //logPlot();
    plot2();
    plot();
    //status bar
    createStatusBar();
    //audioDevice();
    audioDevice2();
    QMainWindow::setCentralWidget(Scalogram);
    //set centralWidget prevents me from multithreading
    //QObject::moveToThread: Widgets cannot be moved to a new thread
    //QObject::moveToThread: Cannot move objects with a parent
    //Scalogram->moveToThread(thread);
    //Scalogram->gaboorThreadSetup(*thread);


    //menu and tool bars
    //createActions();
    //createActions2();
    //different main windows
    //logPlot();
    //plot2();
    //plot();
    //status bar
    //createStatusBar();

    //This is to try and just print data to the console as fast as possibe in parallel with whatever else I'm doing.
    // t5 runs foo::bar() on object f
    ////audioDevice();


    //grabAudioData.join();
    ////QMainWindow::setCentralWidget(Scalogram);
    //auto a1 = std::async(std::launch::async,&MainWindow::threadDataRecord,this);
    //auto a2 = std::async(std::launch::async,&MainWindow::threadScalogramPlot,this);
    //a2.wait();
    //QFuture<void> future = QtConcurrent::task(&MainWindow::audioDevice).spawn();
    //testData();

    //messing around starter widget
    //auto thread = new QThread;
    //connect(thread,&QThread::started, this, &MainWindow::threadScalogramPlot);


    //auto thread2 = new QThread;
    //connect(thread2, &QThread::started, this, &MainWindow::MainThreadTasks);
    //thread->start();
    //thread2->start();


    myTask = new TaskHandler();
    connect(myTask, SIGNAL(numberChanged(int)),this, SLOT(onNumberChange(int)));

    //threadTask* gaborTask = new threadTask(this);
    //removing the parent put it on a different thread.

    //gaborTask = new threadTask();
    //gaborTask->gaboorThreadSetup(*thread);
    //gaborTask->moveToThread(thread);
    //thread->setObjectName("Gabor Thread 1");
    thread3->setObjectName("Fuck yes");

    //QObject::connect(gaborTask,&threadTask::imageComplete,Scalogram, &scalogram::imageRecived);

    //I should have it just pause instead of quit
    //QObject::connect(Scalogram,&scalogram::imageWorkComplete,thread,&QThread::quit);


    /*
    formantPlot *FormantPlot = new formantPlot(Scalogram->F);
    //Value transfer isn't happening need to figure out why to get this to work.
    //Need to figure out how to transfer just one value before transfering all of them.
    LOG("Value transfer testing:\n");
    for(int i = 0; i< 1024 ;i++){
        FormantPlot->F[i][0] = (int)(Scalogram->F[i][0]);
        FormantPlot->F[i][1] = (int)(Scalogram->F[i][1]);
        LOG((int)(Scalogram->F[i][1])) << "screaming AHHHH\n";
    }
    //Turn this into a button? Since it's a pop up anyway.
    FormantPlot->show();*/








}



void MainWindow::startThreadTest(){
    InstrumentationTimer timer("startThreadTest");
    myTask->Stop = false;
    //myTask->start();
    //thread->start();
    thread3->start();
}

void MainWindow::stopThreadTest(){
    myTask->Stop = true;
    //myTask->~TaskHandler();
}

void MainWindow::testData(){
    QTimer *timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &MainWindow::testData2);
        timer->start(1);

}

void MainWindow::onNumberChange(int i){
    //Scalogram->update();
    LOG(i)<<" Oh things are happening here" <<"\n";
}

void MainWindow::testData2(){

    for (int n = 0; n < 1024; ++n)
    {
        //MW_X[n] = 5.0 + 2.0*sin(600.0*PI*(double)(n)/1024.0 + 10.0*PI*cos(50.0*2.0*PI*(double)(n)/1024.0)) + 3.0*cos(800.0*PI*(double)(n)/1024.0) + 40.0 * pow(W_exp,-1*pow(((double)(n)-1024.0/4.0)*2.0,2.0)) ;//+ 9.0*cos(16*PI*PI*n/N);
        //LOG(W_X[n])<<"\n";
        //W_X[n] = 5.0 + sinh(2.0*n/N);
        //W_X[n] = 5.0 + 3.0*cos(4*PI*n/N + 12.0*cos(4*PI*n/N));
        //Two Linear Chirps
        MW_X[n] = 1.0+5.0*cos(300.0*PI*pow((double)(n)/(double)(1024),2.0) + 100.0*PI*((double)(n)/(double)(1024)))+5.0*cos(300.0*PI*pow((double)(n)/(double)(1024),2.0)) ;// + 5.0*cos(-300.0*PI*pow((double)(n)/(double)(N),3.0) + 600.0*PI*((double)(n)/(double)(N)));
    }
    Scalogram->dataPoint = MW_X[counter];
    Scalogram->update();
    //LOG(MW_X[counter]) << "\n";
    counter++;
    counter = counter%1024;

}

void MainWindow::audioDevice2(){
    InstrumentationTimer timer("audio Device 2");
    audioTask = new audioDataThread();
    audioTask->DoSetup(*thread3);
    audioTask->moveToThread(thread3);
    //Write new versions of these:
    //connect(m_device, &audioData::newDataPoint, this, &MainWindow::moveAudioDataToScalogram);
    //connect(m_device, &audioData::newCalcDataPoint,this,&MainWindow::moveDataBuffertoCalculator);
    //These connections drastically increase the time for the data to be done by.
    //I should move my buffer to audio task?
    //get proper timing of the audio task
    //NOTE on sample rates
    // 1000: 1000us
    // 2000:  500us
    // 4000:  250us
    // 8000:  125us
    //16000:  62.5us
    //
    /// This connect at 4us extra should be more than enough time. Upper limit is 125000 sample rate
    connect(audioTask, &audioDataThread::datapointReady, Scalogram, &scalogram::dataBuffer);
    //connect(audioTask, &audioDataThread::dataCalcReady,this,&MainWindow::moveDataBuffertoCalculator);
}



void MainWindow::audioDevice(){

    //I think I need to move all of this to my audio data class, then move that to a thread with the worker model.
    std::lock_guard<std::mutex> lk(m);
    //Figure out what this is doing
    const QAudioDevice inputDevice = QMediaDevices::defaultAudioInput();
    //List of Audio Devices
    QList<QAudioDevice> devices;

    for(int i = 0; i< devices.length();i++){
        LOG(devices[i].description().toStdString() +"\n");
    }

    /*
    if (inputDevice.isNull()) {
        QMessageBox::warning(nullptr, "audio",
                             "There is no audio input device available.");
        return -1;
    }*/
    //m_audioInput = new QAudioInput(inputDevice, this);
    m_audioInput = new QAudioInput(inputDevice);
    LOG("Data from the microphone (" + inputDevice.description().toStdString() + ')');

    QAudioFormat formatAudio;
    formatAudio.setSampleRate(4000);
    formatAudio.setChannelCount(1);
    formatAudio.setSampleFormat(QAudioFormat::UInt8);

    m_audioSource = new QAudioSource(inputDevice, formatAudio);
    m_audioSource->setBufferSize(200);
    //m_audioSource->format();


    //m_device = new audioData(inputDevice);
    m_device = new audioData;
    m_device->open(QIODevice::WriteOnly);

    //m_device->moveToThread(thread3);

    //This starts the aquisition of data from the audio device using the write/read data function from audioData
    //need to add connection to data
    //need to fix my connect function.
    //whenever a new data point is recorded it runs this function
    //how do I make a thread for the recording of data and a thread for the plotting of data on screen?
    //moves the data to the buffer for every data point

    //connect(thread, &QThread::started, m_device,&audioData::newDataPoint);
    //m_device->moveToThread(thread3);

    //connect(m_device, &audioData::newDataPoint, this, &MainWindow::moveAudioDataToScalogram);
    //connect(m_device, &audioData::newCalcDataPoint,this,&MainWindow::moveDataBuffertoCalculator);

    //connect(thread3,&QThread::started,m_device,&audioData::newDataPoint);
    //thread3->start();

    //m_audioInput->moveToThread(thread3);
    //m_audioSource->moveToThread(thread3);
    //m_device->moveToThread(thread3);

    m_audioSource->start(m_device);





    //m_audioInput.


   LOG("Testing \n");
}

void MainWindow::moveDataBuffertoCalculator(){
    //gaborTask->dataBuffer()
    //{
    //LOG("Thread starting?\n");
    //Timer timer;
    //gaborTask->dataBucket2 = Scalogram->dataBucket2;
    //thread->start();
    //}
    Scalogram->update();
}

void MainWindow::moveAudioDataToScalogram(){
    InstrumentationTimer timer("Moving Data");

    //Need to turn this into a setter for the connect function.
    Scalogram->dataBuffer(&m_device->dataPoint);
    //LOG(m_device->dataPoint) <<"\n";
    //The update function essentially is when to update the render
    //does update also update the data?
    //Scalogram->update();

    //Scalogram->dataPoint = m_device->dataPoint;
    //Scalogram->update();
}

void MainWindow::threadScalogramPlot(){
    for (int i= 0;i<100;i++) {
    //while(true){
        //Scalogram->update();
        //std::this_thread::sleep_for(std::chrono::milliseconds(20));
        //QThread::sleep(500);
        //thread->QThread::sleep(20);
        thread->msleep(500);

        LOG("testing if this thread is working do you see it\n");

    }
}

void MainWindow::threadDataRecord(){
    //while (1) {
    //connect(m_device, &audioData::newDataPoint, this, &MainWindow::moveAudioDataToScalogram);
    //    LOG("threadCounter") << 1 <<"\n";
    //}
    //Scalogram->image = gaborTask->image;
    //Scalogram->update();
    thread->exit();

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
    InstrumentationTimer timer("create Action");
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

    const QIcon startThread = QIcon::fromTheme("document-save", QIcon(":/images/QEE.png"));
    QAction *startThreadAct = new QAction(startThread, tr("&StartThread"), this);
    //saveAct->setShortcuts(QKeySequence::Save);
    //saveAct->setStatusTip(tr("Save the document to disk"));
    connect(startThreadAct, &QAction::triggered, this,&MainWindow::startThreadTest );
    fileMenu->addAction(startThreadAct);
    fileToolBar->addAction(startThreadAct);

    const QIcon stopThread = QIcon::fromTheme("document-save", QIcon(":/images/QEE.png"));
    QAction *stopThreadAct = new QAction(stopThread, tr("&StartThread"), this);
    //saveAct->setShortcuts(QKeySequence::Save);
    //saveAct->setStatusTip(tr("Save the document to disk"));
    connect(stopThreadAct, &QAction::triggered, this,&MainWindow::stopThreadTest );
    fileMenu->addAction(stopThreadAct);
    fileToolBar->addAction(stopThreadAct);

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

    numGaborSlider = new QSlider;
    numGaborSlider->setRange(1,1024);
    numGaborSlider->setOrientation(Qt::Horizontal);
    numGaborSlider->setValue((int)(Scalogram->GaborScale));
    fileToolBar->addWidget(numGaborSlider);
    connect(numGaborSlider,QOverload<int>::of(&QSlider::valueChanged),this, &MainWindow::changeTFResolution );


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
    InstrumentationTimer timer("create actions 2");
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


    numGaborSliderScaling1 = new QSlider;
    numGaborSliderScalingLabel1 = new QLabel(tr("Gabor Error"));
    numGaborSliderScalingLabel1->setBuddy(numGaborSliderScaling1);
    numGaborSliderScalingValue1 = new QLabel;
    //numGaborSliderScalingLabel1->setBuddy(numGaborSliderScaling1);

    numGaborSliderScaling1->setRange(1,1024);
    numGaborSliderScaling1->setOrientation(Qt::Horizontal);
    plotToolBar->addWidget(numGaborSliderScalingLabel1);
    plotToolBar->addWidget(numGaborSliderScaling1);
    plotToolBar->addWidget(numGaborSliderScalingValue1);
    connect(numGaborSliderScaling1,QOverload<int>::of(&QSlider::valueChanged),this, &MainWindow::changeTFRError );
    connect(numGaborSliderScaling1,QOverload<int>::of(&QSlider::valueChanged),this, &MainWindow::gaborSliderValueChanged1 );

    numGaborSliderScaling2 = new QSlider;
    numGaborSliderScalingLabel2 = new QLabel(tr("Gabor Amplitude"));
    numGaborSliderScalingLabel2->setBuddy(numGaborSliderScaling2);
    numGaborSliderScalingValue2 = new QLabel;
    numGaborSliderScaling2->setRange(1,1024);
    numGaborSliderScaling2->setOrientation(Qt::Horizontal);
    plotToolBar->addWidget(numGaborSliderScalingLabel2);
    plotToolBar->addWidget(numGaborSliderScaling2);
    plotToolBar->addWidget(numGaborSliderScalingValue2);
    connect(numGaborSliderScaling2,QOverload<int>::of(&QSlider::valueChanged),this, &MainWindow::changeTFAmplitude );
    connect(numGaborSliderScaling2,QOverload<int>::of(&QSlider::valueChanged),this, &MainWindow::gaborSliderValueChanged2 );

    numGaborSliderScaling3 = new QSlider;
    numGaborSliderScalingLabel3 = new QLabel(tr("Gabor Threshold"));
    numGaborSliderScalingLabel3->setBuddy(numGaborSliderScaling3);
    numGaborSliderScalingValue3 = new QLabel;
    numGaborSliderScaling3->setRange(1,1024);
    numGaborSliderScaling3->setOrientation(Qt::Horizontal);
    plotToolBar->addWidget(numGaborSliderScalingLabel3);
    plotToolBar->addWidget(numGaborSliderScaling3);
    plotToolBar->addWidget(numGaborSliderScalingValue3);
    connect(numGaborSliderScaling3,QOverload<int>::of(&QSlider::valueChanged),this, &MainWindow::changeTFValue );
    connect(numGaborSliderScaling3,QOverload<int>::of(&QSlider::valueChanged),this, &MainWindow::gaborSliderValueChanged3 );

    numGaborSliderScaling4 = new QSlider;
    numGaborSliderScalingLabel4 = new QLabel(tr("Gabor delta"));
    numGaborSliderScalingLabel4->setBuddy(numGaborSliderScaling4);
    numGaborSliderScalingValue4 = new QLabel;

    numGaborSliderScaling4->setRange(1,1024);
    numGaborSliderScaling4->setOrientation(Qt::Horizontal);
    plotToolBar->addWidget(numGaborSliderScalingLabel4);
    plotToolBar->addWidget(numGaborSliderScaling4);
    plotToolBar->addWidget(numGaborSliderScalingValue4);
    connect(numGaborSliderScaling4,QOverload<int>::of(&QSlider::valueChanged),this, &MainWindow::changeTFValue2 );
    connect(numGaborSliderScaling4,QOverload<int>::of(&QSlider::valueChanged),this, &MainWindow::gaborSliderValueChanged4 );


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
//!
void MainWindow::changeTFResolution(){
     Scalogram->GaborScale = 256.0f/1024.0f * (float)(numGaborSlider->value());
}

void MainWindow::changeTFRError(){
     Scalogram->Error = 256.0f/1024.0f * (float)(numGaborSliderScaling1->value());
}

void MainWindow::changeTFAmplitude(){
     Scalogram->Amplitude = 50.0f/1024.0f * (float)(numGaborSliderScaling2->value());
}

void MainWindow::changeTFValue(){
     Scalogram->threshold = 0.5f/1024.0f * (float)(numGaborSliderScaling3->value());
}

void MainWindow::changeTFValue2(){
     Scalogram->TFValue = 100.0f/1024.0f * (float)(numGaborSliderScaling4->value());
}

void MainWindow::gaborSliderValueChanged1(int Error){
    numGaborSliderScalingValue1->setText(QString::number(Error));
}

void MainWindow::gaborSliderValueChanged2(int Amplitude){
    numGaborSliderScalingValue2->setText(QString::number(Amplitude));
}

void MainWindow::gaborSliderValueChanged3(int Threshold){
    numGaborSliderScalingValue3->setText(QString::number(Threshold));
}

void MainWindow::gaborSliderValueChanged4(int TFValue){
    numGaborSliderScalingValue4->setText(QString::number(TFValue));
}

//! [32]
void MainWindow::createStatusBar()
//! [32] //! [33]
{
    InstrumentationTimer timer("Status Bar");
    statusBar()->showMessage(tr("Ready"));
}






