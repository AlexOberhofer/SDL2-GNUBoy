#include <stdio.h>

#include "io.h"
#include "debug.h"


struct io_handler* io_hdlr = NULL;

void io_handler_set(struct io_handler* hndlr)
{
	io_hdlr = hndlr;
}

void io_init()
{
	debugf("io_init\n");
	if (io_hdlr != NULL)
	{
		io_hdlr->init();
	}
}

void io_send(un8 byte)
{
	debugf("io_send\n");
	if (io_hdlr != NULL)
	{
		io_hdlr->send(byte);
	}
}


void io_recv()
{
	debugf("io_recv\n");
	if (io_hdlr != NULL)
	{
		io_hdlr->recv();
	}
}

void io_shutdown()
{
	debugf("io_shutdown\n");
	if (io_hdlr != NULL)
	{
		io_hdlr->shutdown();
	}
}

void io_reset()
{
	debugf("io_reset\n");
	io_shutdown();
	io_init();
}
