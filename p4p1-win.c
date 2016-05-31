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

#pragma comment(lib, "ws2_32.lib")	//winsock library

#define BUFSIZE 1024
#define str(a) #a
#define xstr(a) str(a)

#if 0
/*
 * Struct exposed from in.h
 */

/* Internet address */
struct in_addr {
	unsigned int s_addr;
};

/* Internet style socket address */
struct sockaddr_in {
	unsigned short int sin_ family;	/* Address family */
	unsigned short int sin_port;	/* Port number */
	struct in_addr sin_addr;		/* IP address */
	unsigned char sin_zero[...]		/* Pad to size of 'struct sockaddr' */
};

/*
 * Struct exported from netdb.h
 */

/* Domain name service (DNS) host entry */
struct hostent {
	char	*h_name;		/* official name of host */
	char	**h_aliases;	/* alias list */
	int		h_addrtype;		/* host address type */
	int		h_length;		/* lenght of address */
	char	**h_addr_list;	/* list of addresses */
}
#endif

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
    	SOCKET s;
    	struct sockaddr_in server;
	char buf[BUFSIZE]; 
	char c;
	int i;

    	if (WSAStartup(MAKEWORD(2,2),&wsa) != 0) {
		printf("Failed. Error Code : %d",WSAGetLastError());
		return 1;
    	}
     
    	//Create a socket
    	if((s = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET) {
        	printf("Could not create socket : %d" , WSAGetLastError());
    	}
     
     
    	server.sin_addr.s_addr = inet_addr(hostname);
    	server.sin_family = AF_INET;
    	server.sin_port = htons(portno);
 
    	//Connect to remote server
    	if (connect(s , (struct sockaddr *)&server , sizeof(server)) < 0) {
        	puts("connect error");
        	return 1;
    	}

	/*for(i = 0; i < 1024 && (c = getchar()) != EOF; i++){
		buf[i] = c;
	}*/

	fscanf(stdin, " %s", buf);
	
	int sent = 0;
	while(sent < BUFSIZE){
		char *ptr = &buf[sent];
		send(s, ptr, strlen(buf), 0);
		sent++;
	}

	printf("%s", buf);

	return 0;
}

int server(int portno)
{

	return 0;
}
void usage()
{
	printf("");
}
