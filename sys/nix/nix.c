/*
 * nix.c
 *
 * System interface for *nix systems.
 */
#undef _GNU_SOURCE
#define _GNU_SOURCE
#undef _BSD_SOURCE
#define _BSD_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <SDL2/SDL.h>

#include "defs.h"
#include "rc.h"

#define DOTDIR ".gnuboy"

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
	SDL_Delay(us/1000);
}

void sys_checkdir(char *path, int wr)
{
	char *p;
	if (access(path, X_OK | (wr ? W_OK : 0)))
	{
		if (!access(path, F_OK))
			die("cannot access %s: %s\n", path, strerror(errno));
		p = strrchr(path, '/');
		if (!p) die("descended to root trying to create dirs\n");
		*p = 0;
		sys_checkdir(path, wr);
		*p = '/';
		if (mkdir(path, 0777))
			die("cannot create %s: %s\n", path, strerror(errno));
	}
}

void sys_initpath()
{
	char *buf, *home = getenv("HOME");
	if (!home)
	{
		buf = ".";
		rc_setvar("rcpath", 1, &buf);
		rc_setvar("savedir", 1, &buf);
		return;
	}
	buf = malloc(strlen(home) + strlen(DOTDIR) + 8);
	sprintf(buf, "%s/" DOTDIR ":.", home);
	rc_setvar("rcpath", 1, &buf);
	sprintf(buf, "%s/" DOTDIR "/saves" , home);
	rc_setvar("savedir", 1, &buf);
	free(buf);
}

void sys_sanitize(char *s)
{
}




