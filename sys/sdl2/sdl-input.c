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
			case SDL_SCANCODE_ESCAPE: die("Escape Pressed\n"); break;    
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
			case SDL_SCANCODE_ESCAPE: die("Escape Pressed\n"); break;    
			case SDL_SCANCODE_A: ev.type = EV_RELEASE; ev.code = K_LEFT; ev_postevent(&ev); break; 
			case SDL_SCANCODE_D: ev.type = EV_RELEASE; ev.code = K_RIGHT; ev_postevent(&ev); break;
			case SDL_SCANCODE_S: ev.type = EV_RELEASE; ev.code = K_DOWN; ev_postevent(&ev); break;
			case SDL_SCANCODE_W: ev.type = EV_RELEASE; ev.code = K_UP; ev_postevent(&ev); break;   
			case SDL_SCANCODE_Q: ev.type = EV_RELEASE; ev.code = 'q'; ev_postevent(&ev); break; 
			case SDL_SCANCODE_E: ev.type = EV_RELEASE; ev.code = 'e'; ev_postevent(&ev); break;
        }
    }

    if(event.type == SDL_JOYBUTTONDOWN || event.type == SDL_JOYBUTTONUP)
    {
        printf("Joystick button event\n"); //TODO: Map buttons and send events to the queue
    }

}
		
}