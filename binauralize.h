#ifndef BINAURALIZE_H
#define BINAURALIZE_H

#include <stdio.h>
#include <math.h>
#include <sndfile.h>
#include "portaudio.h"
#include "MSCallback.h"

//float			Binauralize		( float input, int index );
//SNDFILE*		read_wave		( char* , SF_INFO* );
float			conv			( float, int );

#endif