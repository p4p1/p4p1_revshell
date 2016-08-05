#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>

#define BUFSIZE 9999

int main(int argc, char *argv[])
{

	int portno;
	char portchar[6];

	FILE * fp = fopen("port.cfg", "r");

	if(fp == NULL){
		portno = 4441;      //If no file set up default port to prevent errors
	} else {
		int i = 0;
		char c;
		while( (c = fgetc(fp)) != EOF){     // Get char from file while not at EOF
			portchar[i] = c;
			i++;
		}
		portchar[i+1] = '\0';               //Terminate string

		portno = atoi(portchar);            //Set up custom port
		fclose(fp);
	}

	printf("       _ _       _\n  _ __| | | _ __/ |\n | '_ \\_  _| '_ \\ |\n | .__/ |_|| .__/_|\n |_|       |_|\n");
	printf("[*] Setting up winsock\n");
	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2, 2), &wsa) != 0){
		printf("[!] Server : The winsock dll was not found!\n");
		getchar();
		return 0;
	}

	printf("[*] Creating socket\n");
	/*Create socket*/
	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if( s == INVALID_SOCKET){
		printf("[!] Server : Error at socket(): %ld\n", WSAGetLastError());
		WSACleanup();
		getchar();
		return 0;
	}

	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(portno);

	printf("[*] Binding");
	/*bind*/
	if(bind(s, (SOCKADDR*)&server, sizeof(server)) == SOCKET_ERROR){
		printf("[!] Server : bind Failed : %ld.\n", WSAGetLastError());
		getchar();
		closesocket(s);
		return 0;
	}

	printf("[*] Listening on port: %d\n", portno);
	/*listen*/
	if(listen(s, 10) == SOCKET_ERROR){
		printf("[!]Server:listen():Error listening to socket %ld.\n", WSAGetLastError());
		getchar();
	}

	/*Accept*/
	SOCKET as;

	printf("[*] waiting for client to connect\n");

	while(1){
		as = SOCKET_ERROR;
		while(as == SOCKET_ERROR){
			as = accept(s, NULL, NULL);
		}
		printf("[*] Server : Client Connected!\n");
		s = as;
		break;
	}

	/* Declare Variables */
	int bytesSent;
	int bytesRecv;
	char buf[BUFSIZE];

	/* pointer to variables to make syntacs faster */
	int * pbr = &bytesRecv;
	int * pbs = &bytesSent;

	*pbr = SOCKET_ERROR;

	strcpy(buf, "1");

	printf("[*] Send Client ID.\n");
	*pbs = send(s, buf, strlen(buf), 0);
	if( *pbs == SOCKET_ERROR ){
		printf("[!] Server : send error %ld.\n", WSAGetLastError());
	}

	while(1){

		memset(buf, 0, BUFSIZE);
		printf("<p4p1 -%d- />", s);
		fgets(buf, BUFSIZE, stdin);
		if( buf[0] == '&' ){
			*pbs = send(s, buf, strlen(buf), 0);
			if( *pbs == SOCKET_ERROR){
				printf("[!] Server: send bug, wait for client to restart.\n");
			}
			goto close;
		} else {
			*pbs = send(s, buf, strlen(buf), 0);
		        if( *pbs == SOCKET_ERROR ){
		                printf("[!] Server : send error %ld.\n", WSAGetLastError());
		        }

			memset(buf, 0, BUFSIZE);

			*pbr = recv(s, buf, BUFSIZE, 0);
			if( *pbr == SOCKET_ERROR ){
				printf("[!] Server : recv error %ld.\n", WSAGetLastError());
			}
			printf("%s\n", buf);
		}
	}

	close:
	WSACleanup();
	return 0;
}
