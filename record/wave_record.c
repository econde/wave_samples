#include <stdio.h>
#include <stdlib.h>
#include <alsa/asoundlib.h>

#include "wave.h"
#include "glib.h"

static int capture_run;

static const snd_pcm_sframes_t period_size = 64;

#if 0
static void print_samples(uint8_t *buffer, snd_pcm_sframes_t nframes, int channels) {
	int16_t *p = (int16_t *)buffer;
	for (snd_pcm_sframes_t i = 0; i < nframes; p += channels) {
		printf("%d", *p);
		if (channels == 2)
			printf("|%d", *(p + 1));
		putchar(' ');
		if ((++i % 16) == 0)
			putchar('\n');
	}
	putchar('\n');
}
#endif

typedef struct {
	Wave *wave;
	int id;
} Sound;

static GList *sounds = NULL;

static int sound_id;

static Wave *wave;

static int capture() {
	wave = wave_create(16, 1);
	if (wave == NULL) {
		fprintf(stderr, "Out of memory\n");
		exit(EXIT_FAILURE);
	}
	wave_set_sample_rate(wave, 44100);
	snd_pcm_t *handle;
	int result = snd_pcm_open (&handle, "default", SND_PCM_STREAM_CAPTURE, 0);
	if (result < 0) {
		fprintf (stderr, "cannot open audio device %s (%s)\n",
			 "default",
			 snd_strerror (result));
			 exit(EXIT_FAILURE);
	}
	result = snd_pcm_set_params(handle,
					  SND_PCM_FORMAT_S16_LE,
					  SND_PCM_ACCESS_RW_INTERLEAVED,
					  wave_get_number_of_channels(wave),
					  wave_get_sample_rate(wave),
					  1,
					  500000);   /* 0.5 sec */
	if (result < 0) {
		fprintf(stderr, "snd_pcm_set_params: %s\n",
			snd_strerror(result));
			exit(EXIT_FAILURE);
    }
	result = snd_pcm_prepare(handle);
	if (result < 0) {
		fprintf (stderr, "cannot prepare audio interface for use (%s)\n",
			 snd_strerror(result));
		exit(EXIT_FAILURE);
	}

	int frame_size = snd_pcm_frames_to_bytes(handle, 1);
	char buffer[period_size * frame_size];
	int frame_index = 0;
	while (capture_run) {
		snd_pcm_sframes_t read_frames =
			snd_pcm_readi(handle, buffer, period_size);
		if (read_frames < 0) {
			fprintf (stderr, "read from audio interface failed (%s)\n",
					snd_strerror(read_frames));
			exit(EXIT_FAILURE);
		}

		wave_put_samples(wave, frame_index, buffer, read_frames);
		frame_index += read_frames;
		fputc('.', stderr);
		//print_samples(buffer, read_frames, wave_get_number_of_channels(wave));
	}
	snd_pcm_close (handle);
	return 0;
}

static int sound_match(const void *data, const void *ctx) {
	return ((Sound *)data)->id - (int) (long) ctx;
}

void save(char *filename) {
	GList *node = g_list_find_custom(sounds, 0, sound_match);
	if (node != NULL)
		wave_store(((Sound*)node->data)->wave, filename);
}

static void sound_print(void *data, void *ctx) {
	Sound *sound = (Sound *)data;
	printf("[%d] - %zd seconds\n", sound->id, wave_get_duration(sound->wave));
}

void list() {
	g_list_foreach(sounds, sound_print, NULL);
}

void delete() {
}

#if 1

#include <threads.h>

static thrd_t thread_capture_id;

void start_capture() {
	capture_run = 1;

	int result = thrd_create(&thread_capture_id, capture, NULL);
	if (result != thrd_success) {
		fprintf(stderr, "create capture error");
		exit(-1);
	}
	printf("thread capture started\n");
}

void stop_capture() {
	capture_run = 0;
	int result = thrd_join(thread_capture_id, NULL);
	if (result != thrd_success) {
		fprintf(stderr, "join thread error");
		exit(-1);
	}
	wave_format_update(wave);
	Sound *sound = malloc(sizeof *sound);
	sound->id = sound_id++;
	sound->wave = wave;
	sounds = g_list_append(sounds, sound);
	printf("thread capture stoped\n");
}

int main(int argc, char *argv[]) {
	int run = 1;
	while (run) {
		static char line[100];
		putchar('>');
		fgets(line, sizeof line, stdin);
		char *command = strtok(line, " \t\n");
		if (command == NULL)
			continue;
		char *argument = strtok(NULL, " \t\n");
		if (strcmp (command, "start") == 0)
			start_capture();
		if (strcmp (command, "stop") == 0)
			stop_capture();
		if (strcmp (command, "save") == 0)
			save(argument);
		if (strcmp (command, "delete") == 0)
			delete(argument);
		if (strcmp (command, "list") == 0)
			list(argument);
		if (strcmp (command, "exit") == 0)
			run = 0;
	}
}

#endif
