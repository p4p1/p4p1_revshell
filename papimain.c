#include "papimain.h"

/*
 * p4p1 Main program loop
 ***/
 int main_loop(struct server_info * inf)
 {
 	pid_t pID;
 	int fd[2];
 	bnlisten(inf);

 	pipe(fd);
 	pID = fork();

 	if(pID == 0){	// child has to receive all of the variables
 		close(fd[1]);
		getmaxyx(stdscr, inf->win.row, inf->win.col);

		printlogo(inf);
 		mvprintw(((inf->win.row) / 2)-1, (inf->win.col-30)/2, "Hello %s You are", inf->username);
 		mvprintw((inf->win.row) / 2, (inf->win.col-35)/2,"listening on %s:%d", inf->ip, inf->portno);
 		mvprintw( (inf->win.row)-1, 0, "" );
 		refresh();

		sleep(1);

		char readBuf[80];
		while (1){

			readBuf[0] = 0;
			do {
				read(fd[0], readBuf, sizeof(readBuf));
			} while(readBuf[0] != '1'); // client connected
			serverThread.cliNum++;

			for(int i = 0; i < 80; i++){
				readBuf[i] = '\0';
			}

			while(readBuf[0] == '\0' || readBuf[0] == '1')
				read(fd[0], readBuf, sizeof(readBuf));

			clear();
			printlogo(inf);
			mvprintw(6, 0, "[*] Connection from %s:%d\n", readBuf, inf->portno);
			refresh();
			printw("hi m8"); refresh();

	 		init_threads(inf);

			do {
				read(fd[0], readBuf, sizeof(readBuf));
			} while (readBuf[0] != '0');

			if(serverThread.cliNum == '1'){
	 			serverThread.connectedTo = 0;
	 			pthread_join(serverThread.onConnect[serverThread.connectedTo], NULL);
			}

		}
 	} else if (pID < 0) {

 		error("Cant fork", -1);

 	} else { // father
 		close(fd[0]);

		char connectionAccepted[3];
		char resetConnectionA[3];
		connectionAccepted[0] = '1';
		resetConnectionA[0] = '0';

 		int c, i = 0, new_s, sock = 0;
 		char * sessionid = "5";
		char sendbuf[80];

		for(i = 0; i < 80; i++){
			sendbuf[i] = '\0';
		}

 		c = sizeof(struct sockaddr_in);
 		while( (new_s = accept(inf->s, (struct sockaddr *)&inf->client, (socklen_t*)&c)) && (sock < NUMOCLIENTS) ){

			serverThread.cliNum++;
 			inf->hostaddrp = inet_ntoa(inf->client.sin_addr);
 			if(inf->hostaddrp == NULL){
 				error("inet_ntoa()", 1);
 			}

			for(i = 0; i < strlen(inf->hostaddrp); i++){
				sendbuf[i] = *(inf->hostaddrp+i);
			}
			sendbuf[i] = '\0';
			*(inf->hostaddrp + i) = '\0';

 			write(fd[1], connectionAccepted, sizeof(connectionAccepted));
			sleep(1);
			write(fd[1], inf->hostaddrp, strlen(inf->hostaddrp)+1);
 			write(new_s, sessionid, strlen(sessionid));
 			*(serverThread.saved_sockets + sock) = new_s;
			sock++;
			write(fd[1], resetConnectionA, sizeof(resetConnectionA));
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
	int row, col;
	//int pbs;
	int t = *(int *) sock;
	int s = *(serverThread.saved_sockets+t);

	//char buf[BUFSIZE];

	getmaxyx(stdscr, row, col);

	while(! leaveloop){
		printw("im here %d\n", t); refresh();
		while(( t =! serverThread.connectedTo))
			;
		mvprintw(row-1, col-1, "<p4p1 -%d-> ", s);
		refresh();


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
