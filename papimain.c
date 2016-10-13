#include "papimain.h"

/*
 * p4p1 Main program loop
 ***/
 int main_loop(struct server_info * inf)
 {
 	bnlisten(inf);
	printFirstScreen(inf);

 	int sock = 0;
	char * sessionid = "5";
	serverThread.allDone = 1;

	while (1) {
		loop:

		ansetupcon(inf, sock);

		printAcceptedConnection(inf, inf->hostaddrp);

		write(*(serverThread.saved_sockets + sock), sessionid, strlen(sessionid));

		//write to socket the session id to tell p4p1-o if in server or netcat

		serverThread.cliNum++;	//update the numofclient character
		if(serverThread.cliNum == '1'){

			pid_t pidToConnection = fork();
			if(pidToConnection == 0){ // child

				serverThread.connectedTo = 0;
				connection_handler(sock, inf);

			} else if(pidToConnection < 0){
				error("Secondfork error", -1);
			} else { //father
				goto loop;
			}
			printf("out of dady\n");

		}
		sock++;	// keep track of how many clients connected
	}



 	return 0;
 }

/*
 * p4p1 connection handler for user
 ***/
void connection_handler(int t, struct server_info * inf)
{
	char * sessionid = "hy";
	write(*(serverThread.saved_sockets+t), sessionid, strlen(sessionid));
	printf("in conn handler\n");
	int s = *(serverThread.saved_sockets+t);
	int leaveloop = 0;

	printConHandler(t);

	for(int i = 0 ; i < BUFSIZE; i++){ serverThread.buf[i] = '\0'; }

	while(! leaveloop){

		while (t != serverThread.connectedTo)
			;
		printPrompt(s);
		commandInterpreter(inf, t);

	}


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

void ansetupcon(struct server_info * inf, int sock)
{
	int new_s;
	int c = sizeof(struct sockaddr_in);
	if( ((new_s = accept(inf->s, (struct sockaddr *)&inf->client, (socklen_t*)&c)) && (sock < NUMOCLIENTS)) > 0 ){

		*(serverThread.saved_sockets + sock) = new_s;

		inf->hostaddrp = inet_ntoa(inf->client.sin_addr);
		if(inf->hostaddrp == NULL){
			error("inet_ntoa()", 1);
		}


	} else {
		error("Accept failed", 1);
	}
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
		printlogo(inf);
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
		printf("[ Initializing Client Conectors no: %d ]\n", t);
	}

}

void printPrompt(int s)
{

	if(serverThread.ncurses){
		getmaxyx(stdscr, row, col);
		clastrow();
		mvprintw(row-1, 0, "<p4p1 -%d-> ", s);
		refresh();
	}
	if (serverThread.cmd) {
		printf("\n<p4p1 -%d-> ", s);
		fgets(serverThread.buf, BUFSIZE, stdin);
	}
}

/*
 * List out all of the server commands!
 */
void help()
{

}

/*
 * Print out computer nmae & last connected ip
 */
void whoami()
{

}

void commandInterpreter(struct server_info * inf, int t)
{
	if(!strcmp(serverThread.buf, "help\n")){
		help();
	} else if(!strcmp(serverThread.buf, "whoami\n")){
		whoami();
	} else if(!strcmp(serverThread.buf, "exit\n")){
		char * leave = "&&";
		write(*(serverThread.saved_sockets + t), leave, strlen(leave));
		quit(0, *(serverThread.saved_sockets + t));
	}
}
