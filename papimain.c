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
		printFirstScreen(inf);
		init_threads(inf);
		sleep(NUMOCLIENTS + 1);


		char readBuf[80];
		char readSock[15];
		int sock = 0;
		serverThread.allDone = 1;
		while (1){

			readBuf[0] = '0';
			do {
				read(fd[0], readBuf, sizeof(readBuf));
			} while(readBuf[0] != '1'); // client connected
			serverThread.cliNum++;

			for(int i = 0; i < 80; i++){
				readBuf[i] = '\0';
			}

			for(int i = 0; i < 80; i++){ readBuf[i] = '\0'; }
			while(readBuf[0] == '\0' || readBuf[0] == '1') // read client ip
				read(fd[0], readBuf, sizeof(readBuf));

			printAcceptedConnection(inf);

			for(int i = 0; i < 15; i++){ readSock[i] = '\0'; }
			read(fd[0], readSock, sizeof(readSock));
			readSock[1] = '\0';
			*(serverThread.saved_sockets + sock) = atoi(readSock);

			do {
				read(fd[0], readBuf, sizeof(readBuf));
			} while (readBuf[0] != '0');	//read quit var


			if(serverThread.cliNum == '1'){
	 			serverThread.connectedTo = 0;
	 			pthread_join(serverThread.onConnect[serverThread.connectedTo], NULL);
			}
			sock++;
		}
 	} else if (pID < 0) {

 		error("Cant fork", -1);

 	} else { // father
 		close(fd[0]);
		sleep(NUMOCLIENTS + 5);

		char connectionAccepted[3];
		char resetConnectionA[3];
		char sockstr[15];
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

			sprintf(sockstr, "%d", new_s);
			sock++;

			serverThread.cliNum++;
 			inf->hostaddrp = inet_ntoa(inf->client.sin_addr);
 			if(inf->hostaddrp == NULL){
 				error("inet_ntoa()", 1);
 			}
			*(inf->hostaddrp + (strlen(inf->hostaddrp))) = '\0';

			for(i = 0; i <= strlen(inf->hostaddrp); i++){
				sendbuf[i] = *(inf->hostaddrp+i);
			}
			//sendbuf[i] = '\0';

 			write(fd[1], connectionAccepted, sizeof(connectionAccepted));
			sleep(1);
			write(fd[1], sendbuf, strlen(sendbuf));
			sleep(1);
			write(fd[1], sockstr, strlen(sockstr));
 			write(new_s, sessionid, strlen(sessionid));
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
	char buf[BUFSIZE];

	printConHandler(t);
	while(( t =! serverThread.connectedTo) && (serverThread.allDone))
		;

	while(! leaveloop){
		sleep(1);

		printPrompt(s);
		mvprintw(3, col - (col/2), "[ All Clients ready to be connected    ]\n");
		mvprintw(row-1, 12, "");
		refresh();
		mvscanw(row-1, 12, "%s", buf);
		clearmain();

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

void printFirstScreen(struct server_info * inf)
{
	if(inf->argo.ncr){
		getmaxyx(stdscr, inf->win.row, inf->win.col);

		printlogo(inf);
		mvprintw(((inf->win.row) / 2)-1, (inf->win.col-30)/2, "Hello %s You are", inf->username);
		mvprintw((inf->win.row) / 2, (inf->win.col-35)/2,"listening on %s:%d", inf->ip, inf->portno);
		mvprintw( (inf->win.row)-1, 0, "" );
		refresh();
	} else if(inf->argo.cli){
		printf("[!] Listening on %s:%d\n", inf->ip, inf->portno);
	}
}

void printAcceptedConnection(struct server_info * inf, char * readBuf)
{
	if(inf->argo.ncr){

		clear();
		printlogo(inf);
		mvprintw(6, 0, "[*] Connection from %s:%d\n", readBuf, inf->portno);
		refresh();

	} else if(inf->argo.cli){

		printlogo(inf);
		printf("[*] Connection from %s:%d\n", readBuf, inf->portno);

	}
}

void printConHandler(int t)
{
	if(serverThread.ncurses){
		getmaxyx(stdscr, row, col);
		mvprintw(3, col - (col/2), "[ Initializing Client Conectors no: %d ]\n", t);
		mvprintw(7, 0, "");
		refresh();
	} else if(serverThread.cmd){
		printf("[ Initializing Client Conectors no: %d ]\n", t):
	}
}

void printPrompt(int s)
{
	if(serverThread.ncurses){
		int row, col;
		getmaxyx(stdscr, row, col);
		mvprintw(row-1, 0, "<p4p1 -%d-> ", s);
		refresh();
	} else if (serverThread.cmd) {
		printf("\n<p4p1 -%d-> ", s);
	}
}
