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
#include "input.h"
#include "rc.h"


SDL_Surface *surface;
SDL_Window *window;
SDL_Surface *window_surface;
SDL_Texture *texture;
SDL_Renderer *renderer;

struct fb fb;

static int vmode[3] = { 0, 0, 16 };

static byte pix[160 * 144 * 4];

void vid_init()
{
	int window_scale = 0;

	if (!vmode[0] || !vmode[1])
	{
		int scale = rc_getint("scale");
		if (scale < 1) scale = 1;
		window_scale = scale;
		vmode[0] = 160;
		vmode[1] = 144;
	}


	if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
        printf("SDL_Init failed: %s\n", SDL_GetError());
        exit(1);
    } else {
        window = SDL_CreateWindow("SDL2 GNUBoy", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, vmode[0] * window_scale, vmode[1] * window_scale, NULL);
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

}


/* TODO: Really need to clean this up - this will break if you rebind the keys.*/
void ev_poll()
{
	event_t ev;
	SDL_Event event;

	while (SDL_PollEvent(&event)){

    	if (event.type == SDL_QUIT){
        	exit(1);
    	}
    

	if(event.type == SDL_KEYDOWN){

        uint32_t key = event.key.keysym.scancode;

        switch(key)
        {
            case SDL_SCANCODE_RETURN: ev.type = EV_PRESS; ev.code = K_ENTER; ev_postevent(&ev); break;  
			case SDL_SCANCODE_A: ev.type = EV_PRESS; ev.code = K_LEFT; ev_postevent(&ev); break; 
			case SDL_SCANCODE_D: ev.type = EV_PRESS; ev.code = K_RIGHT; ev_postevent(&ev); break; 
			case SDL_SCANCODE_S: ev.type = EV_PRESS; ev.code = K_DOWN; ev_postevent(&ev); break; 
			case SDL_SCANCODE_W: ev.type = EV_PRESS; ev.code = K_UP; ev_postevent(&ev); break; 
			case SDL_SCANCODE_Q: ev.type = EV_PRESS; ev.code = 'q'; ev_postevent(&ev); break;
			case SDL_SCANCODE_E: ev.type = EV_PRESS; ev.code = 'e'; ev_postevent(&ev); break;  
        }

    } else if (event.type == SDL_KEYUP){
        
        uint32_t key = event.key.keysym.scancode;
        
        switch(key)
        {
            case SDL_SCANCODE_RETURN: ev.type = EV_RELEASE; ev.code = K_ENTER; ev_postevent(&ev); break; 
			case SDL_SCANCODE_A: ev.type = EV_RELEASE; ev.code = K_LEFT; ev_postevent(&ev); break; 
			case SDL_SCANCODE_D: ev.type = EV_RELEASE; ev.code = K_RIGHT; ev_postevent(&ev); break;
			case SDL_SCANCODE_S: ev.type = EV_RELEASE; ev.code = K_DOWN; ev_postevent(&ev); break;
			case SDL_SCANCODE_W: ev.type = EV_RELEASE; ev.code = K_UP; ev_postevent(&ev); break;   
			case SDL_SCANCODE_Q: ev.type = EV_RELEASE; ev.code = 'q'; ev_postevent(&ev); break; 
			case SDL_SCANCODE_E: ev.type = EV_RELEASE; ev.code = 'e'; ev_postevent(&ev); break;
        }
    }
}
		
}

void vid_setpal(int i, int r, int g, int b)
{

}

void vid_preinit()
{
}

void vid_close()
{
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








