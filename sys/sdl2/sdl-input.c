/*
 * sdl-input.c
 * sdl 2 joystick interfaces
 * 
 * (C) 2020 Alex Oberhofer <alexmoberhofer@gmail.com>
 *
 * Licensed under the GPLv2, or later.
 */
#include <stdlib.h>
#include <stdio.h>

#include <SDL2/SDL.h>

#include "input.h"
#include "rc.h"

/* Set to 1 enable debug tracing for input */
#define JOYTRACE 0


/* Joystick vars */
static int use_joy = 1, sdl_joy_num;
static SDL_Joystick *sdl_joy = NULL;
const int JOYSTICK_DEAD_ZONE = 8000;

rcvar_t joy_exports[] =
    {
        RCV_BOOL("joy", &use_joy),
        RCV_END
    };

/* Store which direction hat value was sent to the event queue on the last iteraon */
static int hat_pressed = 0;

/* used to track what joystick direction to release */
static int last_joy_sent = -1;

void joy_init()
{
    //we obviously have no business being in here
    if (!use_joy)
        return;

    //init joystick subsystem
    if (SDL_Init(SDL_INIT_JOYSTICK < 0))
    {
        printf("SDL could not initialize Joystick! SDL Error: %s\n", SDL_GetError());
        exit(1);
    }

    //Check for a joystick
    if (SDL_NumJoysticks() < 1)
    {
        printf("Warning: No joysticks connected!\n");
        return;
    }
    else
    {
        printf("Found %d joysticks\n", SDL_NumJoysticks());
        //open the gamepad
        sdl_joy = SDL_GameControllerOpen(0);

        //if an error occurs and we have another gamepad... try it
        if(sdl_joy == NULL && (SDL_NumJoysticks() > 1))
        {
            sdl_joy = SDL_GameControllerOpen(1);
        }

        printf("%d:%s\n", 1, SDL_GameControllerNameForIndex(0));
        if (sdl_joy == NULL)
        {
            printf("Warning: Unable to open game controller! SDL Error: %s\n", SDL_GetError());
            exit(1);
        }
    }

    if(JOYTRACE) printf("Joystick initialized Succesfully\n");
}

void joy_close()
{
    //free the controller
    SDL_GameControllerClose(sdl_joy);
    sdl_joy = NULL;
}

/* TODO: Really need to clean this up - this will break if you rebind the keys.*/
/* TODO: Write an effing map function to map sdl keypresses to event codes */
/* TODO: Write a helper function to build + post events */
/* TODO: Add a second joystick event handler */
void ev_poll()
{
    event_t ev;
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {

        if (event.type == SDL_QUIT)
        {
            exit(1);
        }

        /* Keyboard */
        if (event.type == SDL_KEYDOWN)
        {

            uint32_t key = event.key.keysym.scancode;

            switch (key)
            {
            case SDL_SCANCODE_RETURN:
                ev.type = EV_PRESS;
                ev.code = K_ENTER;
                ev_postevent(&ev);
                break;
            case SDL_SCANCODE_X:
                ev.type = EV_PRESS;
                ev.code = 'x';
                ev_postevent(&ev);
                break;
            case SDL_SCANCODE_ESCAPE:
                die("Escape Pressed\n");
                break;
            case SDL_SCANCODE_A:
                ev.type = EV_PRESS;
                ev.code = K_LEFT;
                ev_postevent(&ev);
                break;
            case SDL_SCANCODE_D:
                ev.type = EV_PRESS;
                ev.code = K_RIGHT;
                ev_postevent(&ev);
                break;
            case SDL_SCANCODE_S:
                ev.type = EV_PRESS;
                ev.code = K_DOWN;
                ev_postevent(&ev);
                break;
            case SDL_SCANCODE_W:
                ev.type = EV_PRESS;
                ev.code = K_UP;
                ev_postevent(&ev);
                break;
            case SDL_SCANCODE_Q:
                ev.type = EV_PRESS;
                ev.code = 'q';
                ev_postevent(&ev);
                break;
            case SDL_SCANCODE_E:
                ev.type = EV_PRESS;
                ev.code = 'e';
                ev_postevent(&ev);
                break;
            }
        }
        else if (event.type == SDL_KEYUP)
        {

            uint32_t key = event.key.keysym.scancode;

            switch (key)
            {
            case SDL_SCANCODE_RETURN:
                ev.type = EV_RELEASE;
                ev.code = K_ENTER;
                ev_postevent(&ev);
                break;
            case SDL_SCANCODE_X:
                ev.type = EV_RELEASE;
                ev.code = 'x';
                ev_postevent(&ev);
                break;
            case SDL_SCANCODE_ESCAPE:
                die("Escape Pressed\n");
                break;
            case SDL_SCANCODE_A:
                ev.type = EV_RELEASE;
                ev.code = K_LEFT;
                ev_postevent(&ev);
                break;
            case SDL_SCANCODE_D:
                ev.type = EV_RELEASE;
                ev.code = K_RIGHT;
                ev_postevent(&ev);
                break;
            case SDL_SCANCODE_S:
                ev.type = EV_RELEASE;
                ev.code = K_DOWN;
                ev_postevent(&ev);
                break;
            case SDL_SCANCODE_W:
                ev.type = EV_RELEASE;
                ev.code = K_UP;
                ev_postevent(&ev);
                break;
            case SDL_SCANCODE_Q:
                ev.type = EV_RELEASE;
                ev.code = 'q';
                ev_postevent(&ev);
                break;
            case SDL_SCANCODE_E:
                ev.type = EV_RELEASE;
                ev.code = 'e';
                ev_postevent(&ev);
                break;
            }
        }


        
        /*
        * The joystick impl sucks compared to the gamepad... Maybe I add this back in as a define for 
        * compatability with older joysticks or gamepads?
        */
        if (event.type == SDL_JOYBUTTONDOWN)
        {
            /*switch (event.jbutton.button)
            {
            //note: sdl assumes xbox360 style controller... so I've reversed A + B for now.
            //reversed as in... reverse on 360 pads... correct on the rest of my controllers
            case SDL_CONTROLLER_BUTTON_A:
                if(JOYTRACE) printf("You pressed B\n");
                ev.type = EV_PRESS;
                ev.code = 'e';
                ev_postevent(&ev);
                break;
            case SDL_CONTROLLER_BUTTON_B:
                if(JOYTRACE) printf("You pressed A\n");
                ev.type = EV_PRESS;
                ev.code = 'q';
                ev_postevent(&ev);
                break;
            case 7: //??wtf
                if(JOYTRACE) printf("You pressed Start\n");
                ev.type = EV_PRESS;
                ev.code = K_ENTER;
                ev_postevent(&ev);
                break;
            case SDL_CONTROLLER_BUTTON_BACK:
                if(JOYTRACE) printf("You pressed Back\n"); 
                ev.type = EV_PRESS;
                ev.code = 'space';
                //ev_postevent(&ev);
                break;
            default:
                if(JOYTRACE) printf("SDL_JOYBUTTONDOWN: joystick: %d button: %d state: %d\n",
                       event.jbutton.which, event.jbutton.button, event.jbutton.state);
                break;
            }*/
        }

        /*if (event.type == SDL_JOYBUTTONUP)
        {
            switch (event.jbutton.button)
            {
            case SDL_CONTROLLER_BUTTON_A:
                if(JOYTRACE) printf("You released B\n");
                ev.type = EV_RELEASE;
                ev.code = 'e';
                ev_postevent(&ev);
                break;
            case SDL_CONTROLLER_BUTTON_B:
                if(JOYTRACE) printf("You released A\n");
                ev.type = EV_RELEASE;
                ev.code = 'q';
                ev_postevent(&ev);
                break;
            case 7: //??wtf
                if(JOYTRACE) printf("You released start\n");
                ev.type = EV_RELEASE;
                ev.code = K_ENTER;
                ev_postevent(&ev);
                break;
            case 6:
                if(JOYTRACE) printf("You released Back\n");
                ev.type = EV_RELEASE;
                ev.code = 'space';
                //ev_postevent(&ev);
                break;
            default:
                if(JOYTRACE) printf("SDL_JOYBUTTONUP: joystick: %d button: %d state: %d\n",
                       event.jbutton.which, event.jbutton.button, event.jbutton.state);
                break;
            }
        }*/

        //note: this can probably be common to each controller setup
        //dpad when its a "hat" and not 4 buttons
        if (event.type == SDL_JOYHATMOTION)
        {
            if(JOYTRACE) printf("SDL_JOYHATMOTION: joystick: %d hat: %d value: %d\n",
                           event.jhat.which, event.jhat.hat, event.jhat.value);

            if(event.jhat.value == 1) {
                if(JOYTRACE) printf("Pad Up\n");
                ev.type = EV_PRESS;
                ev.code = K_UP;
                ev_postevent(&ev);
                hat_pressed = 1;
            }

            if(event.jhat.value == 2) {
                if(JOYTRACE) printf("Pad Right\n");
                ev.type = EV_PRESS;
                ev.code = K_RIGHT;
                ev_postevent(&ev);
                hat_pressed = 2;
            }

            if(event.jhat.value == 4) {
                if(JOYTRACE) printf("Pad Down\n");
                ev.type = EV_PRESS;
                ev.code = K_DOWN;
                ev_postevent(&ev);
                hat_pressed = 4;
            }

            if(event.jhat.value == 8) {
                if(JOYTRACE) printf("Pad Left\n");
                ev.type = EV_PRESS;
                ev.code = K_LEFT;
                ev_postevent(&ev);
                hat_pressed = 8;
            }

            if(event.jhat.value == 0) { //release whatever direction we hit on the last run
                if(JOYTRACE) printf("Release %d\n", hat_pressed);
                ev.type = EV_RELEASE;
                switch (hat_pressed) {
                    case 1: ev.code = K_UP; break;
                    case 2: ev.code = K_RIGHT; break;
                    case 4: ev.code = K_DOWN; break;
                    case 8: ev.code = K_LEFT; break;
                }

                ev_postevent(&ev);
            }
        }

        /* Joypad */
        //Probably going to have to do a separate gamepad impl
        if(event.type == SDL_JOYAXISMOTION )
        {
            //X axis motion
            if( event.jaxis.axis == 0 )
            { 
                //Left of dead zone
                if( event.jaxis.value < -JOYSTICK_DEAD_ZONE )
                {
                    if(JOYTRACE) printf("Joy Left\n");
                    if(last_joy_sent != -1) 
                    {
                        ev.type = EV_RELEASE;
                        ev.code = last_joy_sent;
                        ev_postevent(&ev);
                    }
                    ev.type = EV_PRESS;
                    ev.code = K_LEFT;
                    last_joy_sent = K_LEFT;
                    ev_postevent(&ev);
                }
                else if( event.jaxis.value > JOYSTICK_DEAD_ZONE )
                {
                    if(JOYTRACE) printf("Joy Right\n");
                    if(last_joy_sent != -1) 
                    {
                        ev.type = EV_RELEASE;
                        ev.code = last_joy_sent;
                        ev_postevent(&ev);
                    }
                    ev.type = EV_PRESS;
                    ev.code = K_RIGHT;
                    last_joy_sent = K_RIGHT;
                    ev_postevent(&ev);
                }
                else
                {
                    if(JOYTRACE) printf("Reset Joystick L/R\n");
                    if(last_joy_sent != -1) 
                    {
                        ev.type = EV_RELEASE;
                        ev.code = last_joy_sent;
                        ev_postevent(&ev);
                    }
                }
            }
            
            //Y axis motion
            if( event.jaxis.axis == 1 )
            { 
                //Left of dead zone
                if( event.jaxis.value < -JOYSTICK_DEAD_ZONE )
                {
                    if(JOYTRACE) printf("Joy Up\n");
                    if(last_joy_sent != -1) 
                    {
                        ev.type = EV_RELEASE;
                        ev.code = last_joy_sent;
                        ev_postevent(&ev);
                    }
                    ev.type = EV_PRESS;
                    ev.code = K_UP;
                    ev_postevent(&ev);
                    last_joy_sent = K_UP;
                }
                else if( event.jaxis.value > JOYSTICK_DEAD_ZONE )
                {
                    if(JOYTRACE) printf("Joy Down\n");
                    if(last_joy_sent != -1) 
                    {
                        ev.type = EV_RELEASE;
                        ev.code = last_joy_sent;
                        ev_postevent(&ev);
                    }
                    ev.type = EV_PRESS;
                    ev.code = K_DOWN;
                    ev_postevent(&ev);
                    last_joy_sent = K_DOWN;
                }
                else
                {
                    if(JOYTRACE) printf("Reset Joystick U/D\n");
                    /*if(last_joy_sent != -1) {
                        ev.type = EV_RELEASE;
                        ev.code = last_joy_sent;
                        ev_postevent(&ev);
                    }*/

                }
            }
        }

        //I dont even know - ok now I know... add option to change between gamepad impl and joystick impl... probably a define...
        if (event.type == SDL_CONTROLLERBUTTONDOWN)
        {
            char *buttonstring = SDL_GameControllerGetStringForButton(event.cbutton.button);
            
            if((strcmp(buttonstring, "start")) == 0) 
            {
                if(JOYTRACE) printf("You pressed Start\n");
                ev.type = EV_PRESS;
                ev.code = K_ENTER;
                ev_postevent(&ev);
            } else if((strcmp(buttonstring, "b")) == 0)
            {
                if(JOYTRACE) printf("You released B\n");
                ev.type = EV_PRESS;
                ev.code = 'e';
                ev_postevent(&ev);
            } else if((strcmp(buttonstring, "a")) == 0)
            {
                if(JOYTRACE) printf("You released A\n");
                ev.type = EV_PRESS;
                ev.code = 'q';
                ev_postevent(&ev);
            } else if((strcmp(buttonstring, "back")) == 0)
            {
                if(JOYTRACE) printf("You pressed Back\n");
                ev.type = EV_PRESS;
                ev.code = 'x';
                ev_postevent(&ev);
            } else {
                if(JOYTRACE) printf("SDL_CONTROLLERBUTTONUP   controller: %d button: %s state: %d\n",
                   event.cbutton.which,
                   SDL_GameControllerGetStringForButton(event.cbutton.button),
                   event.cbutton.state);
            }         
        }

        if (event.type == SDL_CONTROLLERBUTTONUP)
        {
            char *buttonstring = SDL_GameControllerGetStringForButton(event.cbutton.button);

            if((strcmp(buttonstring, "start")) == 0) 
            {
                if(JOYTRACE) printf("You released Start\n");
                ev.type = EV_RELEASE;
                ev.code = K_ENTER;
                ev_postevent(&ev);
            } else if((strcmp(buttonstring, "b")) == 0)
            {
                if(JOYTRACE) printf("You released B\n");
                ev.type = EV_RELEASE;
                ev.code = 'e';
                ev_postevent(&ev);
            } else if((strcmp(buttonstring, "a")) == 0)
            {
                if(JOYTRACE) printf("You released A\n");
                ev.type = EV_RELEASE;
                ev.code = 'q';
                ev_postevent(&ev);
            } else if((strcmp(buttonstring, "back")) == 0)
            {
                if(JOYTRACE) printf("You released Back\n");
                ev.type = EV_RELEASE;
                ev.code = 'x';
                ev_postevent(&ev);
            } else {
                if(JOYTRACE) printf("SDL_CONTROLLERBUTTONUP   controller: %d button: %s state: %d\n",
                   event.cbutton.which,
                   SDL_GameControllerGetStringForButton(event.cbutton.button),
                   event.cbutton.state);
            }
        }

        /* :\ */
        if (event.type == SDL_CONTROLLERAXISMOTION)
        {
            if(JOYTRACE) printf("SDL_CONTROLLERAXISMOTION controller: %d axis: %-12s value: %d\n",
                   event.caxis.which,
                   SDL_GameControllerGetStringForAxis(event.caxis.axis),
                   event.caxis.value);
        }

        
    }
}