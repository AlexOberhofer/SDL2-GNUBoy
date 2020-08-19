//TODO: Rewrite this + put it somewhere else... But use my sdl joystick debug tracing to improve on this idea
//my joystick impl doesnt exist on its own... So this will probably involve keyboard + mouse if I decide to add
//it
//Original joytest author: rofl0r
#include "sys.h"
#include "events.c"
#include "keytable.c"
#include <stdio.h>

static const char* evt_str[] = { [EV_NONE] = "none", [EV_PRESS] = "press", [EV_RELEASE]  = "release", [EV_REPEAT] = "repeat", [EV_MOUSE] = "mouse", };

int main() 
{
	/*TODO: Joystick init will need to be updated to give more debug info on subsystem start */
	joy_init();
	event_t e;
	while(1) {
		/* Event Poll will trace the SDL joystick/keyboard/gamepad/mouse information */
		ev_poll();
		if(ev_getevent(&e)) 
		{

			/* Here we get the output of what is recieved by the event queue */
			printf("%s: %s\n", evt_str[e.type], k_keyname(e.code));
		} else {
			sys_sleep(3000); //amo: idk... 3000?
		}
	}
	return 0;
}

int WinMain()
{
	main();
	return 0;
}

