#include "Fourier.h"
#include <iostream>
#include <cmath>
#define PI 3.14159265359
using namespace std;
using std::string;
using std::fstream;

template <typename T>
T **Alloc2DArray( int nRows, int nCols){
    T **dynamicArray;

    dynamicArray = new T*[nRows];
    for( int i = 0 ; i < nRows ; i++ )
        dynamicArray[i] = new T [nCols];

    return dynamicArray;
}

template <typename T>
T ***Alloc3DArray( int nRows, int nCols, int nDepth){
    T ***dynamicArray;
    dynamicArray = new T**[nRows];
    for( int i = 0 ; i < nRows ; i++ )
    {
        dynamicArray[i] = new T *[nCols];
        for(int u = 0; u<nCols; u++)
        {
            dynamicArray[i][u] = new T [nDepth];
        }
    }
    return dynamicArray;
}

double** DFT(double* samples, int WINDOWSIZE){
    double real, imag;
    double** freqRI = Alloc2DArray<double>(WINDOWSIZE,2);
    double const1 = -2*PI;
    double const2 = 0;
    for(int k=0; k<WINDOWSIZE/2; k++)
    {
        const2 = const1*k;
        real = imag = 0;
        for(int n=0; n<WINDOWSIZE; n++)
        {
            real += samples[n] * cos(const2*n/WINDOWSIZE);
            imag += samples[n] * sin(const2*n/WINDOWSIZE);
        }
        freqRI[k][0] = real;
        freqRI[k][1] = imag;
    }
    return freqRI;
}

double* InverseDFT(double** freqRI, int WINDOWSIZE){
    double *samples;
    samples = new double[WINDOWSIZE];
    double real=0, imag=0, const1 = 2*PI;
    for(int n=0; n<WINDOWSIZE; n++)
    {
        real = imag = 0;
        for(int k=0; k<WINDOWSIZE/2; k++) //try removing Zpadding form this loop to run shorter
        {
            real += freqRI[k][0]*cos(const1*k*n/(WINDOWSIZE));
            imag += freqRI[k][1]*sin(const1*k*n/(WINDOWSIZE));
        }
        samples[n] = (real-imag)/WINDOWSIZE;
        }
    return samples;
}

// -= RADIX-2 DIT FFT =- (cooley-Tukey)
/*

double** FFT(double* samples, int wsize){//(DOESNT WORK!!!!!!)


    double* even; even = new double[wsize/2];
    double* odd; odd = new double[wsize/2];
    for(int i=0;i<wsize/2;i++){
        even[i] = samples[2*i];
        odd[i] = samples[(2*i)+1];
    }
    double** freqRI = Alloc2DArray<double>(wsize/2, 2);
    double** freqRIO = Alloc2DArray<double>(wsize/2, 2);
    double** freqRIE = Alloc2DArray<double>(wsize/2, 2);

    freqRIE = DFT(even, wsize/2);
    freqRIO = DFT(odd, wsize/2);
    for(int i=0;i<wsize/4;i++){
        freqRI[i][0] = freqRIE[i][0]+freqRIO[i][0];
        freqRI[i][1] = freqRIE[i][1]+freqRIO[i][1];
    }
    return freqRI;
}
*/
