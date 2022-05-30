#include "scalogram.h"
#include <QStylePainter>
#include <iostream>
#include <cmath>
#include "waveletMath.h"
#define LOG(x) std::cout << x

//////////////////////////////////
/// \brief scalogram::scalogram
/// \param parent
/// To Do
///
///  1. Making the sindow auto resizable using resize event
///  2. getting the value from where the curser is at.
///  3. QAudioDevice Library
///  4. QSierial Library
///  5. Storing data in a .cam file
///     -loading data from .cam file
///     - scrolling through data in .cam file
///  6. Make pop up window of the real time formant chart
///     references: https://en.wikipedia.org/wiki/Formant#/media/File:Average_vowel_formants_F1_F2.png
///                 https://www.ipachart.com/
///  7. Need to make heat map for the formant chart
///  8. In Main window add a button to make this the central widget
///

scalogram::scalogram(QWidget *parent): QWidget(parent)
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




    LOG("Spectragram event");
    //update();
    //defines the color mapping for the spectragram
    QVector<float> color0, color1, color2, color3, color4;
    color0.push_back(0.0);  color0.push_back(0.0);  color0.push_back(32.0);color0.push_back(0.0);
    color1.push_back(0.0);color1.push_back(0.0);  color1.push_back(255.0);color1.push_back(0.25);
    color2.push_back(0.0);color2.push_back(255.0);color2.push_back(255.0);color2.push_back(0.5);
    color3.push_back(255.0);color3.push_back(255.0);color3.push_back(0.0);color3.push_back(0.75);
    color4.push_back(255.0);color4.push_back(0.0);color4.push_back(0.0);color4.push_back(1.0000);
    colormap.push_back(color0);
    colormap.push_back(color1);
    colormap.push_back(color2);
    colormap.push_back(color3);
    colormap.push_back(color4);

    setBackgroundRole(QPalette::Base);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

QSize scalogram::minimumSizeHint() const
{
    return QSize(size, size);
}

QSize scalogram::sizeHint() const
{
    return QSize(MaxSize, MaxSize/2);
}

//Need to fix this such that it can resize for anyones monitor
void scalogram::resizeEvent(QResizeEvent *event) {
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
//changes the pixel value to a linear interpolation from red high to blue low.
void scalogram::simpleColorMap(float value, int &r, int &g, int &b){
    //I make the smallest value red = 255 and the largest value blue 255
    // to get some inbetween I could make it a scaling from 0-255
    // red is 255 - interplated_value
    // blue is the interpolated value

    //need to add functionality for if the value goes outside of the range.
    int interpolated_value = 0;
    if(value > max_value){
        interpolated_value = 255;
    } else if(value < min_value){
        interpolated_value = 0;
    } else{
        interpolated_value = (int)((value - min_value) * (255/(max_value - min_value))) ;
    }


    r = 255 - interpolated_value;
    g = 0;
    b = interpolated_value;
}

void scalogram::evalColormap(float value, int &r, int &g, int &b) {
    int nRGB = colormap.size();

    QVector<float> RGB;
    QVector<float> RGBnext;

    for (int indRGB = 0; indRGB < nRGB-1; indRGB++) {
        RGB = colormap[indRGB];
        RGBnext = colormap[indRGB+1];

        if (value < RGB[3]) {
            r = (int)RGB[0];
            g = (int)RGB[1];
            b = (int)RGB[2];
            return;
        } else if (value <= RGBnext[3]) {
            float valcoeff = (value - RGB[3]) / (RGBnext[3] - RGB[3]);

            r = (int)(RGB[0] * (1.0 - valcoeff) + valcoeff * RGBnext[0]);
            g = (int)(RGB[1] * (1.0 - valcoeff) + valcoeff * RGBnext[1]);
            b = (int)(RGB[2] * (1.0 - valcoeff) + valcoeff * RGBnext[2]);
            return;
        }
    }
    r = (int)RGBnext[0];
    g = (int)RGBnext[1];
    b = (int)RGBnext[2];
}
void scalogram::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    counter++;
    LOG("paint event: ");
    LOG(counter);
    LOG("\n");
    image = new QImage(plotwidth, plotheight, QImage::Format_RGB32);
    image->fill(backgroundColor);
    int interp_value = 0;
    int r = 0, g =0, b =0;
    int ivalue = qRgb(12,255, 255);
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

    int N = BucketSize;// Size of the data bucket
    int L = NumFreq;// Number of frequncie to decompose into
    int M = GaborWidth;// Width of the window

    init_ComplexExp(W_C,W_S);
    //for(int n = 0; n < N ; n++){
    //    LOG("n: ") << 3 << "  W_S: " << W_S[3][4] << "\n";
    //}
    init_Data(W_X);

    //DFT();
    GaborTransform(W_X,W_C,W_S,W_MagSf);

    for (int l = 0; l < L; ++l)
        {
        //printf("[ ");
        LOG("[");
            for (int m = 0; m < N-M; ++m)
            {
                //Need to add in some math to find the largest 10 maximum values and their frequencies.
                if(W_MagSf[m][l] >= 0.14){
                    //simpleColorMap(3*(max_value) * W_MagSf[m][l] ,r,g,b);
                    int ivalue = qRgb(0,255,0);
                    image->setPixel(m, L-1-l, ivalue);
                }else{
                    simpleColorMap(3*(max_value) * W_MagSf[m][l] ,r,g,b);
                    int ivalue = qRgb(r,g,b);
                    image->setPixel(m, L-1-l, ivalue);
                }
                LOG(W_MagSf[m][l])<< " ";

            }
        //printf(" ]\n");
        LOG("]\n");

        }
    //init_GaborWindow();
    //init_Data();
    /*
    for(int n = 0; n < N ; n++){
        LOG("n: ") << n << "  W_S: " << W_S[n][4] << "\n";
    }*/
    LOG(W_X[12])<<"This vaue is good!\n";
    LOG( -1.0*sin(2.0));


    painter.drawImage(plotx, ploty, *image);

}
