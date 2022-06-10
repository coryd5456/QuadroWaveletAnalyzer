#ifndef FORMANTPLOT_H
#define FORMANTPLOT_H

#include <QWidget>

class formantPlot : public QWidget
{
public:
    formantPlot( float formantFrequencies[][2] , QWidget *parent = nullptr);
    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;
    int size = 50;
    int MaxSize = 1024;
    int counter = 0;

    int F[1024][2];
    int fFreqLength;


protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    QColor backgroundColor;
    unsigned int plotx, ploty, plotwidth, plotheight;
    QImage *image;

    unsigned int paddingX;
    unsigned int paddingY;
    unsigned int ylabelSpacing;
    unsigned int xlabelSpacing;

    unsigned int colorBarWidth;
    unsigned int spectrumWidth;
    unsigned int waveformWidth;

    float min_value;
    float max_value;
};

#endif // FORMANTPLOT_H
