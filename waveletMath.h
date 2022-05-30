#ifndef WAVELETMATH_H
#define WAVELETMATH_H

#define WNumFreq  512


#endif // WAVELETMATH_H



void Test();
float pNorm(float f[],int N, float p);
float innerProduct(float f[],int F,float g[],int G);
float* DWFT(float f[],int N,float g[], int M);
void init_ComplexExp(float W_C[][WNumFreq], float W_S[][WNumFreq]);
void init_GaborWindow(float W_G[]);
void init_Data(float W_X[]);
void DFT();
void GaborTransform(float W_X[],float W_C[][WNumFreq], float W_S[][WNumFreq],float W_MagSf[][WNumFreq]);
