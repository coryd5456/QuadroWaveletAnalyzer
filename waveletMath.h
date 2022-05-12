#ifndef WAVELETMATH_H
#define WAVELETMATH_H





#endif // WAVELETMATH_H

void Test();
float pNorm(float f[],int N, float p);
float innerProduct(float f[],int F,float g[],int G);
float* DWFT(float f[],int N,float g[], int M);
void init_ComplexExp();
void init_GaborWindow();
void init_Data();
void DFT();
void GaborTransform();
