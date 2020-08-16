/*
 * sdl-audio.c
 * sdl 2 audio interface
 *
 * (C) 2001 Laguna
 * (C) 2020 Alex Oberhofer <alexmoberhofer@gmail.com>
 *
 * Licensed under the GPLv2, or later.
 */

#include <stdlib.h>
#include <stdio.h>

#include <SDL2/SDL.h>

#include "rc.h"
#include "pcm.h"

struct pcm pcm;

#ifdef SILENT
static int sound = 0;
#endif

#ifdef SOUND
static int sound = 1;
#endif
static int samplerate = 44100;
static int stereo = 1;
static volatile int audio_done;

rcvar_t pcm_exports[] =
	{
		RCV_BOOL("sound", &sound),
		RCV_INT("stereo", &stereo),
		RCV_INT("samplerate", &samplerate),
		RCV_END};

static void audio_callback(void *blah, byte *stream, int len)
{
#ifdef SOUND
	memcpy(stream, pcm.buf, len);
#endif
	audio_done = 1;
}

void pcm_init()
{
#ifdef SOUND
	int i;
	SDL_AudioSpec as;

	SDL_InitSubSystem(SDL_INIT_AUDIO);

	as.freq = samplerate;
	as.format = AUDIO_U8;
	as.channels = 1 + stereo;
	as.samples = samplerate / 60;
	for (i = 1; i < as.samples; i <<= 1)
		;
	as.samples = i;
	as.callback = audio_callback;
	as.userdata = 0;
	if (SDL_OpenAudio(&as, 0) == -1)
		return;

	pcm.hz = as.freq;
	pcm.stereo = as.channels - 1;
	pcm.len = as.size;
	pcm.buf = malloc(pcm.len);
	pcm.pos = 0;
	memset(pcm.buf, 0, pcm.len);
#endif
	SDL_PauseAudio(0);
}

int pcm_submit()
{
#ifdef SOUND
	if (!pcm.buf)
		return 0;
	if (pcm.pos < pcm.len)
		return 1;
	while (!audio_done)
		SDL_Delay(4);
	audio_done = 0;
	pcm.pos = 0;
#endif
	return 1;
}

void pcm_close()
{
#ifdef SOUND
	if (sound)
		SDL_CloseAudio();
#endif
}
