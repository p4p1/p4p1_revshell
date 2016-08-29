#ifndef p4p1_h
#define p4p1_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <ncurses.h>
#include <pthread.h>

#define BUFSIZE 9999

struct window {

	int row, col;

};

struct server_info {

	int s;	//socket desc

	int portno;
	char ip[16];
	char * username;
	char * hostaddrp;

	struct window win;
	struct sockaddr_in server, client;

};

void error(char * msg, int num);
void init_variables(struct server_info * inf, char * argv[]);
void usage(char * exname);

#endif
