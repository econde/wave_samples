/*
 * Mostra parâmetros da codificação WAVE
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "wave.h"

int main(int argc, char *argv[]) {
	if (argc != 2) {
		fprintf(stderr, "usage: %s <wave filename>", argv[0]);
		return -1;
	}

	Wave *wave = wave_load(argv[1]);
	if (wave == NULL) {
		fprintf(stderr, "Error loading file \"%s\"\n", argv[1]);
		return -1;
	}

	printf("Número de canais: %d\n", wave_get_number_of_channels(wave));
	printf("Bits por amostra: %d\n", wave_get_bits_per_sample(wave));
	printf("Ritmo de amostragem: %d\n", wave_get_sample_rate(wave));
	printf("Duração: %zd\n", wave_get_duration(wave));

	wave_destroy(wave);
}
