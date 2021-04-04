/*
 * MinGW32 system file
 * based on nix.c and dos.c
 * req's SDL 2
 * -Dave Kiddell
 * -Alex Oberhofer
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
char *strdup();

#include <SDL2/SDL.h>
#include "rc.h"

void *sys_timer()
{
	Uint32 *tv;
	
	tv = malloc(sizeof *tv);
	*tv = SDL_GetTicks() * 1000;
	return tv;
}

int sys_elapsed(Uint32 *cl)
{
	Uint32 now;
	Uint32 usecs;

	now = SDL_GetTicks() * 1000;
	usecs = now - *cl;
	*cl = now;
	return usecs;
}

void sys_sleep(int us)
{
	/* dbk: for some reason 2000 works..
	   maybe its just compensation for the time it takes for SDL_Delay to
	   execute, or maybe sys_timer is too slow */
	SDL_Delay(us/1000);
}

void sys_sanitize(char *s)
{
	int i;
	for (i = 0; s[i]; i++)
		if (s[i] == '\\') s[i] = '/';
}

void sys_initpath(char *exe)
{
	char *buf = ".";

	if (rc_getstr("rcpath") == NULL)
		rc_setvar("rcpath", 1, &buf);

	if (rc_getstr("savedir") == NULL)
		rc_setvar("savedir", 1, &buf);
}

void sys_checkdir(char *path, int wr)
{
}


