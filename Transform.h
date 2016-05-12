#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>
#include "Fourier.h"
#define PI 3.14159265359
#ifndef Transform_H
#define Transform_H
using namespace std;
template <typename T>
T **Alloc2DArray( int nRows, int nCols)
{
    T **dynamicArray;

    dynamicArray = new T*[nRows];
    for( int i = 0 ; i < nRows ; i++ )
        dynamicArray[i] = new T [nCols];

    return dynamicArray;
}

template <typename T>
T ***Alloc3DArray( int nRows, int nCols, int nDepth)
{
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

class Transform
{
    int MAXWIND;
    int WINDOWSIZE;
    short int* samples;
    double* window;
    double*** windFreqSamp;
    double* samplesIn;

public:
    Transform(int WS, short int* samps, int w)
    {
        MAXWIND = w;
        WINDOWSIZE = WS;
        samples = samps;
        windFreqSamp = Alloc3DArray<double>(MAXWIND, WINDOWSIZE, 2);
        samplesIn = new double[WS];
    }

    void setSamples(short int* samps){samples = samps;}

    void setWindow(double* win){window=win;}

    short int* getSamples(){return samples;}

    double*** getWindFreqSamp(){return windFreqSamp;}

    void forwardTransform()
    {
        cout << endl << "starting Forward Transform..." << endl;
        for(int w=0; w<MAXWIND; w++)
        {
            for(int c=0; c<WINDOWSIZE; c++)
            {
                samplesIn[c] = samples[(c+(w*WINDOWSIZE))];
            }
            windFreqSamp[w] = DFT(samplesIn, WINDOWSIZE);
            if(w%2==0){
                cout << ".";
            }
        }
        cout << endl << "done with Forward Transfer Function." << endl << endl;
    }

    void filter(){
        double* window; window =  new double[WINDOWSIZE/2];
        for(double i=0; i<WINDOWSIZE/2; i++)
        {
                window[(int)i] = sqrt(1/(1+(i*i)/7000));
            cout << window[(int)i] << ", ";
        }
        for(int w=0;w<MAXWIND;w++){
            for(int i=0;i<WINDOWSIZE/2;i++){
                windFreqSamp[w][i][0] *= window[i];
                windFreqSamp[w][i][1] *= window[i];
            }
        }
    }

    void inverseTransform()
    {
        cout << endl << "Starting Inverse Transfer Function" << endl;
        for(int w=0; w<MAXWIND; w++)
        {
            samplesIn = InverseDFT(windFreqSamp[w], WINDOWSIZE);
            for(int c=0; c<WINDOWSIZE; c++)
            {
                samples[c+(w*(WINDOWSIZE))] = (short int)samplesIn[c];
            }
            cout << ".";
        }
        cout << endl << "done with Inverse Transfer Function!" << endl;
    }

    void saveSamples(const void* wavHeaderAddress)
    {
        cout << "normalizing audio samples..." << endl;
        double biggest=0;
        for(int i=0; i<MAXWIND*WINDOWSIZE; i++)
        {
            if(samples[i]>biggest)
            {biggest = samples[i];}
        }
        int mFactor = 32700/biggest;
        for(int i=0; i<MAXWIND*WINDOWSIZE; i++)
        {samples[i] *= mFactor;}
        cout << "done" << endl;


        cout << "Writing binary data samples to 'thatcoolnewwav.wav'" << endl;
        FILE *outFile;
        outFile = fopen( "thatcoolnewwav.wav" , "wb" );
        fwrite(wavHeaderAddress, 1, 44, outFile);
        fwrite((char *)samples, 1, WINDOWSIZE*MAXWIND, outFile);
        cout << WINDOWSIZE*MAXWIND<< " samples written!" << endl;
        cout << "writing wolfram data" << endl;

        }
        cin.get();
    }
};
#endif // Transform_H
/*
ofstream yolo;
        yolo.close();
        yolo.open("wolfram3d.nb");
        yolo << "ListPlot3D[{";
        for(int w=0; w<MAXWIND; w++){
            for(int k=0; k<WINDOWSIZE/2; k++){
                if(true){
                    yolo << ", {" << w << ", " << k << ", " << log(sqrt(windFreqSamp[w][k][0]*windFreqSamp[w][k][0]
                                                                       +windFreqSamp[w][k][1]*windFreqSamp[w][k][1])) << "}";
                }
            }
            yolo << "},Joined->True]";
            */
