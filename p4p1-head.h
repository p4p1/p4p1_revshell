#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include <curl/curl.h>

#define str(a) #a
#define xstr(a) str(a)
#define BUFSIZE 1024

/*
 * get buf and check if p4p1 custom command
 */
int iscommand(char buf)
{
	if(buf == '*'){
		return 1;
	} else {
		return 0;
	}
}

void processcommand(char * buf)
{
	if(!strcmp(buf, "*help")){

	}
}
