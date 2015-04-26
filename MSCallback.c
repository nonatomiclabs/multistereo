#include "MSCallback.h"

static int MSCallback( const void *inputBuffer, void *outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo* timeInfo,
                         PaStreamCallbackFlags statusFlags,
                         void *userData )
{
    SAMPLE *out = (SAMPLE*)outputBuffer;
    const SAMPLE *in = (const SAMPLE*)inputBuffer;
    unsigned int i;
    (void) timeInfo; /* Prevent unused variable warnings. */
    (void) statusFlags;
    (void) userData;

    if( inputBuffer == NULL )
    {
        for( i=0; i<framesPerBuffer; i++ )
        {
            *out++ = 0;  /* left - silent */
            *out++ = 0;  /* right - silent */
        }
        gNumNoInputs += 1;
    }
    else
    {
        for( i=0; i<framesPerBuffer; i++ )
        {
            *out++ = Binauralize(*in++,0);
            *out++ = Binauralize(*in++,1);
            *out++ = Binauralize(*in++,2);
            *out++ = Binauralize(*in++,3);
            *out++ = Binauralize(*in++,4);
        }
    }
    
    return paContinue;
}