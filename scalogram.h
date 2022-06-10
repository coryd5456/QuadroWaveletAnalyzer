#ifndef SCALOGRAM_H
#define SCALOGRAM_H

#include <QWidget>
//class QImage;

#define BucketSize  1024
#define NumFreq  512
#define GaborWidth  32

class scalogram : public QWidget
{
public:
    scalogram(QWidget *parent = nullptr);
    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;
    void evalColormap(float value, int &r, int &g, int &b);
    void simpleColorMap(float value, int &r, int &g, int &b);
    int size = 50;
    int MaxSize = 1024;
    int counter = 0;
    float dataPoint = 0;

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


protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
//private variables
private:
    QColor backgroundColor;
    unsigned int plotx, ploty, plotwidth, plotheight;
    QImage *image;
    void dataBuffer(float dataPoint);
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
