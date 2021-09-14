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
#include <sys/ioctl.h>
#include <errno.h>

#include <string.h>
#include <stdlib.h>

// networking
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

void io_network_init();
void io_network_init_ports();
void io_network_try_open();
void io_network_try_open_local_server();
void io_network_try_open_remote_connection();
void io_network_send(un8);
void io_network_send_mode(un8* mode, un8* byte);
int  io_network_ready();
void io_network_recv();
un8  io_network_trigger_int(un8 byte);
void io_network_shutdown();

int server_sock = -1, client_sock = -1, local_port = -1, remote_socket = -1, remote_port = -1;
char *local_port_s, *remote_s, *remote_port_s;

const char* link_data;
char* data;

int io_setup_network(const char* l, struct io_handler* io_hdlr)
{
	io_hdlr->init = io_network_init;
	io_hdlr->send = io_network_send;
	io_hdlr->recv = io_network_recv;
	io_hdlr->shutdown = io_network_shutdown;
	link_data = l;
	return 1;
}

void io_network_try_open_local_server()
{
	struct sockaddr_in addr;
	int on = 1;

	if (server_sock == -1)
	{
		addr.sin_family = AF_INET;
		addr.sin_port = htons(local_port);
		addr.sin_addr.s_addr = htonl(INADDR_ANY);

		server_sock = socket(AF_INET, SOCK_STREAM, 0);
		if (server_sock < 0)
		{
			fprintf(stderr, "link: failed to open connection accepting server\n");
			exit(1);
		}

		if (setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, (char *) &on, sizeof(on)) < 0)
		{
			close(server_sock);
			fprintf(stderr, "link: failed to set options\n");
			exit(1);
		}

		if (ioctl(server_sock, FIONBIO, (char *) &on) < 0)
		{
			close(server_sock);
			fprintf(stderr, "link: failed to set non blocking\n");
			exit(1);
		}

		if (bind(server_sock, (struct sockaddr *) &addr, sizeof(addr)) == -1)
		{
			close(server_sock);
			fprintf(stderr, "link: failed to bind local port\n");
			exit(1);
		}

		if (listen(server_sock, 1) == -1)
		{
			close(server_sock);
			fprintf(stderr, "link: failed to listen on local port\n");
			exit(1);
		}

		debugf("link: server started\n");
	}
}

void io_network_try_open_remote_connection()
{
	struct sockaddr_in serv_addr;
	struct hostent *hp;

	if(remote_socket == -1)
	{
		if ((remote_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		{
			debugf("failed to open socket\n");
			return;
		}

		serv_addr.sin_family = AF_INET;
		serv_addr.sin_port = htons(remote_port);
		debugf("opened remote socket: %d\n", remote_port);

		/*if(inet_pton(AF_INET, remote_s, &serv_addr.sin_addr) <= 0)
		{
			close(remote_socket);
			remote_socket = -1;
			debugf("failed to create inet %s\n", remote_s);
			return;
		}*/

		if (!(hp = gethostbyname(remote_s)))
        {
			close(remote_socket);
			remote_socket = -1;
			debugf("failed to create inet %s\n", remote_s);
			return;
		}

		memcpy (&serv_addr.sin_addr, hp->h_addr_list[0], sizeof(serv_addr.sin_addr));

		if (connect(remote_socket, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		{
			close(remote_socket);
			remote_socket = -1;
			debugf("failed to connect %d\n", errno); // this (error: 111, connection refused) might be fine if the emu instance was started first
			return;
		}
	}
}

void io_network_try_open()
{
	io_network_try_open_local_server();
	io_network_try_open_remote_connection();
}

void io_network_init_ports()
{
	int i;
	size_t data_len;
	data_len = strlen(link_data) + 1;
	data = (char*) malloc(sizeof(char) * data_len);
	memcpy(data, link_data, data_len);

	local_port_s = data;

	for (i = 0; i < data_len; i++)
	{
		if (data[i] == ':')
		{
			data[i] = '\0';
			remote_s = data + i + 1;
			break;
		}
	}

	for (i = data_len - 1; i >= 0 ; i--)
	{
		if (data[i] == ':')
		{
			data[i] = '\0';
			remote_port_s = data + i + 1;
			break;
		}
	}

	debugf("local port %s\n", local_port_s);
	debugf("remote %s\n", remote_s);
	debugf("remote port %s\n", remote_port_s);

	local_port = atoi(local_port_s);
	remote_port = atoi(remote_port_s);

}

void io_network_init()
{
	debugf("io_network_init\n");

	io_network_init_ports();

	io_network_try_open();
}

void io_network_send(un8 byte)
{
	un8 mode;

	debugf("io_network_send\n");

	mode = (un8) SEND_ACTIVE;
	io_network_send_mode(&mode, &byte);
}

void io_network_send_mode(un8* mode, un8* byte)
{
	int polled, can_write, poll_err, poll_hup;
	struct pollfd poll_s;
	char data[2] = {*mode, *byte};

	debugf("io_network_send_mode: %d %d\n", *mode, *byte);

	if(remote_socket <= 0)
	{
		io_network_try_open();
	}

	if(remote_socket > 0)
	{
		poll_s.fd = remote_socket;
		poll_s.events = POLLOUT;
		poll_s.revents = 0;

		polled = poll(&poll_s, 1, 0);
		can_write = poll_s.revents & POLLOUT;
		poll_err = poll_s.revents & POLLERR;
		poll_hup = poll_s.revents & POLLHUP;

		debugf("io_pipe_network_mode: %d %d %d %d\n", polled, can_write, poll_err, poll_hup);

		if (poll_err || poll_hup)
		{
			io_reset();
		}
		else if (polled && can_write)
		{
			send(remote_socket, data, 2, 0);
		}
	}
}

int io_network_ready ()
{
	return -1;
}

void io_network_recv()
{

	int polled, can_read, poll_err, poll_hup;
	int polled_c, can_read_c, poll_err_c, poll_hup_c;
	struct pollfd server_poll_s;
	struct pollfd client_poll_s;
	char data[2] = {'\0', '\0'};
	int n_bytes = 0;

	//debugf("io_network_recv\n");

	if ((server_sock != -1 && client_sock == -1) || 1)
	{

		server_poll_s.fd = server_sock;
		server_poll_s.events = POLLIN;
		server_poll_s.revents = 0;

		polled = poll(&server_poll_s, 1, 0);
		can_read = server_poll_s.revents & POLLIN;
		poll_err = server_poll_s.revents & POLLERR;
		poll_hup = server_poll_s.revents & POLLHUP;

		if (poll_err || poll_hup)
		{
			io_reset();
		}
		else if (polled && can_read)
		{
			if ((client_sock = accept(server_sock, NULL, NULL)) < 0)
			{
				debugf("link: failed to accept remote connection\n");
				io_reset();
				return;
			}
			debugf("link: accepted %d\n", client_sock);
			io_network_try_open_remote_connection();
		}
	}

	if (client_sock != -1)
	{
		client_poll_s.fd = client_sock;
		client_poll_s.events = POLLIN;
		client_poll_s.revents = 0;

		polled_c = poll(&client_poll_s, 1, 0);
		can_read_c = client_poll_s.revents & POLLIN;
		poll_err_c = client_poll_s.revents & POLLERR;
		poll_hup_c = client_poll_s.revents & POLLHUP;

		debugf("io_network_ready: %d %d %d %d\n", polled_c, can_read_c, poll_err_c, poll_hup_c);

		if (poll_err_c || poll_hup_c)
		{
			io_reset();
		}
		else if (polled_c && can_read_c)
		{
			n_bytes = read(client_sock, data, 2);
			debugf("read: %d - %d - %d - '%s' - %d - %d - %d\n", client_sock, EBADF == n_bytes, n_bytes, data, (int)data[0], (int)data[1], errno);

			if (n_bytes > 0)
			{
				un8 own_data = io_network_trigger_int(data[1]);
				if (data[0] & 1)
				{
					un8 mode = (un8) SEND_PASSIVE;
					io_network_send_mode(&mode, &own_data);
				}
			}
			else if (n_bytes == 0) // (read(...) == 0) == EOF => reset io stack
			{
				io_reset();
			}
		}
	}
}

un8 io_network_trigger_int(un8 byte)
{
	un8 old_data = R_SB;
	R_SB = byte;
	hw_interrupt(IF_SERIAL, IF_SERIAL);
	hw_interrupt(0, IF_SERIAL);
	return old_data;
}

void io_network_shutdown()
{
	debugf("io_network_shutdown\n");

	free(data);

	if (server_sock > -1)
	{
		close(server_sock);
		server_sock = -1;
	}

	if (client_sock > -1)
	{
		close(client_sock);
		client_sock = -1;
	}

	if (remote_socket > -1)
	{
		close(remote_socket);
		remote_socket = -1;
	}
}
