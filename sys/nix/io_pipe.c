#include <stdio.h>

#include "defs.h"
#include "io.h"
#include "hw.h"
#include "regs.h"
#include "debug.h"

#include <unistd.h>
#include <stdio.h>
#include <poll.h>

#include <fcntl.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#include <string.h>
#include <stdlib.h>


void io_pipe_init();
void io_pipe_init_path();
void io_pipe_try_open();
void io_pipe_send(un8);
void io_pipe_send_mode(un8* mode, un8* byte);
int  io_pipe_ready();
void io_pipe_recv();
int  io_pipe_read(un8* mode, un8* byte);
un8  io_pipe_trigger_int(un8 byte);
void io_pipe_shutdown();

int file_a, file_b, player_a;
const char* file_name_prefix;
char* file_name_a;
char* file_name_b;

int io_setup_pipe(const char* path, struct io_handler* io_hdlr)
{
	io_hdlr->init = io_pipe_init;
	io_hdlr->send = io_pipe_send;
	io_hdlr->recv = io_pipe_recv;
	io_hdlr->shutdown = io_pipe_shutdown;
	file_name_prefix = path;
	return 1;
}

void io_pipe_try_open()
{
	if (file_a == -1)
	{
		if (player_a)
		{
			if (access(file_name_a, 0) != -1)
			{
				file_a = open(file_name_a, O_WRONLY | O_NONBLOCK);
			}
		}
		else
		{
			if (access(file_name_b, 0) != -1)
			{
				file_a = open(file_name_b, O_WRONLY| O_NONBLOCK);
			}
		}
	}
}

int dir_name(char* file_name)
{
	unsigned int i, last = -1;

	for (i = 0; i < strlen(file_name); i++)
	{
		if (file_name[i] == '/')
		{
			last = i;
		}
	}
	if (last == -1)
	{
		return 0;
	}
	file_name[last] = '\0';
	return 1;
}

void un_dir_name(char* file_name)
{
	file_name[strlen(file_name)] = '/';
}

void io_pipe_init_path()
{
	int max_file_name_length = strlen(file_name_prefix) + 1 + 2;

	char* dirname;

	struct stat dir;

	file_name_a = (char*) malloc(sizeof(char) * max_file_name_length);
	file_name_b = (char*) malloc(sizeof(char) * max_file_name_length);

	strncpy(file_name_a, file_name_prefix, max_file_name_length);
	strncpy(file_name_b, file_name_prefix, max_file_name_length);

	file_name_a[max_file_name_length - 3] = '_';
	file_name_a[max_file_name_length - 2] = 'a';
	file_name_a[max_file_name_length - 1] = '\0';

	file_name_b[max_file_name_length - 3] = '_';
	file_name_b[max_file_name_length - 2] = 'b';
	file_name_b[max_file_name_length - 1] = '\0';

	debugf("file_a name: %s\n", file_name_a);
	debugf("file_b name: %s\n", file_name_b);

	if (dir_name(file_name_a))
	{
		dirname = (char*) malloc(sizeof(char) * (strlen(file_name_a) + 1));
		strncpy(dirname, file_name_a, strlen(file_name_a)+1);
		un_dir_name(file_name_a);
	}
	else
	{
		dirname = getcwd(NULL, 0);
	}

	debugf("file_a dir name: '%s' length: %ld\n", dirname, strlen(dirname));

	if (!(stat(dirname, &dir) == 0 && S_ISDIR(dir.st_mode)))
	{
		fprintf(stderr, "link: failed to create pipe, directory does not exist\n");
		exit(1);
	}

	if (access(dirname, W_OK))
	{
		fprintf(stderr, "link: failed to create pipe, directory not writable\n");
		exit(1);
	}


	if (access(file_name_a, 0) != -1 || access(file_name_a, 0) != -1)
	{
		fprintf(stderr, "link: failed to create pipe, file exists\n");
		exit(1);
	}

	if (dirname != NULL)
	{
		free(dirname);
	}

}

void io_pipe_init()
{
	debugf("io_pipe_init\n");

	file_a = -1;
	file_b = -1;
	player_a = 0;

	io_pipe_init_path();

	if (access(file_name_b, 0) == -1)
	{
		player_a = 1;
		mkfifo(file_name_b, 0777);
		file_b = open(file_name_b, O_RDONLY| O_NONBLOCK);
	}
	else
	{
		mkfifo(file_name_a, 0777);
		file_b = open(file_name_a, O_RDONLY| O_NONBLOCK);
	}
}

void io_pipe_send(un8 byte)
{
	un8 mode;

	debugf("io_pipe_send");

	mode = (un8) SEND_ACTIVE;
	io_pipe_send_mode(&mode, &byte);
}

void io_pipe_send_mode(un8* mode, un8* byte)
{
	int polled, can_write, poll_err, poll_hup;
	struct pollfd poll_s;

	debugf("io_pipe_send_mode: %d %d\n", *mode, *byte);

	io_pipe_try_open();

	if (file_a != -1)
	{


		poll_s.fd = file_a;
		poll_s.events = POLLOUT;
		poll_s.revents = 0;

		polled = poll(&poll_s, 1, 0);
		can_write = poll_s.revents & POLLOUT;
		poll_err = poll_s.revents & POLLERR;
		poll_hup = poll_s.revents & POLLHUP;

		debugf("io_pipe_send_mode: %d %d %d %d\n", polled, can_write, poll_err, poll_hup);


		if (poll_err || poll_hup)
		{
			io_reset();
		} else if (polled && can_write)
		{
			write(file_a, mode, 1);
			write(file_a, byte, 1);
		}
	}
}

int io_pipe_ready ()
{
	int polled, can_read, poll_err, poll_hup;
	struct pollfd poll_s;

	//debugf("io_pipe_ready\n");

	io_pipe_try_open();

	poll_s.fd = file_b;
	poll_s.events = POLLIN;
	poll_s.revents = 0;

	polled = poll(&poll_s, 1, 0);
	can_read = poll_s.revents & POLLIN;
	poll_err = poll_s.revents & POLLERR;
	poll_hup = poll_s.revents & POLLHUP;

	//debugf("io_pipe_ready: %d %d %d %d\n", polled, can_read, poll_err, poll_hup);

	if (poll_err || poll_hup)
	{
		io_reset();
	}

	return polled == 1 && can_read != 0;
}

void io_pipe_recv()
{

	//debugf("io_pipe_recv\n");

	if (io_pipe_ready())
	{
		debugf("io_pipe_recv: io is ready\n");
		un8 mode;
		un8 byte;
		if (io_pipe_read(&mode, &byte))
		{
			debugf("io_pipe_recv: valid read\n");
			un8 own_data = io_pipe_trigger_int(byte);
			if (mode & 1)
			{
				un8 mode = (un8) SEND_PASSIVE;
				io_pipe_send_mode(&mode, &own_data);
			}
		}
	}
}

un8 io_pipe_trigger_int(un8 byte)
{
	un8 old_data = R_SB;
	R_SB = byte;
	hw_interrupt(IF_SERIAL, IF_SERIAL);
	hw_interrupt(0, IF_SERIAL);
	return old_data;
}

int io_pipe_read(un8* mode, un8* byte)
{
	int ret;
	ret = read(file_b, mode, 1) + read(file_b, byte, 1) == 2;
	debugf("io_pipe_read: %d %d\n", *mode, *byte);
	return ret;
}

void io_pipe_shutdown()
{
	debugf("io_pipe_shutdown\n");

	if ( file_a != -1 )
	{
		close(file_a);
	}

	if ( file_a != -1 )
	{
		close(file_b);
	}
	if(player_a == 1)
	{
		remove(file_name_b);
	}
	else
	{
		remove(file_name_a);
	}

	free(file_name_a);
	free(file_name_b);
}
