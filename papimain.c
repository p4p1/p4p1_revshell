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
			while(1){

				loop = connection_handler(sock, inf);
				if (serverThread.ncurses){ clearmain(); }
				if(loop == 2){

					t_sock++;
					read(fd[0], readBuf, sizeof(readBuf));
					int temp_s = atoi(readBuf);
					*(serverThread.saved_sockets+t_sock) = temp_s;

				} else if(loop == 1) {

					break;

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

/*
 * read socket and print to screen
 */
void randptoScreen(int t)
{
	sleep(1);
	char readingBuf[BUFSIZE];
	for(int i = 0; i < BUFSIZE; i++){ readingBuf[i] = '\0'; }

	read(*(serverThread.saved_sockets+t), readingBuf, BUFSIZE);

	if(serverThread.cmd){

		printf("%s\n", readingBuf);

	} else if(serverThread.ncurses){
		mvprintw(8, 0, "%s", readingBuf);
		refresh();
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

/*
 * accept and setup the connection
 */
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

/*Print startup screen*/
void printFirstScreen(struct server_info * inf)
{

	if(serverThread.ncurses){
		getmaxyx(stdscr, row, col);

		printlogo(inf);
		mvprintw(((row) / 2)-1, (col-30)/2, "Hello %s You are", inf->username);
		mvprintw((row) / 2, (col-35)/2,"listening on %s:%d", inf->ip, inf->portno);
		mvprintw( (row)-1, 0, "" );
		refresh();
	} else if(serverThread.cmd){
		printlogo(inf);
		printf("[!] Listening on %s:%d\n", inf->ip, inf->portno);
	}

}

/*
 * Print entering connections that got accepted
 */
void printAcceptedConnection(struct server_info * inf, char * readBuf)
{
	if(serverThread.ncurses){

		clear();
		printlogo(inf);
		mvprintw(6, 0, "[*] Connection from %s:%d\n", readBuf, inf->portno);
		refresh();

	} else if(serverThread.cmd){

		printf("[*] Connection from %s:%d\n", readBuf, inf->portno);

	}
}

/*
 * print the prompt on the screen
 */
void printPrompt(int s)
{

	if(serverThread.ncurses){
		getmaxyx(stdscr, row, col);
		clastrow();
		mvprintw(row-1, 0, "<p4p1 -%d-> ", s);
		getstr(serverThread.buf);
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
	if (serverThread.cmd){
		printf("Help:\n");
		printf("	- Help -> show this message\n");
		printf("	- update -> accept new connection!\n");
		printf("	- download -> download a file as bin.exe\n");
		printf("	change its name if you want an other extention\n");
	} else if(serverThread.ncurses) {
		mvprintw(8, 0, "Help:\n");
		mvprintw(9, 0, "	- Help -> show this message\n");
		mvprintw(10, 0, "	- update -> accept new connection!\n");
		mvprintw(11, 0, "	- download -> download a file as bin.exe\n");
		mvprintw(12, 0, "	change its name if you want an other extention\n");
	}
}

/*
 * download file as bin.exe
 */
void download(int sock)
{
	char dlbuf[3] = "**\n";
	char readPrompt[10];
	char link[BUFSIZE];
	write(sock, dlbuf, strlen(dlbuf));

	read(sock, readPrompt, sizeof(readPrompt));
	if(serverThread.cmd){
		printf("%s ", readPrompt);
		fgets(link, BUFSIZE, stdin);
	} else if(serverThread.ncurses){
		getmaxyx(stdscr, row, col);
		clastrow();
		mvprintw(row-1, 0, "%s ", readPrompt);
		refresh();
		getstr(link);
	}
	write(sock, link, strlen(link));
	write(sock, link, strlen(link));

	if(serverThread.cmd){
		printf("[*] File downloaded\n");
	} else if(serverThread.ncurses){
		clearmain();
		mvprintw(8, 0, "[*] File downloaded\n");
		refresh();
	}
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
		char chclin[BUFSIZE];
		getmaxyx(stdscr, row, col);
		mvprintw(row-1, 0,"CliNum -%c- -%d- #> ", serverThread.cliNum, *t);
		fgets(chclin, BUFSIZE, stdin);

		if(!strcmp(chclin, "+\n")){

			*t += 1;

		} else if(!strcmp(chclin, "-\n")){

			*t -= 1;

		} else{
			mvprintw(8, 0, "[!] Unknown command !");
		}
	}
}

/*
 * function that detrmines the entered commands
 */
int commandInterpreter(struct server_info * inf, int * t)
{

	int inc = *t;
	if (serverThread.ncurses){ clearmain(); }
	if(!strcmp(serverThread.buf, "help\n") || !strcmp(serverThread.buf, "help")){
		help();
	} else if(!strcmp(serverThread.buf, "exit\n") || !strcmp(serverThread.buf, "exit")){

		char * leave = "&&";
		write(*(serverThread.saved_sockets+ inc), leave, strlen(leave));
		return 1;

	} else if(!strcmp(serverThread.buf, "change\n") || !strcmp(serverThread.buf, "change")){

		changeClient(t);

	} else if(!strcmp(serverThread.buf, "update\n") || !strcmp(serverThread.buf, "update")){

		return 2;

	} else if(!strcmp(serverThread.buf, "download\n") || !strcmp(serverThread.buf, "download")){

		download(*(serverThread.saved_sockets+inc));

	} else if(serverThread.buf[0] == 'c' && serverThread.buf[1] == 'd'){

		char prompt[80];
		char dir[BUFSIZE];
		for(int i = 3, q = 0; i < strlen(serverThread.buf); i++, q++){
			dir[q] = serverThread.buf[i];
		}
		write(*(serverThread.saved_sockets+inc), serverThread.buf, strlen(serverThread.buf));
		read(*(serverThread.saved_sockets+inc), prompt, sizeof(prompt));

		for(int i = 0; i < 80; i++){ prompt[i] = '\0';}

		write(*(serverThread.saved_sockets+inc), dir, strlen(dir));
		read(*(serverThread.saved_sockets+inc), prompt, sizeof(prompt));

		if(serverThread.cmd){
			printf("%s\n", prompt);
		} else if (serverThread.ncurses){
			mvprintw(8, 0,"%s", prompt);
		}

	} else {
		write(*(serverThread.saved_sockets+inc), serverThread.buf, strlen(serverThread.buf));
	}

	return 0;

}
