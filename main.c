#include "multistereo.h"

/**
 * \file main.c
 * \author Jean Cruypenynck
 * \date June 2015
 * 
 * Parse different input arguments to define the behavior of the program.
 */

int main(int argc, char *argv[])
{
    if ((argc == 2) && (strcmp("-devs", argv[1]) == 0))
    {
    	printf("cas 1\n");
        listDevices();
    }
    else if ((argc == 5) && (strcmp("-binauralize", argv[1]) == 0)
            && (strcmp("file", argv[2]) == 0))
    {
    	PaError err;
        err = Pa_Initialize();
        if (err != paNoError) exit(-1);
        int outputIndex = (int) Pa_GetDefaultOutputDevice();
        Pa_Terminate();
		binauralizeFile(outputIndex, argv[3], argv[4]);
    }
    else if ((argc == 5) && (strcmp("-binauralize", argv[1]) == 0))
    {
    	printf("cas 2\n");
        int inputIndex  = (int) strtol(argv[2], NULL, 10);
        int outputIndex = (int) strtol(argv[3], NULL, 10);
        binauralizeIO(inputIndex, outputIndex, argv[4]);
    }

    else if ((argc == 4) && (strcmp("-binauralize", argv[1]) == 0)
            && (strcmp("default", argv[2]) == 0))
    {
    	printf("cas 3\n");
        PaError err;
        err = Pa_Initialize();
        if (err != paNoError) exit(-1);
        int inputIndex  = (int) Pa_GetDefaultInputDevice();
        int outputIndex = (int) Pa_GetDefaultOutputDevice();
        Pa_Terminate();
        binauralizeIO(inputIndex, outputIndex, argv[3]);
    }
    else usage();

    return 0;
}