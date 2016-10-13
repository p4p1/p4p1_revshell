#include "papimain.h"

/*
 * p4p1 Main program loop
 ***/
 int main_loop(struct server_info * inf)
 {

 	int sock = 0;
	char * sessionid = "5";
	char sockBuf[3];
	int fd[2];
	serverThread.allDone = 1;
	pid_t pidToConnection;

	pipe(fd);
	bnlisten(inf);
	printFirstScreen(inf);

	while (1) {
		loop:

		ansetupcon(inf, sock);
		printAcceptedConnection(inf, inf->hostaddrp);
		write(*(serverThread.saved_sockets + sock), sessionid, strlen(sessionid));

		//write to socket the session id to tell p4p1-o if in server or netcat
		serverThread.cliNum++;	//update the numofclient character
		if(serverThread.cliNum == '1')
			pidToConnection = fork();

		if(pidToConnection == 0){ // child
			close(fd[1]);

			int loop = 0;
			int t_sock = 0;
			char readBuf[3];
			serverThread.connectedTo = 0;
			while(loop != 1){

				loop = connection_handler(sock, inf);
				if(loop == 2){

					t_sock++;
					read(fd[0], readBuf, sizeof(readBuf));
					printf("\n\n\n%s\n\n\n", readBuf);
					int temp_s = atoi(readBuf);
					*(serverThread.saved_sockets+t_sock) = temp_s;

				} else {
					randptoScreen(sock);
				}


			}

			quit(0, *(serverThread.saved_sockets+sock));

		} else if(pidToConnection < 0){
			error("fork", -1);
		} else { //father

			if(serverThread.cliNum == '1'){
				close(fd[0]);
			} else {
				sprintf(sockBuf, "%d", *(serverThread.saved_sockets+sock));
				write(fd[1], sockBuf, sizeof(sockBuf));
			}

			sock++; // keep track of how many clients connected
			goto loop;

		}

	}



 	return 0;
 }

/*
 * p4p1 connection handler for user
 ***/
int connection_handler(int t, struct server_info * inf)
{

	int s = *(serverThread.saved_sockets+t);
	for(int i = 0 ; i < BUFSIZE; i++){ serverThread.buf[i] = '\0'; }


	while (t != serverThread.connectedTo)
		;
	printPrompt(s);

	return commandInterpreter(inf, &t);;


}

void randptoScreen(int t)
{
	char readingBuf[BUFSIZE];
	for(int i = 0; i < BUFSIZE; i++){ readingBuf[i] = '\0'; }

	read(*(serverThread.saved_sockets+t), readingBuf, BUFSIZE);

	if(serverThread.cmd){

		printf("%s\n", readingBuf);

	} else if(serverThread.ncurses){
		// for ncurses
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

/*
 * Change the client who you are sending data to :)
 * Not Working for now ;)
 */
void changeClient(int * t)
{
	if(serverThread.cmd){

		char chclin[BUFSIZE];
		printf("CliNum -%c- -%d- #> ", serverThread.cliNum, *t);
		fgets(chclin, BUFSIZE, stdin);

		if(!strcmp(chclin, "+\n")){

			*t += 1;

		} else if(!strcmp(chclin, "-\n")){

			*t -= 1;

		} else{
			printf("[!] Unknown command!\n");
		}

	} else if (serverThread.ncurses){
		// for ncurses
	}
}

int commandInterpreter(struct server_info * inf, int * t)
{

	int inc = *t;

	if(!strcmp(serverThread.buf, "help\n")){
		help();
	} else if(!strcmp(serverThread.buf, "whoami\n")){
		whoami();
	} else if(!strcmp(serverThread.buf, "exit\n")){

		char * leave = "&&";
		write(*(serverThread.saved_sockets+ inc), leave, strlen(leave));
		return 1;

	} else if(!strcmp(serverThread.buf, "change\n")){

		changeClient(t);

	} else if(!strcmp(serverThread.buf, "update\n")){

		return 2;

	} else {
		write(*(serverThread.saved_sockets+inc), serverThread.buf, strlen(serverThread.buf));
	}

	return 0;

}
