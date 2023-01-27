/*
 * Mostra parâmetros da codificação WAVE
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <getopt.h>
#include "wave.h"

static void help(char *prog_name) {
	printf("Usage: %s [options] <file samples 1> <file samples 2> ...\n"
		"options:\n"
		"\t--verbose\n"
		"\t-h, --help\n"
		"\t-v, --version\n"
		"\t-i, --input <file name>\n"
		"\t-o, --output <file name>\n"
		"\t-c, --channels <number of channels>\n"
		"\t-f, --format <S16_LE | WAVE>\n"
		"\t-r, --sample_rate <rate>\n",
		prog_name);
}

#define CONFIG_VERSION "0.00"

static void about() {
	printf("join v" CONFIG_VERSION " (" __DATE__ ")\n"
		"Ezequiel Conde (ezeq@cc.isel.ipl.pt)\n");
}

int main(int argc, char *argv[]) {
	static int verbose_flag = false;
	static struct option long_options[] = {
		{"verbose", no_argument, &verbose_flag, 1},
		{"help", no_argument, 0, 'h'},
		{"version", no_argument, 0, 'v'},
		{"output", required_argument, 0, 'o'},
		{"channels", required_argument, 0, 'c'},
		{"format", required_argument, 0, 'f'},
		{"rate", required_argument, 0, 'r'},
		{0, 0, 0, 0}
	};

	int option_index, option_char;
	int error_in_options = false;

	char *option_input_filename = NULL;
	char *option_output_filename = NULL;
	char *option_sample_rate = NULL;
	char *option_format = NULL;
	char *option_channels = NULL;

	while ((option_char = getopt_long(argc, argv, ":hvi:o:f:r:d:l:t:c:g:",
			long_options, &option_index)) != -1) {
		switch (option_char) {
		case 0:	//	Opções longas com afetação de flag
			break;
		case 'h':
			help(argv[0]);
			return 0;
		case 'v':
			about();
			break;
		case 'i':
			option_input_filename = optarg;
			break;
		case 'o':
			option_output_filename = optarg;
			break;
		case 'f':
			option_format = optarg;
			break;
		case 'c':
			option_channels = optarg;
			break;
		case 'r':
			option_sample_rate = optarg;
			break;
		case ':':
			fprintf(stderr, "Error in option -%c argument\n", optopt);
			error_in_options = true;
			break;
		case '?':
			error_in_options = true;
			break;
		}
	}
	if (error_in_options) {
		help(argv[0]);
		exit(EXIT_FAILURE);
	}

	int channels = 1;
	if (option_channels != NULL)
		channels = atoi(option_channels);

	int sample_rate = 48000;
	if (option_sample_rate != NULL)
		sample_rate = atoi(option_sample_rate);

	if (option_input_filename != NULL)
		;

	if (verbose_flag) {
		printf("channels = %d\n", channels);
		printf("sample rate = %d\n", sample_rate);
		printf("output filename = %s\n", option_output_filename);
		for (int i = 0; i < channels; i++)
			printf("input filename [%d] = %s\n", i, argv[optind + i]);
	}

	Wave *out = wave_create(16, channels);
	wave_set_sample_rate(out, sample_rate);

	if (strcmp(option_format, "S16_LE") == 0) {
		unsigned bytes_per_sample = 2;
		FILE *fd[channels];
		for (int i = 0; i < channels; ++i) {
			fd[i] = fopen(argv[optind + i], "r");
			if (fd[i] == NULL) {
				fprintf(stderr, "fopen(%s, \"r\") error: %s\n", argv[optind + i], strerror(errno));
				exit(EXIT_FAILURE);
			}
		}
		while (true) {
			char framme[channels * bytes_per_sample];
			char *framme_ptr = framme;
			for (int i = 0; i < channels; ++i) {
				size_t result = fread(framme_ptr, bytes_per_sample, 1, fd[i]);
				if (result < 1)
					goto break2;
				framme_ptr += bytes_per_sample;
			}
			wave_append_samples(out, framme, 1);
		}
		break2:;
	}
	else if (strcmp(option_format, "WAVE") == 0) {

	}
	else {
		fprintf(stderr, "Unknown format: %s\n", option_format);
		exit(EXIT_FAILURE);
	}
	wave_format_update(out);
	wave_store(out, option_output_filename);
}


