#ifndef SCALOGRAM_H
#define SCALOGRAM_H

#include <QOpenGLWidget>
#include <QWidget>
#include <future>
#include <algorithm>

//class QImage;
class parallel_policy;
#define BucketSize  1024
#define NumFreq  512
#define GaborWidth  32
//adding OpenGL to the project
//class scalogram : public QWidget
class scalogram : public QOpenGLWidget
{
public:
    scalogram(QWidget *parent = nullptr);
    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;
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
    float Error = 0.5;
    float dataPoint = 0;

    int interp_value = 0;
    int r = 0, g =0, b =0;
    int ivalue = 0;

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

    float GaborScale = 64.0;


protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
//private variables
private:
    QColor backgroundColor;
    unsigned int plotx, ploty, plotwidth, plotheight;
    QImage *image;
    void dataBuffer(float* dataPoint);
    float dataBucket[256];
    std::vector<float> dataBucket2;

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
