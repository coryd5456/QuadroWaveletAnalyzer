#include <iostream>
//#include <math.h>
#include <cmath>
#include <vector>
//look into installing OpenGL Math libraries
#include <glm/exponential.hpp>
#include <glm/trigonometric.hpp>
#include "waveletMath.h"
#define LOG(x) std::cout << x <<std::endl

#define BucketSize  1024
#define NumFreq  512
#define GaborWidth  32
//To make this faster I need to pass my variables by reference instead of redoing them every time I call the draw function
int N = BucketSize;// Size of the data bucket
int L = NumFreq;// Number of frequncie to decompose into
int M = GaborWidth;// Width of the window

#define PI 3.14159265
#define W_exp 2.7182818

float W_C[BucketSize][WNumFreq];//[width of dataset][number of frequencies]?
float W_S[BucketSize][WNumFreq];//[width of dataset][number of frequencies]?
float W_X[BucketSize];//size of the data set
float W_G[GaborWidth];//width of the window

float W_MagSf[BucketSize][WNumFreq];//[width of window][number of frequencies]
float W_RTMagSf[WNumFreq];//[number of frequencies]
float W_AngSf[BucketSize][WNumFreq];//[width of window][number of frequencies]
float W_Mfft[WNumFreq];//Number of frequencies
float W_d = 0;
std::vector<float> dataBucket;


/////
/// Before I really start building this out I need to identify the type of data
/// That I'm going to be feeding these functions.
///
/// Need to find my old wavelet notebook to take notes out of my book for.
///
/// 1. So, my first task is to probably start recieving signal data
/// 2. then I need to create some test examples basic wave functions
/// 3. program my own FFT
/// 4. Create buttons that can do each kind of transform
///     -whole thing?
///         Let's start with this one to make sure that the analysis method works
///         a. Need to pick an image size to work with.
///         b. Create test functions
///         c. test out gabor transform
///         d. test out ridglet transform
///         e. test out Wigner-ville distribution
///         f. test out Cohen's Classes
///     - real time?
///         Once I get the audio data mapped out I can begin doing this like I did for the
///         FPGA wavelet analyzer project
///         a. Get qAudio working
///         b. Get qSerial working
///         c. test out gabor transform
///         d. test out ridglet transform
///         e. test out Wigner-ville distribution
///         f. test out Cohen's Classes
/// 5. Dive into Cherno's videos to learn more about the stack and heap
/// 6. See if I can improve the speed
///     a. Using my RAM?
///     b. Using multi-threading
///     c. Using cuda cores on my graphics card
///
///
///



void Test(){
    LOG(2);
}

//Need to set these functions up as a pass by reference.
//Also need to review the math for these to fully take advantage.
//




/*FFT(x) {
  n=length(x);
  if (n==1) return x;
  m = n/2;
  X = (x_{2j})_{j=0}^{m-1};
  Y = (x_{2j+1})_{j=0}^{m-1};
  X = FFT(X);
  Y = FFT(Y);
  U = (X_{k mod m})_{k=0}^{n-1};
  V = (g^{-k}Y_{k mod m})_{k=0}^{n-1};
  return U+V;
}*/
//I'm getting a segmentation fault when I try to run the code now.
void init_ComplexExp(float W_C[][WNumFreq], float W_S[][WNumFreq]){
// sets up e^{-j2PI ln/N}
// where n is the time index
// and where l is the frequency index
    for (int n = 0; n < 1024; n++)
    {
        for (int l = 0; l < 512; l++)
        {
            //need to figure out how to jump by more than 1 frequency for each row of pixels.
            W_C[n][l] = glm::cos(2*PI*(double)(n*l)/(float)(N));
            W_S[n][l] = -1.0*glm::sin(2*PI*(double)(n*l)/(float)(N));
            //W_C[n][l] = cos(n*l);
            //W_S[n][l] = -1.0*std::sin(2.0);
            //LOG("(n,l): (") << n << ","<< l << ") " << "  C_G: " << W_S[n][l] << "\n";
        }

    }
}


void init_GaborWindow(float W_G[]){
//choice of Gabor window
//where n is the time index
    for (int n = 0; n < M; ++n)
    {
        //e^(-x^2)
        //float d = pow(((float)(n)-8)/8,2);
        W_G[n] = pow(W_exp,-1.0*pow((float)(n-8)/8,2));
        //LOG("n: ") << n << "  W_G: " << W_G[n] << " d: " << d << "\n";
    }
}

void init_Data(float W_X[]){
//data where n is the time index
    for (int n = 0; n < N; ++n)
    {
        W_X[n] = 5.0 + 2.0*sin(300.0*PI*(double)(n)/(double)(N)) + 3.0*cos(80.0*PI*(double)(n)/(double)(N)) + 40.0 * pow(W_exp,-1.0*pow(((double)(n)-(double)(N)/4.0)*2.0,2.0)) ;//+ 9.0*cos(16*PI*PI*n/N);
        //LOG(W_X[n])<<"\n";
        //W_X[n] = 5.0 + sinh(2.0*n/N);
        //W_X[n] = 5.0 + 3.0*cos(4*PI*n/N + 12.0*cos(4*PI*n/N));
        //Two Linear Chirps
        //W_X[n] = 1.0+5.0*glm::cos(300.0*PI*glm::pow((double)(n)/(double)(N),2.0) + 100.0*PI*((double)(n)/(double)(N)))+5.0*glm::cos(300.0*PI*pow((double)(n)/(double)(N),2.0)) ;// + 5.0*cos(-300.0*PI*pow((double)(n)/(double)(N),3.0) + 600.0*PI*((double)(n)/(double)(N)));
    }
}

void DFT(){
    for (int j = 0; j < N; ++j)
    {	float Sum[] = {0.0,0.0};

        for (int i = 0; i < N; ++i)
            {
                Sum[0] = W_X[i]*W_C[i][j] + Sum[0];
                Sum[1] = W_X[i]*W_S[i][j] + Sum[1];

            }
                //Rfft[j] = Sum[0];
                //Ifft[j] = Sum[1];
            W_Mfft[j] = 2.0*sqrt(pow(Sum[0],2.0) + pow(Sum[1],2.0))/N;
        }
    W_Mfft[0] = W_Mfft[0]/2;
}

//This is an un-optimized version that computes the whole thing at once instead of just one time stamp?
//Need to add in angle result

//pow(exp,-1*pow((n-m),2))
void GaborTransform(float W_X[],float W_C[][WNumFreq], float W_S[][WNumFreq],float W_MagSf[][WNumFreq],float W_AngSf[][WNumFreq]){
// n is the time index
// l is the frequency index
// m is the center of the time window

// I need to shift this around to being added over the width of M to speed it up
    //The m's compute every shift
    for (int m = 0; m < N-M; ++m)
    {
        // The l's compute every frequnecy
        for (int l = 0; l < L; ++l)
            {	double Sum[] = {0.0,0.0};
            //This for loop gives you the complex gabor coefficient
            for (int n = 0; n < N; ++n)
                {
                // integrating the real part
                // I need to add in s(n-m) for a resolution scaling factor.
                Sum[0] = W_X[n]*glm::pow(W_exp,-PI*glm::pow(((float)(n-m))/(float)(M),2))*W_C[n][l] + Sum[0];
                // integrating the imaginary part
                Sum[1] = W_X[n]*glm::pow(W_exp,-PI*glm::pow(((float)(n-m))/(float)(M),2))*W_S[n][l] + Sum[1];

                }
            //We are using the outer 2 for loops to go through each frequency
            // The divide by N here is the dt of the integral
            W_MagSf[m][l] = 2*glm::sqrt(glm::pow(Sum[0],2) + glm::pow(Sum[1],2))/N;
            W_AngSf[m][l] = glm::atan(Sum[1]/Sum[0]);

            }
            W_MagSf[m][0] = W_MagSf[m][0]/2.0;
    }
}

//Need to create real time gabor transform that computes a single column from the newest data point.
//Pick a buffer size of 256 to compute the data on.


////////////////////////////////////////////////////////////////////////////
/// \brief RTGaborTransform
/// \param W_C
/// \param W_S
///To pull this off I just need to compute for a single m.
/// This will be for the current data point.
/// however, the window needs to be 256 long to be accurate for
/// different scaling values.
/// So, I need to create a buffer.
///
///
/// First let's create a working model for 256 data points. powers of 2 because it compiles faster on microcontrollers
///
/// To improve performance:
/// I can pre-process the bell curve the the gabor window function
/// I can use the FFT recursion algorithm to reduce this to NLog(M) from N*M
void RTGaborTransform(std::vector<float>* dataBucket, float* GaborScale ,float W_C[][WNumFreq], float W_S[][WNumFreq],float W_RTMagSf[],float W_RTAngSf[]){
    // n is the time index
    // l is the frequency index
    // m is the center of the time window

    // I need to shift this around to being added over the width of M to speed it up
        //The m's compute every shift
    //since I just have the current shift, then m = 0
    // The l's compute every frequnecy
            //int m = 0;
            //Checking if data bucket is resieved
            /*
            for(float v: dataBucket){
                LOG(v)<<"\n";
            }*/
            //The function is recieving the data buckets
            //currently M is acting as the scaling factor.
            //float S = 40.0; //Original Gabor Scale
            float C_Sum = 0.0;
            float S_Sum = 0.0;
            float windowedData = 0.0;

            //swapping n and l will improve speeds here, because I will have to compute over n less times?
            //are these for loops communitive?
            for (int l = 0; l < L; l++)
                {	C_Sum = 0.0; S_Sum = 0.0;
                //This for loop gives you the complex gabor coefficient
                for (int n=0; n<dataBucket->size() ;n++)
                    {
                    // integrating the real part
                    // I need to add in s(n-m) for a resolution scaling factor.
                    //M/2 to shift the peak to the middle of the N data points
                    // since the data comes in groups of 256, this will compute for the 128th data point
                    // we are calling the 128th data point the current one to be displayed.

                    windowedData = (*dataBucket)[n]*glm::pow(W_exp,-PI*((((float)(n-(128)))/(*GaborScale))*(((float)(n-(128)))/(*GaborScale))));
                    C_Sum += windowedData *W_C[n][l];
                    // integrating the imaginary part
                    S_Sum += windowedData*W_S[n][l];

                    }
                //We are using the outer 2 for loops to go through each frequency
                // The divide by N here is the dt of the integral
                W_RTMagSf[l] = 2.0*glm::sqrt(C_Sum*C_Sum + S_Sum*S_Sum)/dataBucket->size();
                //values are being computed
                //LOG(W_RTMagSf[l])<<"These are the values of the gabor transform\n";
                W_RTAngSf[l] = glm::atan(S_Sum/C_Sum);

                }
                W_RTMagSf[0] /=2.0;
}

//So, for the ridglets just write a quick algorithm to find the maximums.


//Discreate Windowed Fourier Transform
float* DWFT(float f[],int N,float g[], int M){
    float* WT = new float[N-M];
    for(int i = 0;i<(N-M);i++){
        WT[i] = 0;
        for(int j =0; j<M;j++){
        //This is what I want this to do
        WT[i] += f[j+i]*g[j]; //exp(-i*2*M_PI*l*n/N);
        }
    }
    return WT;
}

//Discreate Wavelet Transform


//Resolution Function


//Time Uncertanty

//Frequency Uncertanty

//Discreate Gabor Transform

//Calculates the pNorm

//Inner Product
//input two functions and their lengths
float innerProduct(float f[],int F,float g[],int G){
    float innerProductVal = 0;
    if (F==G){
        for(int i = 0; i<F;i++){
            innerProductVal += f[i]*g[i];
        }
        return innerProductVal;
    }else{
        std::cout << "The lengths of f[] and g[] must be the same." <<std::endl;
        return 0;
    }

}

//Takes in the data and the length of the data
float pNorm(float f[],int N,float p){
    float pNormValue = 0;
    if (p>0){
     for(int i=0;i<N;i++){
         pNormValue += pow (std::abs(f[i]),p);
     }
     pNormValue = pow(pNormValue,1/p);
     return pNormValue;
    }else{
        std::cout << "You need a value for p>0" <<std::endl;
        return 0;
    }
    return -1;
}


//Reproduction Kernal

//Ambiguity function
