#ifndef SCALOGRAM_H
#define SCALOGRAM_H

#include <QWidget>
//class QImage;

class scalogram : public QWidget
{
public:
    scalogram(QWidget *parent = nullptr);
    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;
    void evalColormap(float value, int &r, int &g, int &b);
    void simpleColorMap(float value, int &r, int &g, int &b);
    int size = 50;
    int MaxSize = 500;
    int counter = 0;
protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
//private variables
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

    QVector<QVector<float> > colormap;

};

#endif // SCALOGRAM_H
