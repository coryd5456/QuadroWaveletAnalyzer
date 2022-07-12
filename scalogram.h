#ifndef SCALOGRAM_H
#define SCALOGRAM_H

#include "timer.h"
#include <QOpenGLWidget>
#include <QWidget>
#include <future>
#include <algorithm>
#include <QThread>
#include <QMutex>

//class QImage;
class parallel_policy;
#define BucketSize  1024
#define NumFreq  512
#define GaborWidth  32
//adding OpenGL to the project
//class scalogram : public QWidget
class scalogram : public QOpenGLWidget
{
    Q_OBJECT
public:
    scalogram(QWidget *parent = nullptr);
    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

    //threading functions
    void gaboorThreadSetup(QThread &cThread);
    void imageThreadSetup(QThread &cThread);

signals:
    void imageWorkComplete();
public slots:
    void gaborCalcWork();
    void imageWork();
    void imageRecived(QImage* cimage);
    //void setNewDataPoint(float*);



//public variables
public:
    QMutex Mu;
    QMutex GMu;
    Timer bufferTimer;
    int SampleCounter =0;
    void evalColormap(float value, int &r, int &g, int &b);
    static void simpleColorMap(float* value, int* r, int* g, int* b);
    int size = 1060;
    int MaxSize = 1024;
    int counter = 0;
    float test_value = 0.0;
    int N = BucketSize;// Size of the data bucket
    int L = NumFreq;// Number of frequncie to decompose into
    int M = 256;// Width of the window
    //quick change values to find ridges
    int G = 26;
    float threshold = 0.14f;
    float Amplitude = 18.0f;
    float Error = 20.0f;
    float TFValue = 0.1f;
    float dataPoint = 0.0f;

    int interp_value = 0;
    int r = 0, g =0, b =0;
    int ivalue = 0;

    QImage *image;

    std::vector<std::future<void>> m_Futures;

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
    void dataBuffer(float dataPoint[]);
    float GaborScale = 64.0;
    std::vector<float> dataBucket2;


protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
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

};

#endif // SCALOGRAM_H
