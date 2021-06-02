/*
 * sdl-audio.c
 * sdl 2 audio interface
 *
 * (C) 2001 Laguna
 * (C) 2020 Alex Oberhofer <alexmoberhofer@gmail.com>
 * 
 * Contributors:
 *  - Ryzee119 - SDL Fixes
 *
 * Licensed under the GPLv2, or later.
 */

#include <stdlib.h>
#include <stdio.h>

#include <SDL2/SDL.h>

#include "rc.h"
#include "pcm.h"

struct pcm pcm;

#ifndef SOUND
#define SILENT
#endif

#ifdef SILENT
static int sound = 0;
#endif

#ifdef SOUND
static int sound = 1;
#endif
static int samplerate = 44100;
static int stereo = 1;
static volatile int audio_done;
SDL_AudioDeviceID device;

rcvar_t pcm_exports[] =
	{
#ifdef SOUND
		RCV_BOOL("sound", &sound),
#endif
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
	SDL_AudioSpec want, obtained;

	SDL_InitSubSystem(SDL_INIT_AUDIO);

	want.freq = samplerate;
	want.format = AUDIO_U8;
	want.channels = 1 + stereo;
	want.samples = samplerate / 60;
	for (i = 1; i < want.samples; i <<= 1)
		;
	want.samples = i;
	want.callback = audio_callback;
	want.userdata = 0;
	device = SDL_OpenAudioDevice(NULL, 0, &want, &obtained, 0);

	pcm.hz = obtained.freq;
	pcm.stereo = obtained.channels - 1;
	pcm.len = obtained.size;
	pcm.buf = malloc(pcm.len);
	pcm.pos = 0;
	memset(pcm.buf, 0, pcm.len);
#endif
	SDL_PauseAudioDevice(device, 0);
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

	return 1;
#endif

#ifdef SILENT
	return 0;
#endif
	
}

void pcm_close()
{
#ifdef SOUND
	if (sound)
		SDL_CloseAudioDevice(device);
#endif
}
