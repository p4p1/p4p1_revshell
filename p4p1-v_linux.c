#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <pthread.h>

#define BUFSIZE 9999
#define SOCKET_ERR -1

void *connection_handler(void *);
int int_to_str(int value, char *buf, unsigned int len);

/*Main vpn server*/
int main(int argc, char *argv[])
{
	int portno;
	char portchar[16];
	FILE * fp = fopen("port.cfg", "r");

	if(fp == NULL){
		portno = 4442;
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

    	int s, new_s, c, *new_sock;
    	struct sockaddr_in server, client;
	char * hostaddrp;

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons( portno );

	printf("[*] Listening on %s:%d\n", inet_ntoa(server.sin_addr), portno);
	//Create socket:
    	s = socket(AF_INET, SOCK_STREAM, 0);
    	if(s == -1){
        	printf("Could not create socket.\n");
    	}

    	if( bind(s, (struct sockaddr *)&server, sizeof(server)) < 0 ){
        	puts("bind failed.");
        	return 1;
    	}

    	//Listen
    	listen(s, 3);

    	//Accept and incoming connection
    	printf("Waiting for incoming connections on LOCALHOST:%d", portno);
    	c = sizeof(struct sockaddr_in);
    	while( (new_s = accept(s, (struct sockaddr *)&client, (socklen_t*)&c)) ){

        	hostaddrp = inet_ntoa(client.sin_addr);

        	if(hostaddrp == NULL){
            		perror("Error in inet_ntoa()");
        	}

        	printf("\n[!] Connection accepted from: %s:%d\n", hostaddrp, portno);

        	pthread_t sniffer_thread;
        	new_sock = malloc(1);
        	*new_sock = new_s;

        	if( pthread_create( &sniffer_thread, NULL, connection_handler, (void*) new_sock ) < 0 ){
            		perror("could not create thread.");
            		return 1;
        	}

        	printf("[!] Handler assigned to %s:%d\n\tOn socket: %d\n", hostaddrp,portno, new_s);
    	}

    	if(new_s < 0){
        	perror("accept failed");
        	return 1;
    	}

    	return 0;
}

/*
 *This will handle the connections for each client.
 **/
void *connection_handler(void *socket_desc)
{
	int s = *(int *) socket_desc;
	int bytesSent, bytesRecv, nc;
	char buf[BUFSIZE];
	int portno;
	char ui[BUFSIZE] = "      _ _       _\n _ __| | | _ __/ |_ ___ __ _ _\n| '_ \\_  _| '_ \\ \\ \\V / '_ \\ ' \\\n| .__/ |_|| .__/_|\\\\_/| .__/_||_|\n|_|       |_|        |_|    \n\0";

	int * pbs = &bytesSent;
	int * pbr = &bytesRecv;

	*pbs = send(s, ui, BUFSIZE, 0);

	bzero(buf, BUFSIZE);
	strcpy(buf, "[*] Enter port:\n");
	*pbs = send(s, buf, BUFSIZE, 0);

	bzero(buf, BUFSIZE);
	*pbr = SOCKET_ERR;
	while(*pbr == SOCKET_ERR){
		*pbr = recv(s, buf, BUFSIZE, 0);
			if(*pbr == 0){
				break;
			}

			if(bytesRecv < 0){
				return 0;
			} else {
				portno = atoi(buf);
				bzero(buf, BUFSIZE);
			}
	}

	strcpy(buf, "[*] are you using netcat?(y/n)\n");
	*pbs = send(s, buf, BUFSIZE, 0);

	bzero(buf, BUFSIZE);
	*pbr = SOCKET_ERR;
	while(*pbr == SOCKET_ERR){
		*pbr = recv(s, buf, BUFSIZE, 0);
			if(*pbr == 0){
				break;
			}

			if(bytesRecv < 0){
				return 0;
			} else {
				if(buf[1] == 'n'){
					nc = 0;
				} else {
					nc = 1;
				}
			}
	}

	strcpy(buf, "[*] done now waiting for client.....\n");
	*pbs = send(s, buf, BUFSIZE, 0);

	if(nc == 1){
		/*vpn custom p4p1 server.*/

		int sock, c, new_s;
		struct sockaddr_in server, client;

		server.sin_family = AF_INET;
		server.sin_addr.s_addr = INADDR_ANY;
		server.sin_port = htons( portno );

		sock = socket(AF_INET, SOCK_STREAM, 0);
		if(sock == -1){
			bzero(buf, BUFSIZE);
			strcpy(buf, "[!] Could not create socket\n");
			*pbs = send(s, buf, BUFSIZE, 0);
	    	}

		if( bind(sock, (struct sockaddr *)&server, sizeof(server)) < 0 ){
			bzero(buf, BUFSIZE);
			strcpy(buf, "[!] Could not bind\n");
			*pbs = send(s, buf, BUFSIZE, 0);
		}

		bzero(buf, BUFSIZE);
		strcpy(buf, "[*] listening\n");
		*pbs = send(s, buf, BUFSIZE, 0);

		listen(sock, 3);

		c = sizeof(struct sockaddr_in);
		while( (new_s = accept(sock, (struct sockaddr *)&client, (socklen_t*)&c)) ){

			sock = new_s;

			bzero(buf, BUFSIZE);
			strcpy(buf, "0");
			*pbs = send(sock, buf, BUFSIZE, 0);

			/*recv ui*/
			bzero(buf, BUFSIZE);
			*pbr = SOCKET_ERR;
			while(*pbr == SOCKET_ERR){
				*pbr = recv(sock, buf, BUFSIZE, 0);
					if(*pbr == 0){
						break;
					}

					if(bytesRecv < 0){
						return 0;
					} else {
						*pbs = send(s, buf, BUFSIZE, 0);
					}
			}
			/*recv prompt*/
			while(1){
				bzero(buf, BUFSIZE);
				*pbr = SOCKET_ERR;
				while(*pbr == SOCKET_ERR){
					*pbr = recv(sock, buf, BUFSIZE, 0);
						if(*pbr == 0){
							break;
						}

						if(bytesRecv < 0){
							return 0;
						} else {
							*pbs = send(s, buf, BUFSIZE, 0);
							bzero(buf, BUFSIZE);
						}
				}
				bzero(buf, BUFSIZE);
				*pbr = SOCKET_ERR;
				while(*pbr == SOCKET_ERR){
					*pbr = recv(s, buf, BUFSIZE, 0);
						if(*pbr == 0){
							break;
						}

						if(bytesRecv < 0){
							return 0;
						} else {
							*pbs = send(sock, buf, BUFSIZE, 0);
							bzero(buf, BUFSIZE);
						}
				}
			}

		}

		/*end of custom p4p1 server*/

	}
	free(socket_desc);
	return 0;
}
