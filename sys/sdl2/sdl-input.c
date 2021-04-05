/*
 * sdl-input.c
 * sdl 2 joystick interfaces
 * 
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

#include "input.h"
#include "rc.h"
#include "defs.h"
#include "mem.h"

/* Covert SDL KeyCodes to gnuboy button events
 * This only needs to handle non standard ascii buttons.
 */
static int kb_sdlkeycode_to_gnuboy(SDL_Keycode keycode)
{
        static const int lookup[48][2] =
        {
            //local event, sdl keycode
            {K_SHIFT, SDLK_LSHIFT},
            {K_SHIFT, SDLK_RSHIFT},
            {K_CTRL, SDLK_LCTRL},
            {K_CTRL, SDLK_RCTRL},
            {K_ALT, SDLK_LALT},
            {K_ALT, SDLK_RALT},
            {K_UP, SDLK_UP},
            {K_DOWN, SDLK_DOWN},
            {K_RIGHT, SDLK_RIGHT},
            {K_LEFT, SDLK_LEFT},
            {K_INS, SDLK_INSERT},
            {K_HOME, SDLK_HOME},
            {K_END, SDLK_END},
            {K_PRIOR, SDLK_PRIOR},
            {K_SYSRQ, SDLK_SYSREQ},
            {K_PAUSE, SDLK_PAUSE},
            {K_CAPS, SDLK_CAPSLOCK},
            {K_SCROLL, SDLK_SCROLLLOCK},
            {K_F1, SDLK_F1},
            {K_F2, SDLK_F2},
            {K_F3, SDLK_F3},
            {K_F4, SDLK_F4},
            {K_F5, SDLK_F5},
            {K_F6, SDLK_F6},
            {K_F7, SDLK_F7},
            {K_F8, SDLK_F8},
            {K_F9, SDLK_F9},
            {K_F10, SDLK_F10},
            {K_F11, SDLK_F11},
            {K_F12, SDLK_F12},
            {K_NUM0, SDLK_KP_0},
            {K_NUM1, SDLK_KP_1},
            {K_NUM2, SDLK_KP_2},
            {K_NUM3, SDLK_KP_3},
            {K_NUM4, SDLK_KP_4},
            {K_NUM5, SDLK_KP_5},
            {K_NUM6, SDLK_KP_6},
            {K_NUM7, SDLK_KP_7},
            {K_NUM8, SDLK_KP_8},
            {K_NUM9, SDLK_KP_9},
            {K_NUMPLUS, SDLK_KP_PLUS},
            {K_NUMMINUS, SDLK_KP_MINUS},
            {K_NUMMUL, SDLK_KP_MULTIPLY},
            {K_NUMDIV, SDLK_KP_DIVIDE},
            {K_NUMDOT, SDLK_KP_PERIOD},
            {K_NUMENTER, SDLK_KP_ENTER},
            {K_NUMLOCK, SDLK_NUMLOCKCLEAR},
            {'/', SDLK_KP_DIVIDE}
        };

    //Already a standard ascii character. Just leave
    if (keycode < MAX_KEYS)
        return keycode;

    //Find the matching gnuboy event from the sdl keycode then return it
    for (int i = 0; i < sizeof(lookup) / sizeof(lookup[0]); i++)
    {
        if (lookup[i][1] == keycode)
            return lookup[i][0];
    }
    return keycode;
}

/* Joystick vars */
static Sint32 gamecontroller_map[14][2] =
    {
        {K_JOY0, SDL_CONTROLLER_BUTTON_A},
        {K_JOY1, SDL_CONTROLLER_BUTTON_B},
        {K_JOY2, SDL_CONTROLLER_BUTTON_BACK},
        {K_JOY3, SDL_CONTROLLER_BUTTON_START},
        {K_JOYUP, SDL_CONTROLLER_BUTTON_DPAD_UP},
        {K_JOYDOWN, SDL_CONTROLLER_BUTTON_DPAD_DOWN},
        {K_JOYLEFT, SDL_CONTROLLER_BUTTON_DPAD_LEFT},
        {K_JOYRIGHT, SDL_CONTROLLER_BUTTON_DPAD_RIGHT},
        {-1, SDL_CONTROLLER_BUTTON_X},
        {-1, SDL_CONTROLLER_BUTTON_Y},
        {-1, SDL_CONTROLLER_BUTTON_LEFTSHOULDER},
        {-1, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER},
        {-1, SDL_CONTROLLER_BUTTON_LEFTSTICK},
        {-1, SDL_CONTROLLER_BUTTON_RIGHTSTICK}
    };

//Set to 1 enable debug tracing for input
#define JOYTRACE 0
static int use_joy = 1;
static int rumble_strength = 100;
static SDL_GameController *sdl_joy = NULL;
const int JOYSTICK_DEAD_ZONE = 8000;

rcvar_t joy_exports[] =
    {
        RCV_BOOL("joy", &use_joy),
        RCV_INT("rumble_strength", &rumble_strength),
        RCV_END
    };

static int joy_find_gamecontroller_mapping(SDL_GameControllerButton button)
{
    for (int i = 0; i < (sizeof(gamecontroller_map) / sizeof(gamecontroller_map[0])); i++)
    {
        if (button == gamecontroller_map[i][1])
            return gamecontroller_map[i][0];
    }
    return -1;
}

void joy_init()
{
    //we obviously have no business being in here
    if (!use_joy)
        return;

    //init gamecontroller (and joystick) subsystem
    if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) < 0)
    {
        printf("SDL could not initialize Joystick! SDL Error: %s\n", SDL_GetError());
        exit(1);
    }

    if (rumble_strength > 100)
        rumble_strength = 100;

    if (JOYTRACE)
        printf("Rumble strength set to %i%%\n", rumble_strength);

    if (JOYTRACE)
        printf("Joystick initialized Succesfully\n");
}

void joy_close()
{
    //free the controller
    SDL_GameControllerClose(sdl_joy);
    sdl_joy = NULL;
}

void ev_poll()
{
    event_t ev;
    SDL_Event event;
    static SDL_GameController *pad = NULL;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            exit(1);
        }

        /* Keyboard */
        if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
        {
            SDL_Scancode scancode = event.key.keysym.scancode;
            int keycode = kb_sdlkeycode_to_gnuboy(SDL_GetKeyFromScancode(scancode));

            if (scancode == SDL_SCANCODE_ESCAPE)
                exit(1);

            //If the keycode is > MAX_KEYS, its not a standard ascii button and its not in the kb_sdlkeycode_to_gnuboy map.
            if (keycode < MAX_KEYS)
            {
                ev.type = (event.type == SDL_KEYDOWN) ? EV_PRESS : EV_RELEASE;
                ev.code = keycode;
                ev_postevent(&ev);
            }
            else
            {
                printf("Pressed unmappable button %s\n", SDL_GetScancodeName(scancode));
            }
        }
        /* Handle gamecontroller hotplugging */
        else if (event.type == SDL_CONTROLLERDEVICEADDED && use_joy)
        {
            if (JOYTRACE) printf("Controller %d connected\n", event.cdevice.which);
            SDL_GameControllerOpen(event.cdevice.which);
        }
        else if (event.type == SDL_CONTROLLERDEVICEREMOVED && use_joy)
        {
            if (JOYTRACE) printf("Controller %d disconnected\n", event.cdevice.which);
            SDL_GameControllerClose(SDL_GameControllerFromInstanceID(event.cdevice.which));
        }
        /* Handle gamecontroller button events*/
        else if ((event.type == SDL_CONTROLLERBUTTONDOWN || event.type == SDL_CONTROLLERBUTTONUP) && use_joy)
        {
            pad = SDL_GameControllerFromInstanceID(event.cdevice.which);
            SDL_GameControllerButton btn = event.cbutton.button;
            const char *buttonstring = SDL_GameControllerGetStringForButton(btn);

            ev.type = (event.type == SDL_CONTROLLERBUTTONDOWN) ? EV_PRESS : EV_RELEASE;
            ev.code = joy_find_gamecontroller_mapping(btn);

            if (JOYTRACE) printf("You %s %s\n", (event.type == SDL_CONTROLLERBUTTONDOWN) ? "pressed" : "released", buttonstring);

            if (ev.code != -1)
                ev_postevent(&ev);
        }
    }

    //Will rumble if playing a compatible rom. The rumble strength can be configured by changing the rumble_strength rc var.
    static int old_rumble_state = 0;
    if (pad != NULL && mbc.type == MBC_RUMBLE && (rumble_strength > 0) && (mbc.rumble_state != old_rumble_state))
    {
        Uint16 rumble_val = (mbc.rumble_state) ? (655 * rumble_strength) : 0x0000;
        if (SDL_GameControllerRumble(pad, rumble_val, rumble_val, 250) == 0)
        {
            old_rumble_state = mbc.rumble_state;
        }
    }

}
