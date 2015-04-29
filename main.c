#include "multistereo.h"

#define SAMPLE_RATE         (48000)
#define PA_SAMPLE_TYPE      paInt24
#define FRAMES_PER_BUFFER   (256)

typedef float SAMPLE;

/* Opens all the IR as a global array of SOUNDFILE*/
IRs = init();

/********************************************************************/
/* Defines the callback function called by Pa_OpenStream() */

static int MSCallback( const void *inputBuffer, void *outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo* timeInfo,
                         PaStreamCallbackFlags statusFlags,
                         void *userData );

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
    int IR_LL, IR_LR, IR_LC, IR_LLs, IR_LRs,
        IR_RL, IR_RR, IR_RC, IR_RLS, IR_RRS;
    for( i=0; i<framesPerBuffer; i++ )
    {
        out[i*2]   =    conv(in[i*5],IRs[0,0]) +
                        conv(in[i*5],IRs[1,0]) +
                        conv(in[i*5],IRs[2,0]) +
                        conv(in[i*5],IRs[3,0])+
                        conv(in[i*5],IRs[4,0]);

        out[i*2+1] =    conv(in[i*5],IRs[0,1]) +
                        conv(in[i*5],IRs[1,1]) +
                        conv(in[i*5],IRs[2,1]) +
                        conv(in[i*5],IRs[3,1])+
                        conv(in[i*5],IRs[4,1]);

    } 
    return paContinue;
}

/********************************************************************/

int main(void)
{
    PaStreamParameters inputParameters, outputParameters;
    PaStream *stream;
    PaError err;

    err = Pa_Initialize();
    if( err != paNoError ) goto error;

    inputParameters.device = Pa_GetDefaultInputDevice(); /* default input device */
    if (inputParameters.device == paNoDevice) {
      fprintf(stderr,"Error: No default input device.\n");
      goto error;
    }
    inputParameters.channelCount = 5;       /* multichannel 5.1 input */
    inputParameters.sampleFormat = PA_SAMPLE_TYPE;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = NULL;

    outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
    if (outputParameters.device == paNoDevice) {
      fprintf(stderr,"Error: No default output device.\n");
      goto error;
    }
    outputParameters.channelCount = 2;       /* stereo output */
    outputParameters.sampleFormat = PA_SAMPLE_TYPE;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    err = Pa_OpenStream(
              &stream,
              &inputParameters,
              &outputParameters,
              SAMPLE_RATE,
              FRAMES_PER_BUFFER,
              0, /* paClipOff, */  /* we won't output out of range samples so don't bother clipping them */
              MSCallback,
              NULL );
    if( err != paNoError ) goto error;

    err = Pa_StartStream( stream );
    if( err != paNoError ) goto error;

    printf("Hit ENTER to stop program.\n");
    getchar();
    err = Pa_CloseStream( stream );
    if( err != paNoError ) goto error;

    printf("Streams closed.\n");
    Pa_Terminate();
    exit();
    return 0;

error:
    Pa_Terminate();
    fprintf( stderr, "An error occured while using the portaudio stream\n" );
    fprintf( stderr, "Error number: %d\n", err );
    fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
    exit();
    return -1;
}