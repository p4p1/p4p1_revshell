#include "papiarg.h"

/*
 * Verrify each arguments and send to the correct part of program
 */
void check_arg(struct server_info * inf, char * argv[], int argc)
{
	if(argc >= 2){

		if(!strcmp(argv[1], "-usb")){

			if(argc >= 3){

				if(strlen(argv[2]) > 0 && strlen(argv[2]) < 50){
					printf("usb\n");
				} else {
					usage(argv[0]);
				}

			} else {

				usage(argv[0]);
				error("Not enought args", 1);

			}

		} else if(!strcmp(argv[1], "-c")){

			printf("Check for connections and send notif");

			if(argc >= 3){
				printf("Email notification at %s", argv[2]);
			} else {
				printf("On screen notification");
			}

		} else if(!strcmp(argv[1], "-gtk")){

			// Gtk invironement with gui interface
			inf->argo.graphic++;
			printf("IN PROGRESS\n");
			usage(argv[0]);

		} else if(!strcmp(argv[1], "-v")){

			// Verbose terminal mode no ncurses
			// nor gtk
			inf->argo.cli++;
			printf("IN PROGRESS\n");
			usage(argv[1]);

		} else if(!strcmp(argv[1], "-cli")){

			inf->argo.cli++;
			main_loop(inf);

		} else {

			usage(argv[0]);

		}

	} else {

		/*
		 * Main function
		 **/
		inf->argo.ncr++;
		initscr();
		keypad(stdscr, TRUE);
		main_loop(inf);
		refresh();
		getch();
		endwin();

	}

}
