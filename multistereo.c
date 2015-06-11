#include "multistereo.h"

/***********************************************************************************/
/**********************************  INITIO ()  **************************************/
/***********************************************************************************/
sharedData initIO (const char *path) {
    SF_INFO IRInfo;
    sharedData IRData;

    /* Defines the paths to IR files */
    char *path_IR_LL = concatenate(path,"/IR_LL.wav");
    char *path_IR_RL = concatenate(path,"/IR_RL.wav");
    char *path_IR_LR = concatenate(path,"/IR_LR.wav");
    char *path_IR_RR = concatenate(path,"/IR_RR.wav");
    
    /* Opens WAV files to SNDFILEs */
    SNDFILE *sndfile_IR_LL  = sf_open(path_IR_LL, SFM_READ, &IRInfo);
    if (sndfile_IR_LL == NULL) {
        puts("Impossible to open IR_LL");
        puts("Read the documentation for more informations.");
        exit(-1);
    }
    
    SNDFILE *sndfile_IR_RL  = sf_open(path_IR_RL, SFM_READ, &IRInfo);
    if (sndfile_IR_RL == NULL) {
        puts("Impossible to open IR_RL");
        puts("Read the documentation for more informations.");
        exit(-1);
    }

    SNDFILE *sndfile_IR_LR  = sf_open(path_IR_LR, SFM_READ, &IRInfo);
    if (sndfile_IR_LL == NULL) {
        puts("Impossible to open IR_LR");
        puts("Read the documentation for more informations.");
        exit(-1);
    }
    
    SNDFILE *sndfile_IR_RR  = sf_open(path_IR_RR, SFM_READ, &IRInfo);
    if (sndfile_IR_RL == NULL) {
        puts("Impossible to open IR_RR");
        puts("Read the documentation for more informations.");
        exit(-1);
    }

    free(path_IR_LL);
    free(path_IR_RL);
    free(path_IR_LR);
    free(path_IR_RR);

    puts("--------------------------------------------------------------------------");
    printf("Impulse response files correctly opened.\n");

    /* Process the FFT of IR files */
    float *IR_LL = (float*) calloc(2048, sizeof(float));
    float *IR_RL = (float*) calloc(2048, sizeof(float));

    float *IR_LR = (float*) calloc(2048, sizeof(float));
    float *IR_RR = (float*) calloc(2048, sizeof(float));

    sf_read_float(sndfile_IR_LL, IR_LL, 1024);
    sf_read_float(sndfile_IR_RL, IR_RL, 1024);

    sf_read_float(sndfile_IR_LR, IR_LR, 1024);
    sf_read_float(sndfile_IR_RR, IR_RR, 1024);

    sf_close(sndfile_IR_LL);
    sf_close(sndfile_IR_RL);

    sf_close(sndfile_IR_LR);
    sf_close(sndfile_IR_RR);

    fftwf_plan forward, backward;

    fftwf_complex *sp_IR_LL = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * 1025);
    fftwf_complex *sp_IR_RL = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * 1025);

    fftwf_complex *sp_IR_LR = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * 1025);
    fftwf_complex *sp_IR_RR = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * 1025);

    forward  = fftwf_plan_dft_r2c_1d(2048, IR_LL, sp_IR_LL, FFTW_PATIENT);
    backward = fftwf_plan_dft_c2r_1d(2048, sp_IR_LL, IR_LL, FFTW_PATIENT);


    fftwf_execute(forward);
    fftwf_execute_dft_r2c(forward, IR_LL, sp_IR_LL);
    fftwf_execute_dft_r2c(forward, IR_LR, sp_IR_LR);
    fftwf_execute_dft_r2c(forward, IR_RR, sp_IR_RR);

    #if VERBOSE
    puts("");
    puts("--------------------------------------------------------------------------");
    puts("-----------------------  VERBOSE  MODE  CALLED  --------------------------");
    puts("--------------------------------------------------------------------------");

    puts("INPUT:");
    for (size_t i = 0; i < 8; ++i)
    {
        printf("%zu: %f\n", i, IR_LL[i]);
    }
    puts("");

    puts("OUTPUT:");
    for (size_t i = 0; i < 8; ++i)
    {
        printf("%zu: %f + i %f\n", i, creal(sp_IR_LL[i]), cimag(sp_IR_LL[i]));
    }

    puts("--------------------------------------------------------------------------");
    #endif

    float *output_left  = (float*) calloc(2048, sizeof(float));
    float *output_right = (float*) calloc(2048, sizeof(float));
    float *temp_left    = (float*) calloc(1024, sizeof(float));
    float *temp_right   = (float*) calloc(1024, sizeof(float));
    float *in_left_fft  = (float*) calloc(2048, sizeof(float));
    float *in_right_fft = (float*) calloc(2048, sizeof(float));

    fftwf_complex *sp_in_left   = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * 1025);
    fftwf_complex *sp_in_right  = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * 1025);
    fftwf_complex *sp_out_left  = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * 1025);
    fftwf_complex *sp_out_right = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * 1025);

    /* Pack all the data processed in a sharedData structure */
    IRData.forward 			= forward;
    IRData.backward			= backward;
    IRData.sp_IR_LL			= sp_IR_LL;
    IRData.sp_IR_RL 		= sp_IR_RL;
    IRData.sp_IR_LR 		= sp_IR_LR;
    IRData.sp_IR_RR 		= sp_IR_RR;
    IRData.output_left		= output_left;
    IRData.output_right		= output_right;
    IRData.temp_left		= temp_left;
    IRData.temp_right		= temp_right;
    IRData.in_left_fft      = in_left_fft;
    IRData.in_right_fft     = in_right_fft;
    IRData.sp_in_left		= sp_in_left;
    IRData.sp_in_right		= sp_in_right;
    IRData.sp_out_left      = sp_out_left;
    IRData.sp_out_right     = sp_out_right;


    printf("Initialization OK!\n");
    puts("--------------------------------------------------------------------------");
    puts("");

    return IRData;
}


/***********************************************************************************/
/*******************************  BINAURALIZEIO ()  **********************************/
/***********************************************************************************/
void binauralizeIO (int inputIndex, int outputIndex, const char* path) {
    /* Initialization process */
    sharedData data = initIO(path);

    PaStreamParameters inputParameters, outputParameters;
    PaStream *stream;
    PaError err;
    
    /* Start PortAudio */
    err = Pa_Initialize();
    if(err != paNoError) goto error;
    
    /* Define input device */
    inputParameters.device = inputIndex;
    if (inputParameters.device == paNoDevice)
    {
        fprintf(stderr,"Error: No default input device.\n");
        goto error;
    }
    inputParameters.channelCount = 2;
    inputParameters.sampleFormat = PA_SAMPLE_TYPE;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device)->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = NULL;
    
    /* Define ouptut device */
    outputParameters.device = outputIndex;
    if (outputParameters.device == paNoDevice)
    {
        fprintf(stderr,"Error: No default output device.\n");
        goto error;
    }
    outputParameters.channelCount = 2;
    outputParameters.sampleFormat = PA_SAMPLE_TYPE;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;
    
    /* Create stream between input and output */
    printf("Streams opening...\n");
    err = Pa_OpenStream(&stream,
                        &inputParameters,
                        &outputParameters,
                        SAMPLE_RATE,
                        FRAMES_PER_BUFFER,
                        0,
                        multiStereoCallbackIO,
                        &data);
    if(err != paNoError) goto error;
    
    /* Start processing audio */
    err = Pa_StartStream(stream);
    if(err != paNoError) goto error;
    
    /* Wait until the user decides to stop program */
    printf("Hit ENTER to stop program.\n");
    getchar();
    
    /* Close stream */
    err = Pa_CloseStream(stream);
    if(err != paNoError) goto error;
    
    /* Terminate PortAudio */
    printf("Streams closed.\n");
    Pa_Terminate();
    closeFiles(data);
    exit(0);
    
error:
    Pa_Terminate();
    fprintf(stderr, "An error occured while using the portaudio stream\n");
    fprintf(stderr, "Error number: %d\n", err);
    fprintf(stderr, "Error message: %s\n", Pa_GetErrorText(err));
    closeFiles(data);
    exit(-1);
}

/***********************************************************************************/
/*********************************  INITFILE ()  ***********************************/
/***********************************************************************************/
sharedData initFile (const char *pathIR, const char* pathFile) {
    SF_INFO IRInfo, FileInfo;
    sharedData IRData;
    fftwf_plan forward, backward;

    SNDFILE *sndfileSource = sf_open(pathFile, SFM_READ, &FileInfo);
    if (sndfileSource == NULL) {
        puts("Impossible to open source file");
        puts("Read the documentation for more informations.");
        exit(-1);
    }
    else if (FileInfo.channels != 5)
    {
        puts("Bad file type");
        puts("Read the documentation for more informations.");
        exit(-1);
    }
    else{puts("Source file successfully opened");}

    float* source = (float*) calloc(FileInfo.channels * FileInfo.frames, sizeof(float));
    if (source == NULL)
    {
        puts("Error allocating memory, input file may be too big.");
    }
    sf_readf_float (sndfileSource, source, FileInfo.frames);
    puts("Source file successfully written");

    /* Defines the paths to IR files */
    char *path_IR_LL  = concatenate(pathIR,"/IR_LL.wav");
    char *path_IR_RL  = concatenate(pathIR,"/IR_RL.wav");
    char *path_IR_LC  = concatenate(pathIR,"/IR_LC.wav");
    char *path_IR_RC  = concatenate(pathIR,"/IR_RC.wav");
    char *path_IR_LR  = concatenate(pathIR,"/IR_LR.wav");
    char *path_IR_RR  = concatenate(pathIR,"/IR_RR.wav");
    char *path_IR_LRs = concatenate(pathIR,"/IR_LRs.wav");
    char *path_IR_RRs = concatenate(pathIR,"/IR_RRs.wav");
    char *path_IR_LLs = concatenate(pathIR,"/IR_LLs.wav");
    char *path_IR_RLs = concatenate(pathIR,"/IR_RLs.wav");


    /* Opens WAV files to SNDFILEs */
    SNDFILE *sndfile_IR_LL  = sf_open(path_IR_LL, SFM_READ, &IRInfo);
    if (sndfile_IR_LL == NULL) {
        puts("Impossible to open IR_LL");
        puts("Read the documentation for more informations.");
        exit(-1);
    }
    
    SNDFILE *sndfile_IR_RL  = sf_open(path_IR_RL, SFM_READ, &IRInfo);
    if (sndfile_IR_RL == NULL) {
        puts("Impossible to open IR_RL");
        puts("Read the documentation for more informations.");
        exit(-1);
    }

    SNDFILE *sndfile_IR_LC  = sf_open(path_IR_LC, SFM_READ, &IRInfo);
    if (sndfile_IR_LC == NULL) {
        puts("Impossible to open IR_LC");
        puts("Read the documentation for more informations.");
        exit(-1);
    }
    
    SNDFILE *sndfile_IR_RC  = sf_open(path_IR_RC, SFM_READ, &IRInfo);
    if (sndfile_IR_RC == NULL) {
        puts("Impossible to open IR_RC");
        puts("Read the documentation for more informations.");
        exit(-1);
    }
    SNDFILE *sndfile_IR_LR  = sf_open(path_IR_LR, SFM_READ, &IRInfo);
    if (sndfile_IR_LR == NULL) {
        puts("Impossible to open IR_LR");
        puts("Read the documentation for more informations.");
        exit(-1);
    }
    
    SNDFILE *sndfile_IR_RR  = sf_open(path_IR_RR, SFM_READ, &IRInfo);
    if (sndfile_IR_RR == NULL) {
        puts("Impossible to open IR_RR");
        puts("Read the documentation for more informations.");
        exit(-1);
    }

    SNDFILE *sndfile_IR_LRs  = sf_open(path_IR_LRs, SFM_READ, &IRInfo);
    if (sndfile_IR_LRs == NULL) {
        puts("Impossible to open IR_LRs");
        puts("Read the documentation for more informations.");
        exit(-1);
    }
    
    SNDFILE *sndfile_IR_RRs  = sf_open(path_IR_RRs, SFM_READ, &IRInfo);
    if (sndfile_IR_RRs == NULL) {
        puts("Impossible to open IR_RRs");
        puts("Read the documentation for more informations.");
        exit(-1);
    }
    SNDFILE *sndfile_IR_LLs  = sf_open(path_IR_LLs, SFM_READ, &IRInfo);
    if (sndfile_IR_LLs == NULL) {
        puts("Impossible to open IR_LLs");
        puts("Read the documentation for more informations.");
        exit(-1);
    }
    
    SNDFILE *sndfile_IR_RLs  = sf_open(path_IR_RLs, SFM_READ, &IRInfo);
    if (sndfile_IR_RLs == NULL) {
        puts("Impossible to open IR_RLs");
        puts("Read the documentation for more informations.");
        exit(-1);
    }

    free(path_IR_LL);
    free(path_IR_RL);
    free(path_IR_LC);
    free(path_IR_RC);
    free(path_IR_LR);
    free(path_IR_RR);
    free(path_IR_LRs);
    free(path_IR_RRs);
    free(path_IR_LLs);
    free(path_IR_RLs);


    puts("--------------------------------------------------------------------------");
    printf("Impulse response files correctly opened.\n");

    /* Process the FFT of IR files */
    float *IR_LL = (float*) calloc(2048, sizeof(float));
    float *IR_RL = (float*) calloc(2048, sizeof(float));

    float *IR_LC = (float*) calloc(2048, sizeof(float));
    float *IR_RC = (float*) calloc(2048, sizeof(float));

    float *IR_LR = (float*) calloc(2048, sizeof(float));
    float *IR_RR = (float*) calloc(2048, sizeof(float));

    float *IR_LRs = (float*) calloc(2048, sizeof(float));
    float *IR_RRs = (float*) calloc(2048, sizeof(float));

    float *IR_LLs = (float*) calloc(2048, sizeof(float));
    float *IR_RLs = (float*) calloc(2048, sizeof(float));



    sf_read_float(sndfile_IR_LL, IR_LL, 1024);
    sf_read_float(sndfile_IR_RL, IR_RL, 1024);

    sf_read_float(sndfile_IR_LC, IR_LC, 1024);
    sf_read_float(sndfile_IR_RC, IR_RC, 1024);

    sf_read_float(sndfile_IR_LR, IR_LR, 1024);
    sf_read_float(sndfile_IR_RR, IR_RR, 1024);

    sf_read_float(sndfile_IR_LRs, IR_LRs, 1024);
    sf_read_float(sndfile_IR_RRs, IR_RRs, 1024);

    sf_read_float(sndfile_IR_LLs, IR_LLs, 1024);
    sf_read_float(sndfile_IR_RLs, IR_RLs, 1024);

    sf_close(sndfile_IR_LL);
    sf_close(sndfile_IR_RL);

    sf_close(sndfile_IR_LC);
    sf_close(sndfile_IR_RC);

    sf_close(sndfile_IR_LR);
    sf_close(sndfile_IR_RR);

    sf_close(sndfile_IR_LRs);
    sf_close(sndfile_IR_RRs);

    sf_close(sndfile_IR_LLs);
    sf_close(sndfile_IR_RLs);


    fftwf_complex *sp_IR_LL = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * 1025);
    fftwf_complex *sp_IR_RL = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * 1025);

    fftwf_complex *sp_IR_LC = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * 1025);
    fftwf_complex *sp_IR_RC = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * 1025);

    fftwf_complex *sp_IR_LR = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * 1025);
    fftwf_complex *sp_IR_RR = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * 1025);

    fftwf_complex *sp_IR_LRs = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * 1025);
    fftwf_complex *sp_IR_RRs = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * 1025);

    fftwf_complex *sp_IR_LLs = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * 1025);
    fftwf_complex *sp_IR_RLs = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * 1025);

    float *fakeArray = (float*) calloc(2048, sizeof(float));
    fftwf_complex *sp_fakeArray = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * 1025);

    forward  = fftwf_plan_dft_r2c_1d(2048, fakeArray, sp_fakeArray, FFTW_PATIENT);
    backward = fftwf_plan_dft_c2r_1d(2048, sp_fakeArray, fakeArray, FFTW_PATIENT);

    free(fakeArray);
    free(sp_fakeArray);

    for (int i = 0; i < 1025; ++i)
    {
        sp_IR_LL[i]  = 0.0 + I * 0.0;
        sp_IR_RL[i]  = 0.0 + I * 0.0;

        sp_IR_LC[i]  = 0.0 + I * 0.0;
        sp_IR_RC[i]  = 0.0 + I * 0.0;

        sp_IR_LR[i]  = 0.0 + I * 0.0;
        sp_IR_RR[i]  = 0.0 + I * 0.0;

        sp_IR_LRs[i] = 0.0 + I * 0.0;
        sp_IR_RRs[i] = 0.0 + I * 0.0;

        sp_IR_LLs[i] = 0.0 + I * 0.0;
        sp_IR_RLs[i] = 0.0 + I * 0.0;
    }


    fftwf_execute_dft_r2c(forward, IR_LL, sp_IR_LL);
    fftwf_execute_dft_r2c(forward, IR_LR, sp_IR_LR);
    fftwf_execute_dft_r2c(forward, IR_LC, sp_IR_LC);
    fftwf_execute_dft_r2c(forward, IR_RC, sp_IR_RC);
    fftwf_execute_dft_r2c(forward, IR_LR, sp_IR_LR);
    fftwf_execute_dft_r2c(forward, IR_RR, sp_IR_RR);
    fftwf_execute_dft_r2c(forward, IR_LRs, sp_IR_LRs);
    fftwf_execute_dft_r2c(forward, IR_RRs, sp_IR_RRs);
    fftwf_execute_dft_r2c(forward, IR_LLs, sp_IR_LLs);
    fftwf_execute_dft_r2c(forward, IR_RLs, sp_IR_RLs);

    #if VERBOSE
    puts("");
    puts("--------------------------------------------------------------------------");
    puts("-----------------------  VERBOSE  MODE  CALLED  --------------------------");
    puts("--------------------------------------------------------------------------");

    puts("INPUT:");
    for (size_t i = 0; i < 8; ++i)
    {
        printf("%zu: %f\n", i, IR_LL[i]);
    }
    puts("");

    puts("OUTPUT:");
    for (size_t i = 0; i < 8; ++i)
    {
        printf("%zu: %f + i %f\n", i, creal(sp_IR_LL[i]), cimag(sp_IR_LL[i]));
    }

    puts("--------------------------------------------------------------------------");
    #endif

    float *output_left    = (float*) calloc(2048, sizeof(float));
    float *output_right   = (float*) calloc(2048, sizeof(float));
    float *temp_left      = (float*) calloc(1024, sizeof(float));
    float *temp_right     = (float*) calloc(1024, sizeof(float));
    float *in_left_fft    = (float*) calloc(2048, sizeof(float));
    float *in_center_fft  = (float*) calloc(2048, sizeof(float));
    float *in_right_fft   = (float*) calloc(2048, sizeof(float));
    float *in_right_s_fft = (float*) calloc(2048, sizeof(float));
    float *in_left_s_fft  = (float*) calloc(2048, sizeof(float));

    fftwf_complex *sp_in_left     = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * 1025);
    fftwf_complex *sp_in_center   = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * 1025);
    fftwf_complex *sp_in_right    = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * 1025);
    fftwf_complex *sp_in_right_s  = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * 1025);
    fftwf_complex *sp_in_left_s   = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * 1025);
    fftwf_complex *sp_out_left    = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * 1025);
    fftwf_complex *sp_out_right   = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * 1025);


    /* Pack all the data processed in a sharedData structure */
    IRData.forward          = forward;
    IRData.backward         = backward;
    IRData.sp_IR_LL         = sp_IR_LL;
    IRData.sp_IR_RL         = sp_IR_RL;
    IRData.sp_IR_LC         = sp_IR_LC;
    IRData.sp_IR_RC         = sp_IR_RC;
    IRData.sp_IR_LR         = sp_IR_LR;
    IRData.sp_IR_RR         = sp_IR_RR;
    IRData.sp_IR_LRs        = sp_IR_LRs;
    IRData.sp_IR_RRs        = sp_IR_RRs;
    IRData.sp_IR_LLs        = sp_IR_LLs;
    IRData.sp_IR_RLs        = sp_IR_RLs;

    IRData.output_left      = output_left;
    IRData.output_right     = output_right;
    IRData.temp_left        = temp_left;
    IRData.temp_right       = temp_right;
    IRData.in_left_fft      = in_left_fft;
    IRData.in_center_fft    = in_center_fft;
    IRData.in_right_fft     = in_right_fft;
    IRData.in_right_s_fft   = in_right_s_fft;
    IRData.in_left_s_fft    = in_left_s_fft;
    IRData.sp_in_left       = sp_in_left;
    IRData.sp_in_center     = sp_in_center;
    IRData.sp_in_right      = sp_in_right;
    IRData.sp_in_right_s    = sp_in_right_s;
    IRData.sp_in_left_s     = sp_in_left_s;
    IRData.sp_out_left      = sp_out_left;
    IRData.sp_out_right     = sp_out_right;
    IRData.source           = source;
    IRData.FileInfo         = FileInfo;


    printf("Initialization OK!\n");
    puts("--------------------------------------------------------------------------");
    puts("");

    return IRData;
}


/***********************************************************************************/
/*****************************  BINAURALIZEFILE ()  ********************************/
/***********************************************************************************/
void binauralizeFile (int outputIndex, const char* pathIR, const char* pathFile) {
    /* Initialization process */
    sharedData data = initFile(pathIR, pathFile);
    unsigned long duration = ((data.FileInfo.frames / SAMPLE_RATE) * 1000.0);
    int buffersRead = 0;
    data.buffersRead = buffersRead;

    PaStreamParameters outputParameters;
    PaStream *stream;
    PaError err;
    
    /* Start PortAudio */
    err = Pa_Initialize();
    if(err != paNoError) goto error;
    
    /* Define ouptut device */
    outputParameters.device = outputIndex;
    if (outputParameters.device == paNoDevice)
    {
        fprintf(stderr,"Error: No default output device.\n");
        goto error;
    }
    outputParameters.channelCount = 2;
    outputParameters.sampleFormat = PA_SAMPLE_TYPE;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;
    
    /* Create stream between input and output */
    printf("Stream opening...\n");
    err = Pa_OpenStream(&stream,
                        NULL,
                        &outputParameters,
                        SAMPLE_RATE,
                        FRAMES_PER_BUFFER,
                        0,
                        multiStereoCallbackFile,
                        &data);
    if(err != paNoError) goto error;
    
    puts("Stream opened");
    puts("Audio will start being processed");
    /* Start processing audio */
    err = Pa_StartStream(stream);
    if(err != paNoError) goto error;
    
    Pa_Sleep(duration);
    
    /* Close stream */
    err = Pa_CloseStream(stream);
    if(err != paNoError) goto error;
    
    /* Terminate PortAudio */
    printf("Streams closed.\n");
    Pa_Terminate();
    closeFiles(data);
    exit(0);
    
error:
    Pa_Terminate();
    fprintf(stderr, "An error occured while using the portaudio stream\n");
    fprintf(stderr, "Error number: %d\n", err);
    fprintf(stderr, "Error message: %s\n", Pa_GetErrorText(err));
    closeFiles(data);
    exit(-1);
}

/***********************************************************************************/
/****************************  MULTISTEREOCALLBACKIO ()  ***************************/
/***********************************************************************************/
int multiStereoCallbackIO(const void *inputBuffer, void *outputBuffer,
                        unsigned long framesPerBuffer,
                        const PaStreamCallbackTimeInfo* timeInfo,
                        PaStreamCallbackFlags statusFlags,
                        void *userData)
{
    sharedData *data = (sharedData*) userData;

    float *in_left  = ((float **) inputBuffer)[0];
    float *in_right = ((float **) inputBuffer)[1];
    
    float *out_left  = ((float **) outputBuffer)[0];
    float *out_right = ((float **) outputBuffer)[1];

    for (size_t i = 0; i < 1024; i++) {
        data->in_left_fft[i]  = in_left[i];
        data->in_right_fft[i] = in_right[i];
    }

    for (size_t i = 1024; i < 2048; i++) {
        data->in_left_fft[i]  = 0.0;
        data->in_right_fft[i] = 0.0;
    }

    fftwf_execute_dft_r2c(data->forward, data->in_left_fft,  data->sp_in_left);
    fftwf_execute_dft_r2c(data->forward, data->in_right_fft, data->sp_in_right);

    for (size_t i = 0; i < 1025; ++i)
    {
        data->sp_out_left[i]  = 0.500 * (data->sp_in_left[i]  * data->sp_IR_LL[i] + data->sp_in_left[i]  * data->sp_IR_LR[i]);
        data->sp_out_right[i] = 0.500 * (data->sp_in_right[i] * data->sp_IR_RL[i] + data->sp_in_right[i] * data->sp_IR_RR[i]);
    }

    fftwf_execute_dft_c2r(data->backward, data->sp_out_left,  data->output_left);
    fftwf_execute_dft_c2r(data->backward, data->sp_out_right, data->output_right);

    for (size_t i = 0; i < 2048; ++i)
    {
        data->output_left[i]  = data->output_left[i]/2048.0;
        data->output_right[i] = data->output_right[i]/2048.0;
    }


    
    unsigned int i;
    
    (void) timeInfo; /* Prevent unused variable warnings. */
    (void) statusFlags;
    (void) userData;
    for(i = 0; i < framesPerBuffer; i++)
    {
        out_left[i]   = 0.500 * (data->output_left[i] + data->temp_left[i]);
        out_right[i]  = 0.500 * (data->output_right[i] + data->temp_right[i]);
        data->temp_left[i]  = data->output_left[i + 1024];
        data->temp_right[i] = data->output_right[i + 1024];
    }
    
    return paContinue;
}

/***********************************************************************************/
/**************************  MULTISTEREOCALLBACKFILE ()  ***************************/
/***********************************************************************************/
int multiStereoCallbackFile(const void *inputBuffer, void *outputBuffer,
                        unsigned long framesPerBuffer,
                        const PaStreamCallbackTimeInfo* timeInfo,
                        PaStreamCallbackFlags statusFlags,
                        void *userData)
{
    //puts("Entered callback.");
    sharedData *data = (sharedData*) userData;
    
    float *out_left  = ((float **) outputBuffer)[0];
    float *out_right = ((float **) outputBuffer)[1];
    //puts("Outputs created.");
    for (size_t i = 0; i < 1024; i++) {
        data->in_left_fft[i]    = data->source[5 * (framesPerBuffer * data->buffersRead + i)];
        data->in_center_fft[i]  = data->source[5 * (framesPerBuffer * data->buffersRead + i) + 1];
        data->in_right_fft[i]   = data->source[5 * (framesPerBuffer * data->buffersRead + i) + 2];
        data->in_right_s_fft[i] = data->source[5 * (framesPerBuffer * data->buffersRead + i) + 3];
        data->in_left_s_fft[i]  = data->source[5 * (framesPerBuffer * data->buffersRead + i) + 4];
        //printf("%f\n", data->in_left_fft[i]);
        //printf("%f\n", data->in_center_fft[i]);
        //printf("%f\n", data->in_right_fft[i]);
        //printf("%f\n", data->in_right_s_fft[i]);
        //printf("%f\n", data->in_left_s_fft[i]);
    }

    //printf("%i\n", data->buffersRead);
    //printf("\n");

    for (size_t i = 1024; i < 2048; i++) {
        data->in_left_fft[i]    = 0.0;
        data->in_center_fft[i]  = 0.0;
        data->in_right_fft[i]   = 0.0;
        data->in_right_s_fft[i] = 0.0;
        data->in_left_s_fft[i]  = 0.0;
    }

    //puts("Will execute FFTs");
    fftwf_execute_dft_r2c(data->forward, data->in_left_fft,    data->sp_in_left);
    fftwf_execute_dft_r2c(data->forward, data->in_center_fft,  data->sp_in_center);
    fftwf_execute_dft_r2c(data->forward, data->in_right_fft,   data->sp_in_right);
    fftwf_execute_dft_r2c(data->forward, data->in_right_s_fft, data->sp_in_right_s);
    fftwf_execute_dft_r2c(data->forward, data->in_left_s_fft,  data->sp_in_left_s);

    if (data->buffersRead == 95)
    {
        for (int i = 0; i < 1024; ++i)
        {
            //printf("IN_CENTER %i: %f \n", i, data->in_center_fft[i]);
            //printf("SP_CENTER %i: %f + i %f\n",i, creal(data->sp_in_center[i]), cimag(data->sp_in_center[i]));
        }
    }


    for (size_t i = 0; i < 1025; ++i)
    {
        //printf("SP_LEFT %zu: %f + i %f\n", i, creal(data->sp_in_left[i]), cimag(data->sp_in_left[i]));
        //printf("SP_CENTER %zu: %f + i %f\n",i, creal(data->sp_in_center[i]), cimag(data->sp_in_center[i]));
        //printf("SP_RIGHT %zu: %f + i %f\n",i,  creal(data->sp_in_right[i]), cimag(data->sp_in_right[i]));
        //printf("SP_RIGHT_S %zu: %f + i %f\n",i, creal(data->sp_in_right_s[i]), cimag(data->sp_in_right_s[i]));
        //printf("SP_LEFT_S %zu: %f + i %f\n",i,  creal(data->sp_in_left_s[i]), creal(data->sp_in_left_s[i]));
        // if (data->buffersRead == 95)
        // {
        //  printf("SP_CENTER %zu: %f + i %f\n",i, creal(data->sp_in_center[i]*data->sp_IR_LC[i]), cimag(data->sp_in_center[i]*data->sp_IR_LC[i]));   
        // }
        data->sp_out_left[i]  = 0.20 * (data->sp_in_left[i]    * data->sp_IR_LL[i] +
                                        data->sp_in_center[i]  * data->sp_IR_LC[i] +
                                        data->sp_in_right[i]   * data->sp_IR_LR[i] +
                                        data->sp_in_right_s[i] * data->sp_IR_LRs[i]+
                                        data->sp_in_left_s[i]  * data->sp_IR_LLs[i]);
        data->sp_out_right[i] = 0.20 * (data->sp_in_left[i]    * data->sp_IR_RL[i] +
                                        data->sp_in_center[i]  * data->sp_IR_RC[i] +
                                        data->sp_in_right[i]   * data->sp_IR_RR[i] +
                                        data->sp_in_right_s[i] * data->sp_IR_RRs[i]+
                                        data->sp_in_left_s[i]  * data->sp_IR_RLs[i]);
        //printf("SP_IR_LEFT %zu: %f + i %f\n", i, creal(data->sp_IR_LL[i]), cimag(data->sp_IR_LL[i]));
    }

    // for (int i = 0; i < 1024; ++i)
    // {
    //     printf("SP_IN_LEFT %i:%i = %f + i %f\n", data->buffersRead, i, creal(data->sp_in_left[i]), cimag(data->sp_in_left[i]));
    //     printf("SP_IR_RL %i:%i = %f + i %f\n", data->buffersRead, i, creal(data->sp_IR_RL[i]), cimag(data->sp_IR_RL[i]));

    //     printf("SP_IN_CENTER %i:%i = %f + i %f\n", data->buffersRead, i, creal(data->sp_in_center[i]), cimag(data->sp_in_center[i]));
    //     printf("SP_IR_RC %i:%i = %f + i %f\n", data->buffersRead, i, creal(data->sp_IR_RC[i]), cimag(data->sp_IR_RC[i]));

    //     printf("SP_IN_RIGHT %i:%i = %f + i %f\n", data->buffersRead, i, creal(data->sp_in_right[i]), cimag(data->sp_in_right[i]));
    //     printf("SP_IR_RR %i:%i = %f + i %f\n", data->buffersRead, i, creal(data->sp_IR_RR[i]), cimag(data->sp_IR_RR[i]));

    //     printf("SP_IN_RIGHT_S %i:%i = %f + i %f\n", data->buffersRead, i, creal(data->sp_in_right_s[i]), cimag(data->sp_in_right_s[i]));
    //     printf("SP_IR_RRs %i:%i = %f + i %f\n", data->buffersRead, i, creal(data->sp_IR_RRs[i]), cimag(data->sp_IR_RRs[i]));

    //     printf("SP_IN_LEFT_S %i:%i = %f + i %f\n", data->buffersRead, i, creal(data->sp_in_left_s[i]), cimag(data->sp_in_left_s[i]));
    //     printf("SP_IR_RLs %i:%i = %f + i %f\n", data->buffersRead, i, creal(data->sp_IR_RLs[i]), cimag(data->sp_IR_RLs[i]));

    //     printf("OUTPUT_RIGHT %i:%i = %f\n", data->buffersRead, i, data->output_right[i]);
    // }

    fftwf_execute_dft_c2r(data->backward, data->sp_out_left,  data->output_left);
    fftwf_execute_dft_c2r(data->backward, data->sp_out_right, data->output_right);

    for (size_t i = 0; i < 2048; ++i)
    {
        data->output_left[i]  = data->output_left[i]/2048.0;
        data->output_right[i] = data->output_right[i]/2048.0;
    }

    
    (void) timeInfo; /* Prevent unused variable warnings. */
    (void) statusFlags;
    (void) userData;
    if (data->buffersRead * framesPerBuffer < data->FileInfo.frames)
    { 
        for(size_t i = 0; i < framesPerBuffer; i++)
        {
            //printf("OUTPUT_RIGHT %i:%zu = %f\n", data->buffersRead, i, data->output_right[i]);
            //printf("TEMP_RIGHT %i:%zu = %f\n", data->buffersRead, i, data->temp_right[i]);
            out_left[i]         = 1.00 * (data->output_left[i]  + data->temp_left[i]);
            out_right[i]        = 1.00 * (data->output_right[i] + data->temp_right[i]);
            data->temp_left[i]  = data->output_left[i + 1024];
            data->temp_right[i] = data->output_right[i + 1024];
        }
    }
    else
    {
        for (size_t i = 0; i < framesPerBuffer; i++)
        {
            out_left[i]   = 0.0;
            out_right[i]  = 0.0;
        }
    }
    data->buffersRead += 1;
    return paContinue;
}

/***********************************************************************************/
/***********************************  USAGE ()  ************************************/
/***********************************************************************************/
void usage (void) {
    puts("--------------------------------------------------------------------------");
    color("1");
    puts("Welcome in multistereo!");
    color("0");
    puts("");
    puts("");
    printf("To start \'binauralization\' process, please use the ");
    color("1;32");
    printf("\'-binauralize\'");
    color("0");
    printf(" flag\n");
    printf("followed by the index of the device that you want to use for input and output\n");
    printf("and the path to the folder containing audio files for impulse responses.\n");
    puts("Also make sure that your IR are correctly named, \'IR_LRs.wav\' for the impulse response");
    puts("of the surround right channel in the left ear.");
    puts("Please make sure that the input/output devices have enough channels to be used.");
    printf("To use default input and output devices, use the flag");
    color("1;32");
    printf("\'-binauralize default\'\n");
    color("0");
    puts("");
    puts("");  
    printf("To get a list of available devices with their index, use the flag ");
    color("1;32");
    printf("\'-devs\'\n");
    color("0");
    puts("");
    puts(""); 
    printf("Multistereo can also work with files. Just use the flag");
    color("1;32");
    printf("\'-binauralize file\'\n");
    color("0");
    puts("followed by the path to your IR folder and the path to your multichannel  5.1 audio file.");
    puts("--------------------------------------------------------------------------");
}


/***********************************************************************************/
/*******************************  LISTDEVICES ()  **********************************/
/***********************************************************************************/
void listDevices (void) {
    int     i, numDevices, defaultDisplayed;
    const   PaDeviceInfo *deviceInfo;
    PaError err;
    
    /* Start PortAudio */
    err = Pa_Initialize();
    if(err != paNoError)
    {
        printf("ERROR: Pa_Initialize returned 0x%x\n", err);
        goto error;
    }
    
    /* Get the number of audio devices */
    numDevices = Pa_GetDeviceCount();
    if(numDevices < 0)
    {
        printf("ERROR: Pa_GetDeviceCount returned 0x%x\n", numDevices);
        err = numDevices;
        goto error;
    }
    
    printf("%d audio devices found\n", numDevices);
    for(i = 0; i < numDevices; i++)
    {
        deviceInfo = Pa_GetDeviceInfo(i);
        printf("--------------------------------------- device #%d\n", i);
        
        defaultDisplayed = 0;
        if(i == Pa_GetDefaultInputDevice())
        {
            printf("[ Default Input");
            defaultDisplayed = 1;
        }
        else if(i == Pa_GetHostApiInfo(deviceInfo->hostApi)->defaultInputDevice)
        {
            const PaHostApiInfo *hostInfo = Pa_GetHostApiInfo(deviceInfo->hostApi);
            printf("[ Default %s Input", hostInfo->name);
            defaultDisplayed = 1;
        }
        
        if(i == Pa_GetDefaultOutputDevice())
        {
            printf((defaultDisplayed ? "," : "["));
            printf(" Default Output");
            defaultDisplayed = 1;
        }
        else if(i == Pa_GetHostApiInfo(deviceInfo->hostApi)->defaultOutputDevice)
        {
            const PaHostApiInfo *hostInfo = Pa_GetHostApiInfo(deviceInfo->hostApi);
            printf((defaultDisplayed ? "," : "["));
            printf(" Default %s Output", hostInfo->name);
            defaultDisplayed = 1;
        }
        
        if(defaultDisplayed)
            printf(" ]\n");
        
        /* Print device info */
        printf("Name                        = %s\n", deviceInfo->name);
        printf("Host API                    = %s\n",  Pa_GetHostApiInfo(deviceInfo->hostApi)->name);
        printf("Max inputs = %d", deviceInfo->maxInputChannels);
        printf(", Max outputs = %d\n", deviceInfo->maxOutputChannels);
        printf("Default low input latency   = %8.4f\n", deviceInfo->defaultLowInputLatency);
        printf("Default low output latency  = %8.4f\n", deviceInfo->defaultLowOutputLatency);
        printf("Default high input latency  = %8.4f\n", deviceInfo->defaultHighInputLatency);
        printf("Default high output latency = %8.4f\n", deviceInfo->defaultHighOutputLatency);
        printf("Default sample rate         = %8.2f\n", deviceInfo->defaultSampleRate);
    }
    
    Pa_Terminate();
    
    puts("----------------------------------------------");
    exit(0);
    
error:
    exit(-1);
}


/***********************************************************************************/
/********************************  CLOSEFILES ()  **********************************/
/***********************************************************************************/
void closeFiles (sharedData data) {
	free(data.sp_IR_LL);
	free(data.sp_IR_LR);
	free(data.sp_IR_RL);
	free(data.sp_IR_RR);

	free(data.sp_in_left);
	free(data.sp_in_right);
	free(data.sp_out_left);
	free(data.sp_out_right);

	free(data.output_left);
	free(data.output_right);
	free(data.temp_left);
	free(data.temp_right);

	fftwf_destroy_plan(data.forward);
	fftwf_destroy_plan(data.backward);

    fftwf_cleanup();
}


/***********************************************************************************/
/*******************************  CONCATENATE ()  **********************************/
/***********************************************************************************/
char* concatenate(const char *string1, const char *string2)
{
    char *out = malloc(strlen(string1)+strlen(string2)+1);
    if (out == NULL) {
        printf("Problem concatenating input strings\n");
        exit(-1);
    }
    strcpy(out, string1);
    strcat(out, string2);
    return out;
}