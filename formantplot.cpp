#include "formantplot.h"
#include <QStylePainter>
#include <iostream>
#include <cmath>
#include "waveletMath.h"
#define PI 3.14159265
#define LOG(x) std::cout << x


////////////////////////////////////////////
/// \brief formantPlot::formantPlot
/// The goal of this section of code is to display a formant plot or chart.
/// This will compare where the first and second formant appear in frequency
/// relative to eachother.

formantPlot::formantPlot( float formantFrequencies[][2] , QWidget *parent): QWidget(parent)
{
    paddingX = 5;
    paddingY = 5;
    xlabelSpacing = 5;
    ylabelSpacing = 5;
    plotwidth = 1920;
    plotheight = 1080;
    plotx = 0;
    ploty = 0;
    backgroundColor = QColor(0, 0, 0);
    image = 0;

    max_value = 10000;
    min_value = 0;


    waveformWidth = 100;
    spectrumWidth = 100;
    colorBarWidth = 50;

    //fFreqLength = sizeof(formantFrequencies[0])/sizeof(formantFrequencies[0][0]);
    /*
    for(int i = 0; i< 1024 ;i++){
        F[i][0] = (int)formantFrequencies[i][0];
        F[i][1] = (int)formantFrequencies[i][1];
    }*/

    setBackgroundRole(QPalette::Base);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

}


QSize formantPlot::minimumSizeHint() const
{
    return QSize(size, size);
}

QSize formantPlot::sizeHint() const
{
    return QSize(MaxSize, MaxSize/2);
}

//Need to fix this such that it can resize for anyones monitor
void formantPlot::resizeEvent(QResizeEvent *event) {
    //padding *2 because of both sides of the window
    plotwidth  = width()  - paddingX * 2 - ylabelSpacing;
    plotheight = height() - paddingY * 2 - xlabelSpacing;
    plotx = paddingX + ylabelSpacing;
    ploty = paddingY;

    //Use these to move where it is based on the other parts of the window
    //plotwidth -= spectrumWidth;
    //plotwidth -= colorBarWidth;
    //plotheight -= waveformWidth;
    //ploty += waveformWidth;

    Q_UNUSED(event);
}

void formantPlot::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    counter++;
    LOG("paint event: ");
    LOG(counter);
    LOG("\n");
    image = new QImage(plotwidth, plotheight, QImage::Format_RGB32);
    image->fill(backgroundColor);
    //initializes values for this drawing.
    int interp_value = 0;
    int r = 0, g =0, b =0;
    int ivalue = qRgb(12,255, 255);

    //I need to draw (0,0) in the top right, and positive axies out and down from there.
    for(int i = 0; i<1024;i++){
            LOG(F[i][0])<<" i: " << i << "\n";
            image->setPixel(plotwidth - F[i][0], F[i][1], qRgb(20,255,255));
    }


/*
    //Need to make a mirror mapping for the drawing of the image.
    for(int i = 0; i<plotwidth;i++){
        for (int j = 0;j<plotheight ;j++ ) {
            //interp_value += 1000 ;
            //evalColormap(interp_value, r, g, b);
            //int ivalue = qRgb(i*j*255*255/(plotwidth*plotheight),i*255/plotwidth, j*255/plotheight);

            //Simple color map works. I now need to test out a transformed signal.

            //takes in a function and maps it to a coloring from red to blue
            simpleColorMap(max_value * cos(i*j) ,r,g,b);
            int ivalue = qRgb(r,g,b);
            image->setPixel(i, j, ivalue);
        }
    }
    */
    //Drawing a cosine wave test
    //image->setPixel(2, 2, 200);

    /*for (int i = 0; i<(plotwidth) ; i++ ) {
        simpleColorMap(0 ,r,g,b);
        int ivalue = qRgb(0,0,0);
        //(int)(((10)*sin(2*i))+40)
        for (int s = 0; s < 20 ; s++ ) {
            //set pixel doesn't seem to literally be 1 pixel
            // seems like I need to learn some aliasing properties due to quantization
            image->setPixel(i, (int)((50)*sin((44/7)*(i))+(150+s)) , ivalue);
        }

    }*/

    /*

    for (int l = 0; l < L; ++l)
        {
        //printf("[ ");
        //LOG("[");
            for (int m = 0; m < N-M; ++m)
            {
                //Need to add in some math to find the largest 10 maximum values and their frequencies.

                if(m >5 &&  (2*PI*1024*(W_AngSf[m][l] - W_AngSf[m-1][l])) - l <20 && (2*PI*1024*(W_AngSf[m][l] - W_AngSf[m-1][l])) - l > -20 && W_MagSf[m][l]>0.1){//0.14
                    //simpleColorMap(3*(max_value) * W_MagSf[m][l] ,r,g,b);
                    int ivalue = qRgb(0,255,0);
                    image->setPixel(m, L-1-l, ivalue);
                }else{
                    simpleColorMap(3*(max_value) * W_MagSf[m][l] ,r,g,b);
                    //simpleColorMap(max_value ,r,g,b);
                    int ivalue = qRgb(r,g,b);
                    image->setPixel(m, L-1-l, ivalue);
                }

                if(m ==200 && (2*PI*1024*(W_AngSf[m][l] - W_AngSf[m-1][l])) - l <20 && (2*PI*1024*(W_AngSf[m][l] - W_AngSf[m-1][l])) - l > -20 && W_MagSf[m][l]>0.1){
                    LOG("Values on the ridge:");
                    LOG(W_MagSf[m][l])<< " at frequency: "<< l << "\n";
                }

            }
        //printf(" ]\n");
        //LOG("]\n");

        }*/
    /*
    for(int n = 0; n < N ; n++){
        LOG("n: ") << n << "  W_S: " << W_S[n][4] << "\n";
    }*/
    //LOG(W_X[12])<<"This vaue is good!\n";
    //LOG( -1.0*sin(2.0));


    painter.drawImage(plotx, ploty, *image);

}
