/*
 * p4p1.c - A simple TCP server / client to run basic command on server
 * Usage:	p4p1 -s <port>	-> server
 * 		p4p1 -c <host> <port> -> client
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <io.h>
#include <winsock2.h>
#include <windows.h>

#define BUFSIZE 1024
#define str(a) #a
#define xstr(a) str(a)

/*
 * error - wrapper for perror
 */

void error(char *msg){
	perror(msg);
	exit(1);
}

int server(int portno);
int client(char * hostname, int portno);
void usage();

int main(int argc, char *argv[])
{
	int c, index;
	char *cvalue = NULL;

	opterr = 0;

	if(argc < 2){
		error("Not enought arguments\n");
	}

	while((c = getopt(argc, argv, "sch:")) != -1){
		switch(c){
			case 's':
				server(atoi(argv[2]));
				break;
			case 'c':
				client(argv[2], atoi(argv[3]));
				break;
			case 'h':
				usage();
				break;
		}
	}

	return 0;

}

int client(char *hostname, int portno)
{
	WSADATA wsa;
    	if (WSAStartup(MAKEWORD(2,2),&wsa) != NO_ERROR) {
		error("WSAStartup Error.");
    	}

	/*socket*/
	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(s == INVALID_SOCKET){
		error("error at socket.");
	}

	/*Connect*/
	struct sockaddr_in client;
	client.sin_family = AF_INET;
	client.sin_addr.s_addr = inet_addr(hostname);
	client.sin_port = htons(portno);

	if(connect(s, (SOCKADDR*)&client, sizeof(client)) == SOCKET_ERROR){
		error("Failed to connect.\n");
	}

	/*Send & Recv*/
	int bytesSent, bytesRecv = SOCKET_ERROR;
	char sendbuf[BUFSIZE] = "";
	char recvbuf[BUFSIZE] = "";

	printf("<p4p1 />");
	fscanf(stdin, " %s", sendbuf);
	bytesSent = send(s, sendbuf, strlen(sendbuf), 0);
	if(bytesSent == SOCKET_ERROR){
		error("send error");
	}

	while(bytesRecv == SOCKET_ERROR){
		bytesRecv = recv(s, recvbuf, BUFSIZE, 0);
		if(bytesRecv == 0 || bytesRecv == WSAECONNRESET){
			error("Connection Closed.\n");
		}

		if(bytesRecv < 0){
			exit(0);
		} else {
			printf("\n%s", recvbuf);
		}
	}

	WSACleanup();
	return 0;
}

int server(int portno)
{
WSADATA wsaData;
        int wsaerr = WSAStartup(MAKEWORD(2, 2), &wsaData);

        if(wsaerr != 0){
                error("Server : The Winsock dll not found!");
        }

        /*socket*/
        SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if(s == INVALID_SOCKET){
                error("error at socket");
        }

        /*bind*/
        struct sockaddr_in service;
        service.sin_family = AF_INET;
        service.sin_addr.s_addr = inet_addr("192.168.1.17");
        service.sin_port = htons(portno);

        if(bind(s, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR){
                closesocket(s);
                error("server bind failed");
        }

        /*listen*/
        if(listen(s, 10) == SOCKET_ERROR){
                printf("Server : listen(): Error istening on socket %ld.\n", WSA                                                                                        GetLastError());
        }

        /*accept*/
        SOCKET AcceptSocket;

        while(1){
                AcceptSocket = SOCKET_ERROR;
                while(AcceptSocket == SOCKET_ERROR){
                        AcceptSocket = accept(s, NULL, NULL);
                }
                printf("Server: Client Connected!\n");
                s = AcceptSocket;
                break;
        }
        while(1){
                int bytesSent;
                int bytesRecv = SOCKET_ERROR;
                char sendbuf[BUFSIZE] = "";
                char recvbuf[BUFSIZE] = "";

                bytesRecv = recv(s, recvbuf, BUFSIZE, 0);

                if(bytesRecv == SOCKET_ERROR){
                        printf("Server : recv() error %ld.\n", WSAGetLastError()                                                                                        );
                        error("recv");
                }

                FILE *fp = popen(recvbuf, "r");
                char ch;
                int i = 0;
                while((ch = fgetc(fp)) != EOF){
                        sendbuf[i] = ch;
                        i++;
                }

                bytesSent = send(s, sendbuf, strlen(sendbuf), 0);

                if(bytesSent == SOCKET_ERROR){
                        error("send error");
                }

                pclose(fp);
        }
        WSACleanup();
	return 0;
}
void usage()
{
	printf("");
}
