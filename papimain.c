#include "papimain.h"

/*
 * p4p1 Main program loop
 ***/
int main_loop(struct server_info * inf)
{
	int c, new_s, *new_sock;
	char * sessionid = "5";

	erase();
	getmaxyx(stdscr, inf->win.row, inf->win.col);

	bnlisten(inf);
	printlogo(inf);
	mvprintw(((inf->win.row) / 2)-1, (inf->win.col-30)/2, "Hello %s You are", inf->username);
	mvprintw((inf->win.row) / 2, (inf->win.col-35)/2,"listening on %s:%d", inf->ip, inf->portno);
	mvprintw( (inf->win.row)-1, 0, "" );
	refresh();

	c = sizeof(struct sockaddr_in);
	while( (new_s = accept(inf->s, (struct sockaddr *)&inf->client, (socklen_t*)&c)) ){

		erase();

		inf->cliNum++;
		inf->hostaddrp = inet_ntoa(inf->client.sin_addr);
		if(inf->hostaddrp == NULL){
			error("inet_ntoa()", 1);
		}

		printlogo(inf);
		mvprintw(6, 0, "[*] Connection from %s:%d\n", inf->hostaddrp, inf->portno);
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

/*
 * p4p1 connection handler for user
 ***/
void *connection_handler(void * sock)
{
	int s = *(int *) sock;
	int leaveloop = 0;
	int pbs;
	int row, col;

	char buf[BUFSIZE];

	getmaxyx(stdscr, row, col);

	while(!leaveloop){

		bzero(buf, BUFSIZE);
		clastrow();

		mvprintw(row-1, 0, "<p4p1 -%d-> ", s);
		refresh();

		mvscanw(row-1, 12, "%s", buf);

		clearmain();
		mvprintw(9, 0, "");


		if(pbs == SOCKET_ERROR){
			mvprintw(row, col, "Cant send data! Client down\n");
			break;
		} else {

			if( buf[0] == '&' ){

				write(s, buf, BUFSIZE);
				quit(0, s);

			} else if( buf[0] == '*' ){

				write(s, "*\n\0", 3);
				bzero(buf, BUFSIZE);

				//Read url prompt
	            		read(s, buf, BUFSIZE);
				mvprintw(row-1, 0, "%s", buf);
				bzero(buf, BUFSIZE);
				mvscanw(row-1, 4, "%s", buf);

				write(s, buf, BUFSIZE);
				bzero(buf, BUFSIZE);
				read(s, buf, BUFSIZE);
				printw("%s", buf);
				bzero(buf, BUFSIZE);

			} else if ( buf[0] == 'c' && buf[1] == 'd' ){

				write(s, "cd\n\0", 4);
				bzero(buf, BUFSIZE);

				read(s, buf, BUFSIZE);
				mvprintw(row-1, 0, "%s", buf);
				bzero(buf, BUFSIZE);
				mvscanw(row-1, 4,"%s", buf);

				write(s, buf, BUFSIZE);
				bzero(buf, BUFSIZE);
				read(s, buf, BUFSIZE);
				printw("%s", buf);
				bzero(buf, BUFSIZE);

			} else {
				pbs = send(s, buf, strlen(buf), 0);
				bzero(buf, BUFSIZE);
				read(s, buf, BUFSIZE);
				mvprintw(8, 0, "%s", buf);

			}

		}

	}

	refresh();
	free(sock);
	quit(0, s);

	return 0;
}

/*
 * Bind and listening function
 **/
void bnlisten(struct server_info * inf)
{

	if( bind(inf->s, (struct sockaddr *)&inf->server, sizeof(inf->server)) < 0 ){
		error("bind failed", 1);
	}

	listen(inf->s, 3);

}
