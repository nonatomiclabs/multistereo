#ifndef MULTISTEREO
#define MULTISTEREO

/**
 * \file multistereo.h
 * \author Jean Cruypenynck
 * \date June 2015
 * 
 * Defines different constants and functions used in the program.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <complex.h>
#include <fftw3.h>
#include <portaudio.h>
#include <sndfile.h>

/**
 * Display more informations during the execution of the program if not 0.
 * 
 * Used in init() to display IR signal before and after FFT to be sure that everything is fine.
 */
#define VERBOSE 0

/**
 * Used to tune color of console output in an easy way.
 */
#define color(param) printf("\033[%sm",param)


#define SAMPLE_RATE         (48000)
#define PA_SAMPLE_TYPE      (paFloat32|paNonInterleaved)
#define FRAMES_PER_BUFFER   (1024)

/**
 * Struct used to send data to the callback function.
 */
typedef struct
{
	fftwf_complex *sp_IR_LL; /*!< Spectrum of impulse response LL */
	fftwf_complex *sp_IR_RL; /*!< Spectrum of impulse response RL */
	fftwf_complex *sp_IR_LR; /*!< Spectrum of impulse response LR */
	fftwf_complex *sp_IR_RR; /*!< Spectrum of impulse response RR */

	fftwf_plan forward;		 /*!< FFTW forward plan used inside the callback */
	fftwf_plan backward;	 /*!< FFTW backward plan used inside the callback */
}
sharedData;

/**
 * init() is called when the program starts, before streams are
 * opened by portaudio.
 * It is intended to load all impulse responses in RAM and process their FFT for being used
 * after in the callback function.
 * \param path Path to the folder containing impulse response audio files.
 * \returns A sharedData structure then passed to the callback.
 */
sharedData init (const char* path);

/**
 * Responsible for opening audio streams which will be used to binauralize the audio.
 * \param inputIndex  Index of the input device to use to open audio stream.
 * \param outputIndex Index of the output device to use to open audio stream.
 * \param path Path to the folder containing impulse response audio files.
 */
void binauralize (int inputIndex, int outputIndex, const char* path);


/** Callback called to process the 'binauralization'
	\returns PaContinue
 */
int multiStereoCallback(const void *inputBuffer,
						void *outputBuffer,
						unsigned long framesPerBuffer,
						const PaStreamCallbackTimeInfo* timeInfo,
						PaStreamCallbackFlags statusFlags,
						void *userData);




/** Displays rules and different flags to use the program properly.
 */
void usage (void);

/** Called with the flag '-devs' to list available devices.
 */
void listDevices (void);

/** Called in the end of the execution of the program to deallocate memory and clean everything.
 */
void closeFiles (void);

/** Simple concatenation function used to open soundfiles by passing an argument to the program.
 *  \returns The concatenated string.
 */
char* concatenate (const char *string1, const char *string2);

#endif