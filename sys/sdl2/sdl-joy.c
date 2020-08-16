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
	//Initialize SDL
    if( SDL_Init(SDL_INIT_JOYSTICK < 0 ))
    {
        printf( "SDL could not initialize Joystick! SDL Error: %s\n", SDL_GetError() );
		exit(1);
    }

	//Check for joysticks
    if( SDL_NumJoysticks() < 1 )
    {
        printf( "Warning: No joysticks connected!\n" );
    } else 
	{
		//Load joystick
        sdl_joy = SDL_JoystickOpen( 0 );
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
	//Close game controller
    SDL_JoystickClose(sdl_joy);
    sdl_joy = NULL;
}