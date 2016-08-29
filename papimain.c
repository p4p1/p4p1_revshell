#include "papimain.h"

int main_loop(struct server_info * inf)
{
	int c, new_s, *new_sock;
	char * sessionid = "5";
	bnlisten(inf);
	printlogo();
	printw("Listening on %s:%d\n", inf->ip, inf->portno);
	refresh();

	int * pbs = &inf->byteSent;
	int * pbr = &inf->byteRecv;

	c = sizeof(struct sockaddr_in);
	while( (new_s = accept(inf->s, (struct sockaddr *)&inf->client, (socklen_t*)&c)) ){

		inf->hostaddrp = inet_ntoa(inf->client.sin_addr);
		if(inf->hostaddrp == NULL){
			error("inet_ntoa()", 1);
		}

		printw("Connection from %s:%d\nSending client Id", inf->hostaddrp, inf->portno);
		refresh();
		write(new_s, sessionid, strlen(sessionid));

		pthread_t sniffer_thread;
		new_sock = malloc(1);
		*new_sock = new_s;

		if( pthread_create( &sniffer_thread, NULL, connection_handler, (void *) new_sock) < 0 ){
			error("thread error", 1);
		}


	}

	if(new_s < 0){
		error("accept", 1);
	}

	return 0;
}

void *connection_handler(void * sock)
{
	int s = *(int *) sock;
	int leaveloop = 0;
	int pbs, pbr;

	char buf[BUFSIZE];

	while(!leaveloop){

		bzero(buf, BUFSIZE);

		printw("\n<p4p1 -%d-> ", s);
		refresh();

		scanw("%s", buf);

		pbs = send(s, buf, strlen(buf), 0);
		if(pbs == SOCKET_ERROR){
			printw("Cant send data! Client down\n");
			break;
		} else {

			if( buf[0] == '&' ){

				write(s, buf, BUFSIZE);
				quit(0, s);

			} else if( buf[0] == '*' ){

				write(s, buf, BUFSIZE);
				bzero(buf, BUFSIZE);

				//Read url prompt
	            		read(s, buf, BUFSIZE);
				printw("%s", buf);
				bzero(buf, BUFSIZE);
				scanw("%s", buf);

				write(s, buf, BUFSIZE);
				bzero(buf, BUFSIZE);
				read(s, buf, BUFSIZE);
				printw("%s", buf);
				bzero(buf, BUFSIZE);

			} else {

				bzero(buf, BUFSIZE);
				read(s, buf, BUFSIZE);
				printw("%s", buf);

			}

		}

	}

	refresh();
	free(sock);
	quit(0, s);

}

void bnlisten(struct server_info * inf)
{

	if( bind(inf->s, (struct sockaddr *)&inf->server, sizeof(inf->server)) < 0 ){
		error("bind failed", 1);
	}

	listen(inf->s, 3);

}
