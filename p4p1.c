/*
 * p4p1.c - A simple TCP server / client to run basic command on server
 * Usage:	p4p1 -s <port>	-> server
 * 			p4p1 -c <host> <port> -> client
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#define BUFSIZE 1024


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

int server(int portno, int invs);
int client(char * hostname, int portno);
void usage();

int main(int argc, char *argv[])
{
	int c, invs;					//create basic char
	opterr = 0;

	while((c = getopt(argc, argv, "isch:")) != -1){			//loop to get all arguments
		switch(c){										// check for every argument in case:
			case 'i':									/* -i is  */
				invs = 1;
				break;
			case 's':									/* -s is for the server */
				if(argv[2] == NULL)
					error("Not enought args");
				server(atoi(argv[2]), invs);
				break;
			case 'c':									/* -c is for the client */
				if(argv[2] == NULL || argv[3] == NULL)
					error("Not enought args");
				client(argv[2], atoi(argv[3]));
				break;
			case 'h':									/* -h is for help */
				usage();
				break;
			case '?':
				usage();
				break;
			default:
				usage();
		}
	}

	return 0;
}

/*
 * this is the client side
 */

int client(char *hostname, int portno)
{
	int sockfd, n;
	struct sockaddr_in serveraddr;
	struct hostent *server;
	char buf[BUFSIZE];

	/* socket: create the socket */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
		error("ERROR opening socket");

	/* gethostbyname: get the server's DNS entry */
	server = gethostbyname(hostname);
	if (server == NULL) {
		fprintf(stderr,"ERROR, no such host as %s\n", hostname);
		exit(0);
	}

	/* build the server's Internet address */
	bzero((char *) &serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, 
		(char *)&serveraddr.sin_addr.s_addr, server->h_length);
	serveraddr.sin_port = htons(portno);

	/* connect: create a connection with the server */
	if (connect(sockfd, &serveraddr, sizeof(serveraddr)) < 0) 
		error("ERROR connecting");
	/* get message line from the user */
	printf("<p4p1__> ");
	bzero(buf, BUFSIZE);
	fgets(buf, BUFSIZE, stdin);

	/* send the message line to the server */
	n = write(sockfd, buf, strlen(buf));
	if (n < 0) 
		error("ERROR writing to socket");

	/* print the server's reply */
	bzero(buf, BUFSIZE);
	n = read(sockfd, buf, BUFSIZE);
	if (n < 0) 
		error("ERROR reading from socket");
	printf("%s", buf);

	close(sockfd);
	return 0;
}

/*
 * this is the server side
 */

int server(int portno, int invs)
{
	int parentfd; /* parent socket */
  	int childfd; /* child socket */
  	int clientlen; /* byte size of client's address */
  	struct sockaddr_in serveraddr; /* server's addr */
  	struct sockaddr_in clientaddr; /* client addr */
  	struct hostent *hostp; /* client host info */
  	char buf[BUFSIZE]; /* message buffer */
  	char *hostaddrp; /* dotted decimal host addr string */
  	int optval; /* flag value for setsockopt */
  	int n; /* message byte size */

  	/* 
   	 * socket: create the parent socket 
  	 */
  	parentfd = socket(AF_INET, SOCK_STREAM, 0);
	if (parentfd < 0) 
		error("ERROR opening socket");

  	/* setsockopt: Handy debugging trick that lets 
  	 * us rerun the server immediately after we kill it; 
 	 * otherwise we have to wait about 20 secs. 
 	 * Eliminates "ERROR on binding: Address already in use" error. 
   	 */
  	optval = 1;
  	setsockopt(parentfd, SOL_SOCKET, SO_REUSEADDR, 
	     (const void *)&optval , sizeof(int));

  	/*
   	 * build the server's Internet address
   	 */
 	bzero((char *) &serveraddr, sizeof(serveraddr));

  	/* this is an Internet address */
  	serveraddr.sin_family = AF_INET;

  	/* let the system figure out our IP address */
  	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

  	/* this is the port we will listen on */
  	serveraddr.sin_port = htons((unsigned short)portno);

  	/* 
   	 * bind: associate the parent socket with a port 
  	 */
	if (bind(parentfd, (struct sockaddr *) &serveraddr, 
		sizeof(serveraddr)) < 0) 
		error("ERROR on binding");

  	/* 
  	 * listen: make this socket ready to accept connection requests 
  	 */
  	if (listen(parentfd, 5) < 0) /* allow 5 requests to queue up */ 
		error("ERROR on listen");

  	/* 
  	 * main loop: wait for a connection request, echo input line, 
  	 * then close connection.
  	 */
  	clientlen = sizeof(clientaddr);
 	while (1) {

		/* 
		 * accept: wait for a connection request 
		 */
		childfd = accept(parentfd, (struct sockaddr *) &clientaddr, &clientlen);
		if (childfd < 0) 
			error("ERROR on accept");
	    
		/* 
		 * gethostbyaddr: determine who sent the message 
		 */
		hostp = gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr, 
				  sizeof(clientaddr.sin_addr.s_addr), AF_INET);
		if (hostp == NULL)
			error("ERROR on gethostbyaddr");
		hostaddrp = inet_ntoa(clientaddr.sin_addr);
		if (hostaddrp == NULL)
			error("ERROR on inet_ntoa\n");
		printf("server established connection with %s (%s)\n", 
		   	hostp->h_name, hostaddrp);

		/* 
		 * read: read input string from the client
		 */
		bzero(buf, BUFSIZE);
		n = read(childfd, buf, BUFSIZE);
		if (n < 0) 
			error("ERROR reading from socket");
		printf("server received %d bytes: %s", n, buf);

		FILE *fp = popen(buf, "r");
		char ch;
		int i = 0;
		while((ch = fgetc(fp)) != EOF){
		buf[i] = ch;
			++;
		}
	
		n = write(childfd, buf, strlen(buf));
		if(n < 0)
			error("ERROR writing to socket");
	
		pclose(fp);
		close(childfd);
	}

	return 0;
}

void usage()
{
	printf("Usage:\n\tp4p1 -c 192.168.1.1 80\n\tp4p1 -s 80");
}
