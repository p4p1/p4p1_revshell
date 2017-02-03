#ifndef _MAIN_H
#define _MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include "default.h"

//#define _DEBUG

struct main_struct {

	int port;
	int cn;
	char * ip;
	char * cmd;
	char * buf;

	SOCKET s;
	WSADATA wsa;

	struct sockaddr_in server;

};

void error(char *);
#ifdef _DEBUG
void debug(struct main_struct *);
#endif
void first_line_to_buffer(FILE *, char *);

#endif
