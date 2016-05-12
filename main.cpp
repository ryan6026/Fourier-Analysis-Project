#include "Transform.h"
#include "Fourier.h"
#define WINDOWSIZE 512
using namespace std;
using std::string;
using std::fstream;

typedef struct  WAV_HEADER
{
    char                RIFF[4];        // RIFF Header      Magic header
    unsigned long       ChunkSize;      // RIFF Chunk Size
    char                WAVE[4];        // WAVE Header
    char                fmt[4];         // FMT header
    unsigned long       Subchunk1Size;  // Size of the fmt chunk
    unsigned short      AudioFormat;    // Audio format 1=PCM,6=mulaw,7=alaw, 257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
    unsigned short      NumOfChan;      // Number of channels 1=Mono 2=Sterio
    unsigned long       SamplesPerSec;  // Sampling Frequency in Hz
    unsigned long       bytesPerSec;    // bytes per second
    unsigned short      blockAlign;     // 2=16-bit mono, 4=16-bit stereo
    unsigned short      bitsPerSample;  // Number of bits per sample
    char                Subchunk2ID[4]; // "data"  string
    unsigned long       Subchunk2Size;  // Sampled data length

} wav_hdr;

wav_hdr wavHeader;
FILE *wavFile;
int getFileSize(FILE *inFile);

int getFileSize(FILE *inFile){\
    int fileSize = 0;
    fseek(inFile,0,SEEK_END);
    fileSize=ftell(inFile);
    fseek(inFile,0,SEEK_SET);
    return fileSize;
}

void readHeader()
{
    int headerSize = sizeof(wav_hdr),filelength = 0;
    cout << "opening 'wav.wav'" << endl;
    wavFile = fopen( "wav.wav" , "r" );

    if(wavFile == NULL)
    {
        printf("Can not open wave file\n");
    }

    fread(&wavHeader,headerSize,1,wavFile);
    filelength = getFileSize(wavFile);

    cout << "File is                    :" << filelength << " bytes." << endl;
    cout << "RIFF header                :" << wavHeader.RIFF[0]
         << wavHeader.RIFF[1]
         << wavHeader.RIFF[2]
         << wavHeader.RIFF[3] << endl;
    cout << "WAVE header                :" << wavHeader.WAVE[0]
         << wavHeader.WAVE[1]
         << wavHeader.WAVE[2]
         << wavHeader.WAVE[3]
         << endl;
    cout << "FMT                        :" << wavHeader.fmt[0]
         << wavHeader.fmt[1]
         << wavHeader.fmt[2]
         << wavHeader.fmt[3]
         << endl;
    cout << "Data size      :" << wavHeader.ChunkSize << endl;
    
    // Display the sampling Rate form the header
    cout << "Sampling Rate              :" << wavHeader.SamplesPerSec << endl;
    cout << "Number of bits used        :" << wavHeader.bitsPerSample << endl;
    cout << "Number of channels         :" << wavHeader.NumOfChan << endl;
    cout << "Number of bytes per second :" << wavHeader.bytesPerSec << endl;
    cout << "Data length                :" << wavHeader.Subchunk2Size << endl;
    cout << "Audio Format               :" << wavHeader.AudioFormat << endl;
    // Audio format 1=PCM,6=mulaw,7=alaw, 257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
    cout << "Block align                :" << wavHeader.blockAlign << endl;
    cout << "Data string                :" << wavHeader.Subchunk2ID[0]
         << wavHeader.Subchunk2ID[1]
         << wavHeader.Subchunk2ID[2]
         << wavHeader.Subchunk2ID[3] << endl;
}

int main()
{
    readHeader();
    cout << wavHeader.Subchunk2Size/2 << " samples loaded into memory" << endl;
    cout << "window size: " << WINDOWSIZE << endl;
    cout << "max window: " << (wavHeader.Subchunk2Size/(WINDOWSIZE*2)) <<endl;

    short int *tmp;
    tmp = new short int[(wavHeader.Subchunk2Size/2)+22];
    fread(tmp,1,wavHeader.Subchunk2Size+44,wavFile);
    for(unsigned int i=0;i<wavHeader.Subchunk2Size/2;i++){ // to get rid of header data
        tmp[i] = tmp[i+22];
    }

    Transform* lol = new Transform(WINDOWSIZE, tmp, wavHeader.Subchunk2Size/(WINDOWSIZE*2));
    lol->forwardTransform();
    cout << "filter?" << endl;
    string in = "";
    cin >> in;
    if(in=="y"||in=="Y"){
        cout << "filtering..." << endl;
        lol->filter();
    }else{
        cout << "no filtering..." << endl;
    }
    lol->inverseTransform();
    lol->saveSamples(&wavHeader);

    cout << endl << "press enter to exit";
    cin.get();
    return 0;
}
