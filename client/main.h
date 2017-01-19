#ifndef _MAIN_H
#define _MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>

#define DEFAULT_PORT 4441
#define DEFAULT_IP "127.0.0.1"
#define DEFAULT_IP_SIZE 9
#define _DEBUG

struct main_struct {

	int port;
	int cn;
	char * ip;
	char * buf;

	SOCKET s;
	WSADATA wsa;

	struct sockaddr_in server;

};

void error(char *);
void debug(struct main_struct *);
void first_line_to_buffer(FILE *, char *);

#endif
