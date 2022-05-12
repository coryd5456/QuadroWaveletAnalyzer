#include <iostream>
#include <math.h>
#include <cmath>
#include "waveletMath.h"
#define LOG(x) std::cout << x <<std::endl
int N = 16;
#define PI 3.14159265
#define W_exp 2.7182818
float W_C[16][16];
float W_S[16][16];
float W_X[16];
float W_G[16];

float W_MagSf[16][16];
float W_Mfft[16];
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

void init_ComplexExp(){
// sets up e^{-j2PI ln/N}
// where n is the time index
// and where l is the frequency index
    for (int n = 0; n < N; ++n)
    {
        for (int l = 0; l < N; ++l)
        {

            W_C[n][l] = cos(2*PI*n*l/N);
            W_S[n][l] = -1*sin(2*PI*n*l/N);
        }
    }
}


void init_GaborWindow(){
//choice of Gabor window
//where n is the time index
    for (int n = 0; n < N-1; ++n)
    {
        W_G[n] = pow(W_exp,-1*pow(n-N/2,2));
    }
}

void init_Data(){
//data where n is the time index
    for (int n = 0; n < N; ++n)
    {
        W_X[n] = 5.0 + 2.0*sin(2*PI*n/N) + 3.0*cos(4*PI*n/N) +10 * pow(W_exp,-1*pow((n-N/2)*2,2)) ;//+ 9.0*cos(16*PI*PI*n/N);
        //W_X[n] = 5.0 + sinh(2.0*n/N);
        //W_X[n] = 5.0 + 3.0*cos(4*PI*n/N + 12.0*cos(4*PI*n/N));
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
            W_Mfft[j] = 2*sqrt(pow(Sum[0],2) + pow(Sum[1],2))/N;
        }
    W_Mfft[0] = W_Mfft[0]/2;
}

//pow(exp,-1*pow((n-m),2))
void GaborTransform(){
// n is the time index
// l is the frequency index
// m is the center of the time window

    for (int m = 0; m < N; ++m)
    {

        for (int l = 0; l < N; ++l)
            {	double Sum[] = {0.0,0.0};

            for (int n = 0; n < N; ++n)
                {
                // integrating the real part
                Sum[0] = W_X[n]*pow(W_exp,-1*pow((n-m),2))*W_C[n][l] + Sum[0];
                // integrating the imaginary part
                Sum[1] = W_X[n]*pow(W_exp,-1*pow((n-m),2))*W_S[n][l] + Sum[1];

                }
            // The divide by N here is the dt of the integral
            W_MagSf[m][l] = 2*sqrt(pow(Sum[0],2) + pow(Sum[1],2))/N;

            }
            W_MagSf[m][0] = W_MagSf[m][0]/2;
    }
}


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
