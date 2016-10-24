#include "papiarg.h"

/*
 * Verrify each arguments and send to the correct part of program
 */
void check_arg(struct server_info * inf, char * argv[], int argc)
{
	if(argc >= 2){

		if(!strcmp(argv[1], "-c")){
			// check for connection and send the notification to the screen and to a file :)

			if(argc >= 3){
				printf("Email notification at %s", argv[2]);
			} else {
				acandsendnotif(inf, 0);
			}

		} else if(!strcmp(argv[1], "-v")){

			// Verbose terminal mode no ncurses
			serverThread.cmd++;
			printf("IN PROGRESS\n");
			usage(argv[1]);

		} else if(!strcmp(argv[1], "-cli")){

			serverThread.cmd++;
			main_loop(inf);

		} else {

			usage(argv[0]);

		}

	} else {

		/*
		 * Main function
		 **/
		serverThread.ncurses++;
		initscr();
		keypad(stdscr, TRUE);
		main_loop(inf);
		refresh();
		getch();
		endwin();

	}

}

/*
 * accept a connection and send a notification on screen
 */
void acandsendnotif(struct server_info * inf, int flag)
{
	bnlisten(inf);
	int sock;
	char * sessionid = "5";
	char * leave = "&&";
	while (1){
		ansetupcon(inf, sock);
		write(*(serverThread.saved_sockets + sock), sessionid, strlen(sessionid));
		write(*(serverThread.saved_sockets + sock), leave, strlen(leave));
		printf("[!!!!!!]Accepted a client!!!!\n");
		if(flag)
			//send email
		sock++;
		getchar();
	}
}
