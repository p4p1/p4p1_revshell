#ifndef papi_h
#define papi_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <netinet/in.h>

#include <ncurses.h>
#include <pthread.h>

#define BUFSIZE 9999
#define SOCKET_ERROR -1
#define NUMOCLIENTS 100

/*
 * Thread structure
 ***/
typedef struct {

	int cliNum;
	int * saved_sockets;
	int connectedTo;


	pthread_t onConnect[NUMOCLIENTS];
} serverthread;

/*
 * Window structure
 ***/
struct window {

	int row, col;

};

/*
 * Argument options structure
 ***/
struct arg_options {

	char * usb_path;

};

/*
 * Main variable structure
 ***/
struct server_info {

	int s;	//socket desc

	int portno;
	char ip[16];
	char username[50];
	char * hostaddrp;

	struct arg_options argo;
	struct window win;
	struct sockaddr_in server, client;

};

static serverthread serverThread;

/*
 * Function declarations see p4p1-s_linux.c
 * for each function details
 ***/
void init_variables(struct server_info * inf, char * argv[]);
void init_threads(struct server_info * inf, char * argv[]);
void pscreen(char * str, int x, int y);
void error(char * msg, int num);
void usage(char * exname);
void quit(int c, int s);
int reusesock(struct server_info * inf);
void printlogo();
void clastrow();
void clearmain();

#endif
