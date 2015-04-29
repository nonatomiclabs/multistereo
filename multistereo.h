#ifndef BINAURALIZE_H
#define BINAURALIZE_H

#include <stdio.h>
#include <math.h>
#include <sndfile.h>
#include "portaudio.h"
#include "MSCallback.h"


SOUNDFILE*		init		( void );
void			exit		( void );
float			conv		( float, SOUNDFILE );

#endif