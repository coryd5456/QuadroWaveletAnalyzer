#include "scalogram.h"
#include <QStylePainter>
#include <iostream>
#include <cmath>
#include <future>
#include <QThread>
#include <QTimer>
//#include <algorithm>
//#include <execution>
#include <glm/common.hpp> // abs
#include <glm/ext/scalar_constants.hpp> // epsilon
#include "Instrumentor.h"
#include "qdebug.h"
#include "waveletMath.h"
#include "timer.h"
#include <chrono>
#define PI 3.14159265f
#define LOG(x) std::cout << x
#define Bucket_length 512

//I really need to get multithreading working

static std::mutex mu;

//////////////////////////////////
/// \brief scalogram::scalogram
/// \param parent
/// To Do
///
///  1. Making the window auto resizable using resize event
///     -Half done: Need to scale the data too?
///     -Maybe set a minimum size
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
///  New tasks:
///     1. (done)I need to learn sorting algorithms to find the maximums
///     2. I need to optimize the gabor transform algorithm
///         a. (done) Remove close to 0 values
///         b. pass by reference
///         c. Set up a sequence of pre-calculated bell curve scales?
///             - maybe set this up as a 256/128-length array that can be scaled.
///             - set a min max scalin value
///     3. (done)Need to learn how to get real time audio data.
///     4. code runs, need to set up test data
///
/// Maybe I could add time events to this set up to get this done.

scalogram::scalogram(QWidget *parent):  QOpenGLWidget(parent)
{
    //Note you must initialize all of your types they are just whatever they previously were in memory otherwise
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


    //reserves memory for our data bucket vector such that we have no copies from push_back onto it
    dataBucket2.reserve(Bucket_length);

    //initializes gabor transform values such that they are only generated once and not each time the
    //code is run.


    init_ComplexExp( W_C,W_S);
    init_Data(W_X);

    image = new QImage(plotwidth, plotheight, QImage::Format_RGB32);
    image->fill(backgroundColor);




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
    return QSize(size, size/2);
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

//I can use a map to pre-process my interpolated values.
//I technically only need 256 values mapped due to 8-bit rgb
//speed lost due to casting?

void scalogram::simpleColorMap(float* value, int* r, int* g, int* b){
    //I make the smallest value red = 255 and the largest value blue 255
    // to get some inbetween I could make it a scaling from 0-255
    // red is 255 - interplated_value
    // blue is the interpolated value

    //int max_value = 10000;
    //int min_value = 0;
    //need to add functionality for if the value goes outside of the range.
    //int interpolated_value = 0;


    //std::lock_guard<std::mutex> lock(mu);
    if(*value > 100){
        //interpolated_value = 255;
        *r = 0;
        *b = 255;
    } else if(*value < 0){
        //interpolated_value = 0;
        *r = 255;
        *b = 0;
    } else{
        //interpolated_value = (int)((*value - min_value) * (255/(max_value - min_value))) ;
       *r = 255- (int)((*value - 0) * (255/100));
       *b = (int)((*value - 0) * (255/100));
    }


    //*r = 255 - interpolated_value;
    *g = 0; //counter%255;//for testing if the scalogram paint event is being triggered by data
    //*b = interpolated_value;


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


void scalogram::dataBuffer(float dataPoint[]){

    //add in my timer here for how long it takes to get to my sample rate.
    //include some sort of delta time.
    /*
    if(SampleCounter ==0){
        bufferTimer.StartDemand();
        SampleCounter = 1;
    } else if(SampleCounter ==1){
        bufferTimer.StopDemand();
        SampleCounter =0;
    }*/
    //LOG(SampleCounter)<<"\n";
    //SampleCounter = SampleCounter%2000;
    //SampleCounter++;
    //Code using an array
    /*
    if(bufferCount < 256){
        //fill up to the innitial buffer size
        dataBucket[bufferCount] = dataPoint;
     bufferCount++;
    }else{
        //once buffer is full do this stuff
        //shift all the data over except the newest data point needs to be loaded in
        // Thus only shift index 0-254 over
        for(int i =0; i<(256 -1); i++){
            dataBucket[i] = dataBucket[i+1];
        }
        //add the newest data point to the dataBucket
        dataBucket[255] = dataPoint;
    }*/

    //Code using vectors
    //Switched from using "push_back" to "emplace_back" to avoid copying of data
    Mu.lock();
    {
        //LOG("Buffer Timer");
        //Timer timer4;

    for(int i = 0; i<40;i++){
        if(bufferCount < Bucket_length){
            //fill up to the innitial buffer size
            dataBucket2.emplace_back(dataPoint[i]);
        bufferCount++;
        }else{
            //remove the oldest element
            dataBucket2.erase(dataBucket2.begin());
            //add the newest data point to the dataBucket
            dataBucket2.emplace_back(dataPoint[i]);
        }
    }

    }
    Mu.unlock();

    this->update();
}

void scalogram::gaboorThreadSetup(QThread &cThread){
    connect(&cThread, &QThread::started,this, &scalogram::gaborCalcWork);
}
void scalogram::imageThreadSetup(QThread &cThread){
    connect(&cThread, &QThread::started,this, &scalogram::imageWork);
}

void scalogram::gaborCalcWork(){
    /////////////I may have a mutex problem with the data bucket passing it by reference////////////
    if(dataBucket2.size() ==Bucket_length){
    {
            LOG("Gaabor Transform computation\n");
            Timer timer;

    RTGaborTransform(&dataBucket2,&GaborScale,W_C,W_S,W_RTMagSf,W_RTAngSf);
        }
    //RTGaborTransform()

    //The following code all assumes that my buffer is full.

    //Need to break up my code into multiple parts

    //first shift all of my results in my image back
    {
            LOG("shifting function\n");
            Timer timer;
    for(int m = 0; m < N; m++){
        for(int l=0; l<L;l++){
            //The shifting isn't happening?
            W_MagSf[m][l] = W_MagSf[m+1][l];//force values +0.0001
            W_AngSf[m][l] = W_AngSf[m+1][l];
            //The shifting is definitely working. It's just not grabbing the newest values.
            //It's not grabbing the newest values, because there is an off by 1 error. m < N-M-1 only goes up to N-M-2.
        }
    }
    //LOG(W_MagSf[N-1][50])<<"This is the value going to screen\n";
    //load in newest results

        for(int l = 0; l<L;l++){
            W_MagSf[N][l] = W_RTMagSf[l];
            W_AngSf[N][l] = W_RTAngSf[l];

        }

    }
    //Now, I need to draw the formant lines:
    }


    /////////////////////From here on is the image work//////////////
    image = new QImage(plotwidth, plotheight, QImage::Format_RGB32);
    image->fill(backgroundColor);

    interp_value = 0;
    int r = 0, g =0, b =0;

    //I can improve performance with parallel processing via a parallel for loop since this is very indipendent operations
    {
        LOG("Coloring the data\n");
        Timer timer;

    for (int l = 0; l < L; l++)
        {
        //printf("[ ");
        //LOG("[");
        for (int m = 1; m < N; m++)
            {
                //Finds the formants and plots the thing.
                //I can optimize this by only doing the formants on the newest thing
                //The 2PI and 1024 coorispond to normalizing my units based on the time and frequencies

                if( std::fabs((TFValue*2*PI*G*(W_AngSf[m][l] - W_AngSf[m-1][l])) - (float)(l)) <= Error && W_MagSf[m][l]>threshold){//0.14
                    //Add TFValue to gabor calc to reduce run time.
                    image->setPixel(m, L-1-l, qRgb(0,255,0));
                }else{
                    float color = Amplitude* W_MagSf[m][l];//Add amplitude scaling to calculation to reduce run-time
                    simpleColorMap(&color ,&r,&g,&b);

                    image->setPixel(m, L-1-l, qRgb(r,g,b));
                }

            }
    }

    }

    update();

}

void scalogram::imageWork(){

}

//void scalogram::imageWorkComplete(){};

void scalogram::imageRecived(QImage* cimage){
    {
        LOG("Testing the image Recieved Time\n");
    Timer timer;
    *image = *cimage;
    LOG("I sent the image over\n");
    update();
    emit imageWorkComplete();
    //this->update();
    }
}

void scalogram::paintEvent(QPaintEvent *) {
    {
    InstrumentationTimer timer("Paint Event");

    QPainter painter(this);
    //counter++;
    //LOG("paint event: ");
    //LOG(counter);
    //LOG("\n");
    //Look into removing the amount of pre-defining and copying
    //painter.setRenderHint(QPainter::Antialiasing);
    image = new QImage(plotwidth, plotheight, QImage::Format_RGB32);
    image->fill(backgroundColor);
    /*
    int interp_value = 0;
    int r = 0, g =0, b =0;
    int ivalue = qRgb(12,255, 255);
    */

    ///interp_value = 0;
    ///int r = 0, g =0, b =0;
    //ivalue = qRgb(12,255, 255);
/*
    //Need to make a mirror mapping for the drawing of the image.
    for(int i = 0; i<plotwidth;i++
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


    //I need to construct a real time version of this code.
    //To do that I need
    // 1. a buffer bucket
    // 2. A way to shift the data over with each new data point
    // 3. need to redraw all the data every time I get a new data point
    // 4. I probably need a deltaTime to set this up
    // 5. need to learn templates

    /////////
    /// \brief N
    /// Need to write a function to handle the buffer vector.
    ///
    /// Need to write a data steam function to test this out.
    /*
    int N = BucketSize;// Size of the data bucket
    int L = NumFreq;// Number of frequncie to decompose into
    int M = 256;// Width of the window

    init_ComplexExp( W_C,W_S);
    //for(int n = 0; n < N ; n++){
    //    LOG("n: ") << 3 << "  W_S: " << W_S[3][4] << "\n";
    //}
    init_Data(W_X);
    */
    //Need to pass a data point and a buffer vector

    //dataBuffer(&dataPoint);
    /*
    LOG("\n\n");
    LOG("start of bucket \n");
    LOG(dataBucket2.size())<<"\n";

    //Testing if data is being passed correctly

    for(float v: dataBucket2){
        LOG(v)<<"This is a data point\n";
    }*/

    /////////////
    /// \brief RTGaborTransform
    ///This section of code is to impliment a real time gabor transform.
    ///
    ///
    ///

    if(dataBucket2.size() ==Bucket_length){
    {
            InstrumentationTimer timer2("Gabor Transform computation");
            LOG("Gaabor Transform computation\n");
            Timer timer;
    GMu.lock();
    RTGaborTransform(&dataBucket2,&GaborScale,W_C,W_S,W_RTMagSf,W_RTAngSf);
    GMu.unlock();
        }
    //RTGaborTransform()

    //The following code all assumes that my buffer is full.

    //Need to break up my code into multiple parts

    //first shift all of my results in my image back
    {
            //InstrumentationTimer timer3("Shifting Function");
            LOG("shifting function\n");
            Timer timer;
    for(int m = 0; m < N; m++){
        for(int l=0; l<L;l++){
            //The shifting isn't happening?
            W_MagSf[m][l] = W_MagSf[m+1][l];//force values +0.0001
            W_AngSf[m][l] = W_AngSf[m+1][l];
            //The shifting is definitely working. It's just not grabbing the newest values.
            //It's not grabbing the newest values, because there is an off by 1 error. m < N-M-1 only goes up to N-M-2.
        }
    }
    //LOG(W_MagSf[N-1][50])<<"This is the value going to screen\n";
    //load in newest results

        for(int l = 0; l<L;l++){
            W_MagSf[N][l] = W_RTMagSf[l];
            W_AngSf[N][l] = W_RTAngSf[l];

        }

    }
    //Now, I need to draw the formant lines:
    }

    //quick change values to find ridges
    //moved to header to not define on every run.
    //int G = 26;
    //int Error = 1;
    //go through the m's first, then the l's

    //I don't need to recolor all of the data, just the latest row?

    //I can improve performance with parallel processing via a parallel for loop since this is very indipendent operations


    {
        //InstrumentationTimer timer4("Coloring the data");
        LOG("Coloring the data\n");
        Timer timer;

        /*

        std::for_each(std::execution::par , std::begin(W_MagSf), std::end(W_MagSf), [&](int m){
            std::lock_guard<std::mutex> guard(mu);
            for(int l =0; l <L;l++){
                simpleColorMap( &W_MagSf[m][l] ,&r,&g,&b);
                image->setPixel(m, L-1-l, qRgb(r,g,b));
            }
        });*/

    //I may be able to speed this up by storing the pixel data for the color map

    for (int l = 0; l < L; l++)
        {
        //printf("[ ");
        //LOG("[");
        for (int m = 1; m < N; m++)
            {

                //Finds the formants and plots the thing.
                //I can optimize this by only doing the formants on the newest thing
                //The 2PI and 1024 coorispond to normalizing my units based on the time and frequencies
                //if(m >0 &&  (2*PI*1024*(W_AngSf[m][l] - W_AngSf[m-1][l])) - l <20 && (2*PI*1024*(W_AngSf[m][l] - W_AngSf[m-1][l])) - l > -20 && W_MagSf[m][l]>0.1){//0.14
                //2*PI*G*(W_AngSf[m][l] - W_AngSf[m-1][l])) is returning 0 for some reason.
                //ridge detection isn't working.
                //Next thing on my list is to start making sliders anyway.
                // Oh, I haven't added the angular part to this yet oops.
                /*if(m ==200 && (2*PI*1024*(W_AngSf[m][l] - W_AngSf[m-1][l])) - l <20 && (2*PI*1024*(W_AngSf[m][l] - W_AngSf[m-1][l])) - l > -20 && W_MagSf[m][l]>0.1){
                    LOG("Values on the ridge:");
                    LOG(W_MagSf[m][l])<< " at frequency: "<< l << "\n";

                }*/

               /* if (m==600){
                    LOG(2*PI*G*(W_AngSf[m][l] - W_AngSf[m-1][l]))<<"values at "<< l <<"\n";
                }*/
                //removed m>0 requirement to just take m=0 column out of the loop
                //I can do &m and &l and dereference them WTF lol
            if( std::fabs((25.6f*2.0f*PI*(W_AngSf[m][l] - W_AngSf[m-1][l])) - (float)(l)) <= Error && W_MagSf[m][l]>threshold){//0.14

                image->setPixel(m, L-1-l, qRgb(0,255,0));
            }else{
                float color = Amplitude*10.0* W_MagSf[m][l];
                simpleColorMap(&color ,&r,&g,&b);

                image->setPixel(m, L-1-l, qRgb(r,g,b));
            }
            //test_value = W_MagSf[m][l];
            //m_Futures.push_back(std::async(simpleColorMap , &W_MagSf[m][l] ,&r,&g,&b));
            //m_Futures.push_back(std::async(image->setPixel , m ,L-1-l,qRgb(r,g,b)));

            //Testing if I have values for W_MagSf

            //simpleColorMap(max_value ,r,g,b);

            //image->setPixel(m, L-1-l, qRgb(r,g,b));

            }
    }

    }

    //DFT();
    //////////////
    /// This section of code is for a static Gabor transform on static data
    /// This new section of code is going to be for real time gabor transform. SS
    /*
    GaborTransform(W_X,W_C,W_S,W_MagSf,W_AngSf);
    bool flip = false;
    int count = 0;
    LOG("count testing: \n");
    for (int l = 0; l < L; ++l)
        {
        //printf("[ ");
        //LOG("[");
            for (int m = 0; m < N-M; ++m)
            {
                //Need to add in some math to find the largest 10 maximum values and their frequencies.

                if(m >20 &&  (2*PI*1024*(W_AngSf[m][l] - W_AngSf[m-1][l])) - l <20 && (2*PI*1024*(W_AngSf[m][l] - W_AngSf[m-1][l])) - l > -20 && W_MagSf[m][l]>0.1){//0.14
                    //simpleColorMap(3*(max_value) * W_MagSf[m][l] ,r,g,b);
                    //check these values

                    F[count][flip] = (float)(l);
                    if(flip){
                        count++;

                    }
                    LOG(count) << "\n";
                    flip = !flip;

                    //LOG(flip) << "\n";

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

        }

    for(int i = count; i<1024;i++){
        F[i][0] = 0.0;
        F[i][1] = 0.0;
    }
    */




    //init_GaborWindow();
    //init_Data();
    /*
    for(int n = 0; n < N ; n++){
        LOG("n: ") << n << "  W_S: " << W_S[n][4] << "\n";
    }*/
    //LOG(W_X[12])<<"This vaue is good!\n";
    //LOG( -1.0*sin(2.0));


    //qInfo()<<QThread::currentThread()<<" DRAWING THE IMAGE \n";
    {
        LOG("Drawing the data\n");
        Timer timer;
    painter.drawImage(plotx, ploty, *image);
    }
    LOG("\n");

}
}
