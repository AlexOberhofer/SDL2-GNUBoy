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

#include "input.h"
#include "rc.h"

rcvar_t joy_exports[] =
    {
        RCV_BOOL("joy", 1),
        RCV_END};

static int use_joy = 1, sdl_joy_num;
static SDL_Joystick *sdl_joy = NULL;
static const int joy_commit_range = 3276;
static char Xstatus, Ystatus;

/* Store which direction hat value was sent to the event queue on the last iteraon */
static int hat_pressed = 0;

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
    }
    else
    {
        //open the gamepad
        sdl_joy = SDL_JoystickOpen(0);
        printf("%d:%s\n", 1, SDL_JoystickNameForIndex(0));
        if (sdl_joy == NULL)
        {
            printf("Warning: Unable to open game controller! SDL Error: %s\n", SDL_GetError());
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

    while (SDL_PollEvent(&event))
    {

        if (event.type == SDL_QUIT)
        {
            exit(1);
        }

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

        if (event.type == SDL_JOYBUTTONDOWN)
        {
            switch (event.jbutton.button)
            {
            case SDL_CONTROLLER_BUTTON_A:
                printf("You pressed A\n");
                ev.type = EV_PRESS;
                ev.code = 'q';
                ev_postevent(&ev);
                break;
            case SDL_CONTROLLER_BUTTON_B:
                printf("You pressed B\n");
                ev.type = EV_PRESS;
                ev.code = 'e';
                ev_postevent(&ev);
                break;
            case 7:
                printf("You pressed Start\n");
                ev.type = EV_PRESS;
                ev.code = K_ENTER;
                ev_postevent(&ev);
                break;
            case 6:
                printf("You pressed Back\n"); //TODO: CALL SELECT IDIOT
                ev.type = EV_PRESS;
                ev.code = 'tab';
                //ev_postevent(&ev);
                break;
            default:
                printf("SDL_JOYBUTTONDOWN: joystick: %d button: %d state: %d\n",
                       event.jbutton.which, event.jbutton.button, event.jbutton.state);
                break;
            }
        }

        if (event.type == SDL_JOYBUTTONUP)
        {
            switch (event.jbutton.button)
            {
            case SDL_CONTROLLER_BUTTON_A:
                printf("You released A\n");
                ev.type = EV_RELEASE;
                ev.code = 'q';
                ev_postevent(&ev);
                break;
            case SDL_CONTROLLER_BUTTON_B:
                printf("You released B\n");
                ev.type = EV_RELEASE;
                ev.code = 'e';
                ev_postevent(&ev);
                break;
            case 7:
                printf("You released start\n");
                ev.type = EV_RELEASE;
                ev.code = K_ENTER;
                ev_postevent(&ev);
                break;
            case 6:
                printf("You released Back\n");
                ev.type = EV_RELEASE;
                ev.code = 'tab';
                //ev_postevent(&ev);
                break;
            default:
                printf("SDL_JOYBUTTONUP: joystick: %d button: %d state: %d\n",
                       event.jbutton.which, event.jbutton.button, event.jbutton.state);
                break;
            }
        }

        if (event.type == SDL_CONTROLLERBUTTONDOWN)
        {
            printf("SDL_CONTROLLERBUTTONDOWN controller: %d button: %s state: %d\n",
                   event.cbutton.which,
                   SDL_GameControllerGetStringForButton(event.cbutton.button),
                   event.cbutton.state);
        }

        if (event.type == SDL_CONTROLLERBUTTONUP)
        {
            printf("SDL_CONTROLLERBUTTONUP   controller: %d button: %s state: %d\n",
                   event.cbutton.which,
                   SDL_GameControllerGetStringForButton(event.cbutton.button),
                   event.cbutton.state);
        }

        if (event.type == SDL_CONTROLLERAXISMOTION)
        {
            printf("SDL_CONTROLLERAXISMOTION controller: %d axis: %-12s value: %d\n",
                   event.caxis.which,
                   SDL_GameControllerGetStringForAxis(event.caxis.axis),
                   event.caxis.value);
        }

        if (event.type == SDL_JOYHATMOTION)
        {
            //TODO: DPAD for xbox controller to go here
            //printf("SDL_JOYHATMOTION: joystick: %d hat: %d value: %d\n",
            //               event.jhat.which, event.jhat.hat, event.jhat.value);

            if(event.jhat.value == 1) {
                printf("Up\n");
                ev.type = EV_PRESS;
                ev.code = K_UP;
                ev_postevent(&ev);
                hat_pressed = 1;
            }

            if(event.jhat.value == 2) {
                printf("Right\n");
                ev.type = EV_PRESS;
                ev.code = K_RIGHT;
                ev_postevent(&ev);
                hat_pressed = 2;
            }

            if(event.jhat.value == 4) {
                printf("Down\n");
                ev.type = EV_PRESS;
                ev.code = K_DOWN;
                ev_postevent(&ev);
                hat_pressed = 4;
            }

            if(event.jhat.value == 8) {
                printf("Left\n");
                ev.type = EV_PRESS;
                ev.code = K_LEFT;
                ev_postevent(&ev);
                hat_pressed = 8;
            }

            if(event.jhat.value == 0) { //release whatever direction we hit on the last run
                printf("Release %d\n", hat_pressed);
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
    }
}