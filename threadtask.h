#ifndef THREADTASK_H
#define THREADTASK_H

#include "qcolor.h"
#include <QObject>

#include <QThread>
#include <iostream>
#include <vector>
#define BucketSize  1024
#define NumFreq  512
#define GaborWidth  32

class threadTask : public QObject
{
    Q_OBJECT
public:
    explicit threadTask(QObject *parent = nullptr);
    void DoSetup(QThread &cThread);
    //threading functions
    void gaboorThreadSetup(QThread &cThread);
    void imageThreadSetup(QThread &cThread);
    static void simpleColorMap(float* value, int* r, int* g, int* b);

//public variables
public:
    //void evalColormap(float value, int &r, int &g, int &b);
    int threadCounter =0;

    int size = 1060;
    int MaxSize = 1024;
    int counter = 0;
    float test_value = 0.0;
    int N = BucketSize;// Size of the data bucket
    int L = NumFreq;// Number of frequncie to decompose into
    int M = 256;// Width of the window
    //quick change values to find ridges
    int G = 26;
    float Error = 0.5;
    float dataPoint = 0;

    int interp_value = 0;
    int r = 0, g =0, b =0;
    int ivalue = 0;

    float F[BucketSize][2];

    float W_C[BucketSize][NumFreq];//[width of dataset][number of frequencies]?
    float W_S[BucketSize][NumFreq];//[width of dataset][number of frequencies]?
    float W_X[BucketSize];//size of the data set
    float W_G[GaborWidth];//width of the window

    //Use the MagSf as full image.
    //Load Real Time values into it
    float W_MagSf[BucketSize][NumFreq];//[width of window][number of frequencies]
    float W_RTMagSf[NumFreq];//[number of frequencies]
    float W_AngSf[BucketSize][NumFreq];//[width of window][number of frequencies]
    float W_RTAngSf[NumFreq];//[number of frequencies]
    float W_Mfft[NumFreq];//Number of frequencies
    float W_d = 0;
    void dataBuffer(float* dataPoint);
    float GaborScale = 64.0;

    QImage *image;
    std::vector<float> dataBucket2;

    //for the image
//private variables
private:
    QColor backgroundColor;
    unsigned int plotx, ploty, plotwidth, plotheight;


    float dataBucket[256];


    int bufferCount = 0;



    unsigned int paddingX;
    unsigned int paddingY;
    unsigned int ylabelSpacing;
    unsigned int xlabelSpacing;

    unsigned int colorBarWidth;
    unsigned int spectrumWidth;
    unsigned int waveformWidth;

    float min_value;
    float max_value;

    QVector<QVector<float> > colormap;

signals:
    void imageComplete(QImage* image);
    void finished();

public slots:
    void DoWork();
    void gaborCalcWork();
    void imageWork();
    void quit();
};

#endif // THREADTASK_H
