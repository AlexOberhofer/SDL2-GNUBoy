/*
 * sdl.c
 * sdl 2 interfaces -- based on sdl.c 
 *
 * (C) 2001 Damian Gryski <dgryski@uwaterloo.ca>
 * (C) 2020 Alex Oberhofer <alexmoberhofer@gmail.com>
 * Joystick code contributed by David Lau
 *
 * Licensed under the GPLv2, or later.
 */

#include <stdlib.h>
#include <stdio.h>

#include <SDL2/SDL.h>

#include "fb.h"
#include "rc.h"


SDL_Surface *surface;
SDL_Window *window;
SDL_Surface *window_surface;
SDL_Texture *texture;
SDL_Renderer *renderer;
int fullscreen = 1;

struct fb fb;

static int vmode[3] = { 0, 0, 16 };

static byte pix[160 * 144 * 4];

void vid_init()
{
	printf("vid_init\n");
	int window_scale = 0;

	if (!vmode[0] || !vmode[1])
	{
		int scale = rc_getint("scale");
		fullscreen = rc_getint("fullscreen");
		if (scale < 1) scale = 1;
		window_scale = scale;
		vmode[0] = 160;
		vmode[1] = 144;
	}

	uint32_t flags;

	if(fullscreen) {flags |= SDL_WINDOW_FULLSCREEN; }

	//joy_init();

	if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
        printf("SDL_Init failed: %s\n", SDL_GetError());
        exit(1);
    } else {
        window = SDL_CreateWindow("SDL2 GNUBoy", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, vmode[0] * window_scale, vmode[1] * window_scale, flags);
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, vmode[0], vmode[1]);
    }

	fb.w = vmode[0];
	fb.h = vmode[1];
	fb.pelsize = 4;
	fb.pitch = fb.w * fb.pelsize;
	fb.indexed = fb.pelsize == 1;
	fb.ptr = pix;
	fb.cc[0].r = fb.cc[1].r = fb.cc[2].r = fb.cc[3].r = 0;
	fb.cc[0].l = 16;
	fb.cc[1].l = 8;
	fb.cc[2].l = 0;
	fb.cc[3].l = 0;

	fb.enabled = 1;
	fb.dirty = 0;

	joy_init();
}

void vid_setpal(int i, int r, int g, int b)
{

}

void vid_preinit()
{
}

void vid_close()
{
	joy_close();
	SDL_DestroyWindow(window);
	SDL_DestroyTexture(texture);
	SDL_Quit();
}

void vid_settitle(char *title)
{
	
}

void vid_begin()
{
	fb.ptr = pix;
}

void vid_end()
{
	fb.ptr = pix;
	SDL_RenderClear(renderer);
	SDL_UpdateTexture(texture, NULL, pix, vmode[0] * 4);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);	
}





