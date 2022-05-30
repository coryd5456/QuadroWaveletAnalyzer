#include <iostream>
//#include <math.h>
#include <cmath>
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
float W_Mfft[WNumFreq];//Number of frequencies
float W_d = 0;


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

void init_ComplexExp(float W_C[][WNumFreq], float W_S[][WNumFreq]){
// sets up e^{-j2PI ln/N}
// where n is the time index
// and where l is the frequency index
    for (int n = 0; n < N; ++n)
    {
        for (int l = 0; l < L; ++l)
        {

            W_C[n][l] = cos(2*PI*(double)(n*l)/(float)(N));
            W_S[n][l] = -1.0*sin(2*PI*(double)(n*l)/(float)(N));
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
        //W_X[n] = 5.0 + 2.0*sin(300.0*PI*n/N) + 3.0*cos(80.0*PI*n/N) + 40 * pow(W_exp,-1*pow((n-N/4)*2,2)) ;//+ 9.0*cos(16*PI*PI*n/N);
        //LOG(W_X[n])<<"\n";
        //W_X[n] = 5.0 + sinh(2.0*n/N);
        //W_X[n] = 5.0 + 3.0*cos(4*PI*n/N + 12.0*cos(4*PI*n/N));
        //Two Linear Chirps
        W_X[n] = 1.0+5.0*cos(300.0*PI*pow((double)(n)/(double)(N),2.0) + 100.0*PI*((double)(n)/(double)(N)))+5.0*cos(300.0*PI*pow((double)(n)/(double)(N),2.0)) + 5.0*cos(-300.0*PI*pow((double)(n)/(double)(N),3.0) + 600.0*PI*((double)(n)/(double)(N)));
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


//pow(exp,-1*pow((n-m),2))
void GaborTransform(float W_X[],float W_C[][WNumFreq], float W_S[][WNumFreq],float W_MagSf[][WNumFreq]){
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
                Sum[0] = W_X[n]*pow(W_exp,-PI*pow(((float)(n-m))/(float)(M),2))*W_C[n][l] + Sum[0];
                // integrating the imaginary part
                Sum[1] = W_X[n]*pow(W_exp,-PI*pow(((float)(n-m))/(float)(M),2))*W_S[n][l] + Sum[1];

                }
            //We are using the outer 2 for loops to go through each frequency
            // The divide by N here is the dt of the integral
            W_MagSf[m][l] = 2*sqrt(pow(Sum[0],2) + pow(Sum[1],2))/N;

            }
            W_MagSf[m][0] = W_MagSf[m][0]/2;
    }
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
