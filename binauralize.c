#include "binauralize.h"

float conv (float input, int index) {
	return 0.0;
}


/*float Binauralize( float input, int index ){

	float output;

	// Load impulse responses
	FILE *IR_LL  = NULL;
    FILE *IR_LR  = NULL;
    FILE *IR_LC  = NULL;
    FILE *IR_LLs = NULL;
    FILE *IR_LRs = NULL;

    IR_LL  = read_wave("test_IR/IR_LL.wav", "r");
    IR_LR  = read_wave("test_IR/IR_LR.wav", "r");
    IR_LC  = read_wave("test_IR/IR_LC.wav", "r");
    IR_LLs = read_wave("test_IR/IR_LLs.wav", "r");
    IR_LRs = read_wave("test_IR/IR_LRs.wav", "r");

	fclose(IR_LL);
	fclose(IR_LR);
	fclose(IR_LC);
	fclose(IR_LLs);
	fclose(IR_LRs);

	return output;
}*/

/*SNDFILE* read_wave(char* filename, SF_INFO* info) {
  SNDFILE* snd;
  info->format = 0;
  snd = sf_open(filename, SFM_READ, info);
  if (!snd) { fprintf(stderr, "%s: %s\n", filename, sf_strerror(snd)); exit(EXIT_FAILURE); }
  printf("%s: %d - %d channel\n", filename, info->samplerate, info->channels);
  return snd;
}

double* read_whole_buffer(SNDFILE* snd, SF_INFO* info) {
  sf_count_t count = info_n(info);
  double* buffer = malloc(sizeof(double) * count);
  sf_read_double(snd, buffer, count);
  return buffer;
}

double fabs(double n) {
  return (n < 0.0) ? -n : n;
}

void convolve_wave(char* infile, char* iirfile, char* outfile) {
  
// Crée deux fichiers d'infos sndfile WAV
  SF_INFO in_info, iir_info;

// Ouvre le fichier in et l'IR dans un type SNDFILE
  SNDFILE* in_snd = read_wave(infile, &in_info);
  SNDFILE* iir_snd = read_wave(iirfile, &iir_info);

// Initialise deux buffers de la taile du fichier d'entrée et de l'IR
  double* in_buffer = read_whole_buffer(in_snd, &in_info);
  double* iir_buffer = read_whole_buffer(iir_snd, &iir_info);


// Longueurs des fichiers ?
  sf_count_t in_len = info_n(&in_info);
  sf_count_t iir_len = info_n(&iir_info);
  sf_count_t out_len = in_len + iir_len - 1;


// Alloue un espace de la bonne taille pour la sortie
  double* out = malloc(sizeof(double) * out_len);


  // convolution
  printf("Convolution.\n");
  double val, max;
  sf_count_t low, high, i, j; // Crée 4 variables de type sf_count_t pour les différentes bornes des sommes

  // BEWARE OFF BY ONE ERRORS
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
}*/
