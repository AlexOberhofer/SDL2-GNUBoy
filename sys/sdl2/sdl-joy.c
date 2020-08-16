/*
 * sdl.c
 * sdl 2 joystick interfaces
 *
 * (C) 2001 Damian Gryski <dgryski@uwaterloo.ca>
 * Based on SDL Joystick code contributed by David Lau
 *
 * Licensed under the GPLv2, or later.
 */
#include <stdlib.h>
#include <stdio.h>

#include <SDL2/SDL.h>

#include "fb.h"
#include "input.h"
#include "rc.h"

rcvar_t joy_exports[] =
{
	RCV_BOOL("joy", 1),
	RCV_END
};

static int use_joy = 1, sdl_joy_num;
static SDL_Joystick * sdl_joy = NULL;
static const int joy_commit_range = 3276;
static char Xstatus, Ystatus;


void joy_init() 
{
	//we obviously have no business being in here
    if (!use_joy) return;
    
    //init joystick subsystem
    if( SDL_Init(SDL_INIT_JOYSTICK < 0 ))
    {
        printf( "SDL could not initialize Joystick! SDL Error: %s\n", SDL_GetError() );
		exit(1);
    }

	//Check for a joystick
    if( SDL_NumJoysticks() < 1 )
    {
        printf( "Warning: No joysticks connected!\n" );
    } else 
	{
        //open the gamepad
        sdl_joy = SDL_JoystickOpen( 0 );
        printf("%d:%s\n", 1, SDL_JoystickNameForIndex(0));
        if( sdl_joy == NULL )
        {
            printf( "Warning: Unable to open game controller! SDL Error: %s\n", SDL_GetError() );
			exit(1);
        }
	}

	printf("Joystick initialized Succesfully\n");
}

void joy_close()
{
	//free the controller
    SDL_JoystickClose(sdl_joy);
    sdl_joy = NULL;
}

void joy_poll()
{
    event_t ev;
	SDL_Event event;

    while(SDL_PollEvent(&event))
    {
        if (ev.type == SDL_QUIT) exit(1);

        if(ev.type == SDL_JOYBUTTONDOWN)
        {
            printf("button down!\n");
        }
    }
}