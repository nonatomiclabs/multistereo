#ifndef MSCALLBACK_H
#define MSCALLBACK_H

#include <stdio.h>
#include <math.h>
#include "portaudio.h"
#include "binauralize.h"

static int MSCallback( const void *inputBuffer, void *outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo* timeInfo,
                         PaStreamCallbackFlags statusFlags,
                         void *userData );

#endif