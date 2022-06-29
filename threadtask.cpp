#include "threadtask.h"
#include "qdebug.h"
#include "waveletMath.h"
#include "timer.h"
#include <QImage>
#include <QMutex>

#define PI 3.14159265
#define LOG(x) std::cout << x

///
/// \brief threadTask::threadTask
/// \param parent
/// 1. How do I send databucket to this thread
/// 2. I need to send an image from here to the paint event in scalogram.

threadTask::threadTask(QObject *parent)
    : QObject(parent)
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
    dataBucket2.reserve(256);

    //initializes gabor transform values such that they are only generated once and not each time the
    //code is run.


    init_ComplexExp( W_C,W_S);
    init_Data(W_X);
}
//Starts this object on a different thread such that we can sub class it
void threadTask::DoSetup(QThread &cThread){
    connect(&cThread, &QThread::started, this , &threadTask::DoWork);

}

void threadTask::DoWork(){
    for(int i = 0; i<1000; i++){
        LOG(i)<<"\n";
    }
}

void threadTask::gaboorThreadSetup(QThread &cThread){
    connect(&cThread, &QThread::started,this, &threadTask::gaborCalcWork);
}
void threadTask::imageThreadSetup(QThread &cThread){
    connect(&cThread, &QThread::started,this, &threadTask::imageWork);
}

void threadTask::gaborCalcWork(){
    /////////////I may have a mutex problem with the data bucket passing it by reference////////////
    ///
    //qInfo()<<QThread::currentThread()<<" This is the thread gabor is runing on \n";
    if(dataBucket2.size() ==256){
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

                if( std::fabs((2*PI*G*(W_AngSf[m][l] - W_AngSf[m-1][l])) - (float)(l)) <= Error && W_MagSf[m][l]>0.1){//0.14

                    image->setPixel(m, L-1-l, qRgb(0,255,0));
                }else{
                    float color = 100.0* W_MagSf[m][l];
                    simpleColorMap(&color ,&r,&g,&b);

                    image->setPixel(m, L-1-l, qRgb(r,g,b));
                }

            }
    }

    }
    //Need to emit a signal to update the scalogram
    //update();
    LOG("Current Gabor run finishes")<<threadCounter<<"\n";
    QMutex mutex;
    mutex.lock();
    emit threadTask::imageComplete(image);
    mutex.unlock();
}

void threadTask::imageWork(){

}

void threadTask::quit()
{
    emit threadTask::finished();
}

void threadTask::simpleColorMap(float* value, int* r, int* g, int* b){
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
