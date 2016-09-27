#include "papimain.h"

/*
 * p4p1 Main program loop
 ***/
int main_loop(struct server_info * inf)
{
	pid_t pID = fork();
	getmaxyx(stdscr, inf->win.row, inf->win.col);
	int * pcn = &serverThread.cliNum;



	if(pID == 0){	// child

		bnlisten(inf);
		printlogo(inf);
		mvprintw(((inf->win.row) / 2)-1, (inf->win.col-30)/2, "Hello %s You are", inf->username);
		mvprintw((inf->win.row) / 2, (inf->win.col-35)/2,"listening on %s:%d", inf->ip, inf->portno);
		mvprintw( (inf->win.row)-1, 0, "" );
		refresh();

		while(*pcn <= 0)
			;	// Wait for connection
		mvprintw(6, 0, "[*] Connection from %s:%d\n", inf->hostaddrp, inf->portno);
		refresh();

		init_threads(inf);

		serverThread.connectedTo = 0;
		pthread_join(serverThread.onConnect[serverThread.connectedTo], NULL);

	} else if (pID < 0) {

		error("Cant fork", -1);

	} else { // father
		int c, new_s;
		char * sessionid = "5";
		c = sizeof(struct sockaddr_in);
		while( (new_s = accept(inf->s, (struct sockaddr *)&inf->client, (socklen_t*)&c)) && (serverThread.cliNum < NUMOCLIENTS) ){

			serverThread.cliNum++;
			inf->hostaddrp = inet_ntoa(inf->client.sin_addr);
			if(inf->hostaddrp == NULL){
				error("inet_ntoa()", 1);
			}

			write(new_s, sessionid, strlen(sessionid));
			*(serverThread.saved_sockets + serverThread.cliNum) = new_s;
		}
	}

	return 0;
}

/*
 * p4p1 connection handler for user
 ***/
void *connection_handler(void * sock)
{
	int leaveloop = 0;
	//int pbs;
	//int row, col;

	//char buf[BUFSIZE];

	//getmaxyx(stdscr, row, col);

	while(! leaveloop){

		while(( *(int *) sock =! serverThread.connectedTo))
			;
	}

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
