#ifndef _MYLIB_H_
#define _MYLIB_H_

    double** DFT(double* samples, int WINDOWSIZE);

    double* InverseDFT(double** freqRI, int freqSize);

    // -= RADIX-2 DIT FFT =- (cooley-Tukey)
    //double** FFT(double* samples, int wsize);// requires 2^N samples input
#endif
