#include "binauralize.h"

/* init() is called when the program starts, before streams are
// opened by portaudio.
// It is intended to load all impulse responses in RAM for being used
// after in the callback function */
float* init (void) {
    SF_INFO *IR_info;
    IR_info->samplerate = 48000;
    IR_info->channels = 1;
    IR_info->format = 0;


    SOUNDFILE *IR_LL  = NULL;
    SOUNDFILE *IR_LR  = NULL;
    SOUNDFILE *IR_LC  = NULL;
    SOUNDFILE *IR_LLs = NULL;
    SOUNDFILE *IR_LRs = NULL;

    SOUNDFILE *IR_RL  = NULL;
    SOUNDFILE *IR_RR  = NULL;
    SOUNDFILE *IR_RC  = NULL;
    SOUNDFILE *IR_RLs = NULL;
    SOUNDFILE *IR_RRs = NULL;

    IR_LL  = sf_open("test_IR/IR_LL.wav" , SFM_READ, IR_info);
    IR_LR  = sf_open("test_IR/IR_LR.wav" , SFM_READ, IR_info);
    IR_LC  = sf_open("test_IR/IR_LC.wav" , SFM_READ, IR_info);
    IR_LLs = sf_open("test_IR/IR_LLs.wav", SFM_READ, IR_info);
    IR_LRs = sf_open("test_IR/IR_LRs.wav", SFM_READ, IR_info);

    IR_RL  = sf_open("test_IR/IR_RL.wav" , SFM_READ, IR_info);
    IR_RR  = sf_open("test_IR/IR_RR.wav" , SFM_READ, IR_info);
    IR_RC  = sf_open("test_IR/IR_RC.wav" , SFM_READ, IR_info);
    IR_RLs = sf_open("test_IR/IR_RLs.wav", SFM_READ, IR_info);
    IR_RRs = sf_open("test_IR/IR_RRs.wav", SFM_READ, IR_info);

    SOUNDFILE* IR_array = [IR_LL, IR_LR, IR_LC, IR_LLs, IR_LRs
                      IR_RL, IR_RR, IR_RC, IR_RLs, IR_RRs];

    return IR_array;
}

/* exit() is called after stream is closed by portaudio.
It closes all the IR files opened by init() */
void exit (void) {
  sf_close(IR_LL);
  sf_close(IR_LR);
  sf_close(IR_LC);
  sf_close(IR_LLs);
  sf_close(IR_LRs);

  sf_close(IR_RL);
  sf_close(IR_RR);
  sf_close(IR_RC);
  sf_close(IR_RLs);
  sf_close(IR_RRs);
}

float conv (float input, SOUNDFILE IR) {
	double* out = malloc(sizeof(double) * 512);
}

/*double fabs(double n) {
  return (n < 0.0) ? -n : n;
}*/

void convolve_wave(char* infile, char* iirfile, char* outfile) {
  
// Crée deux fichiers d'infos sndfile WAV
  //SF_INFO in_info, iir_info;

// Ouvre le fichier in et l'IR dans un type SNDFILE
  //SNDFILE* in_snd = sf_open(infile, &in_info);
  //SNDFILE* iir_snd = sf_open(iirfile, &iir_info);

// Initialise deux buffers de la taile du fichier d'entrée et de l'IR
  //double* in_buffer = read_whole_buffer(in_snd, &in_info);
  //double* iir_buffer = read_whole_buffer(iir_snd, &iir_info);


// Longueurs des fichiers ?
  sf_count_t in_len = info_n(&in_info);
  sf_count_t iir_len = info_n(&iir_info);
  sf_count_t out_len = in_len + iir_len - 1;


// Alloue un espace de la bonne taille pour la sortie
  double* out = malloc(sizeof(double) * out_len);


  // convolution
  printf("Convolution.\n");
  double val, max;

// Crée 4 variables de type sf_count_t pour les différentes bornes des sommes
  sf_count_t low, high, i, j;

  max = 0.0;
  for (i = 0; i < out_len; i++) { // Dans notre cas, pour i allant de 0 à 1023
    low = i - iir_len + 1; // low va de -1023 à 0
    low = low > 0 ? low : 0; // Check si low est positif; si oui, on renvoie low, sinon 0; pour nous ce sera toujours 0

    high = i; // high va de 0 à 1023
    high = high > (in_len-1) ? (in_len-1) : high; // Check si high est plus grand que 1023; si oui, on renvoie 1023, sinon renvoie high; renverra toujours high pr ns

    val = 0;
    for (j = low; j <= high; j++) { // Pour j allant de 0 à high, avec high variant de 0 à 1023
      sf_count_t k = (i-low)-(j-low);
      //assert(j >= 0 && j < in_len);
      //assert(k >= 0 && k < iir_len);
      val += in_buffer[j] * iir_buffer[k];
    }

    if (fabs(val) > max) max = fabs(val); // si l'abs de la sortie est plus grand que le max, assigne le max à la val
    out[i] = val; // Renvoie val
  }

// Libère tous les buffers
  free(in_buffer);
  free(iir_buffer);

// Ferme les fichiers audio
  sf_close(in_snd);
  sf_close(iir_snd);
}
