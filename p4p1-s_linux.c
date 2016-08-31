#include "papi.h"
#include "papimain.h"

int main(int argc, char *argv[])
{
	struct server_info inf;

	init_variables(&inf, argv);
	initscr();
	keypad(stdscr, TRUE);

	main_loop(&inf);

	refresh();

	getch();
	endwin();
	return 0;
}

void usage(char * exname)
{

	printf("Documentation:\n");
	printf("      %s Is a server for p4p1-o it listens on the port\n", exname);
	printf("      Configured by the installer, if you did not use the\n");
	printf("      Installer please download it from github(https://github.com/p4p1/p4p1)\n");
	printf("Program Usage:\n");
	printf("      When you run this app and a client connects\n");
	printf("      you can use special commands in the command prompt\n");
	printf("      The special ones are:\n");
	printf("          *url -> dowload from a url the path is requiered\n");
	printf("                  by this prompt #!url >\n");
	printf("          &q   -> quit the program correctly\n");
	printf("          !ch  -> Change client who you are connected to\n");
	printf("Command Usage:\n");
	printf("      %s -h    -> show this message\n", exname);

}

void error(char * msg, int num)
{
	endwin();
	perror(msg);
	exit(num);
}

void init_variables(struct server_info * inf, char * argv[])
{

	FILE * ipfile = fopen("/usr/.p4p1-o/cfg/ip.cfg", "r");
	FILE * portfile = fopen("/usr/.p4p1-o/cfg/port.cfg", "r");
	FILE * unamefile = fopen("/usr/.p4p1-o/cfg/un.cfg", "r");
	char fportbuf[50];
	char fipbuf[50];
	char funbuf[50];
	char c;
	int i;

	if( ipfile == NULL || portfile == NULL || unamefile == NULL) {
		usage(argv[0]);
		error("Cant file the files.", -1);
	} else {

		i = 0;
		while( ( c = fgetc(portfile) ) != EOF ){
			fportbuf[i] = c;
			i++;
		}
		fportbuf[i] = '\0';

		i = 0;
		while( ( c = fgetc(ipfile) ) != EOF ){
			fipbuf[i] = c;
			i++;
		}
		fipbuf[i-1] = '\0';

		i = 0;
		while( ( c = fgetc(unamefile) ) != EOF ){
			funbuf[i] = c;
			i++;
		}
		funbuf[i-1] = '\0';

		strcpy(inf->username, funbuf);
		strcpy(inf->ip, fipbuf);
		inf->portno = atoi(fportbuf);
	}

	inf->s = socket(AF_INET, SOCK_STREAM, 0);
	inf->server.sin_family = AF_INET;
	inf->server.sin_addr.s_addr = INADDR_ANY;
	inf->server.sin_port = htons( inf->portno );
	inf->cliNum = 0;

	if(inf->s == -1){
		error("Can't init socket.", -1);
	}

	if(inf->portno < 0 || inf->portno > 9999 ){
		error("Unknown port", -1);
	}

	if( strlen(inf->ip) < 0 ){
		error("Unknown ip", -1);
	}

	fclose(portfile);
	fclose(ipfile);
	fclose(unamefile);

}

void pscreen(char * str, int x, int y)
{
	mvprintw(y, x, "%s", str);
	refresh();
}

void quit(int c, int s)
{
	clear();
	close(s);
	printw("thank you for using byebye");
	refresh();
	getch();
	endwin();
	exit(c);
}

void printlogo(struct server_info * inf)
{

	getmaxyx(stdscr, inf->win.row, inf->win.col);

	mvprintw(0, 0, "       _ _       _\n");
    	mvprintw(1, 0, "  _ __| | | _ __/ |\n");
    	mvprintw(2, 0, " | '_ \\_  _| '_ \\ |\t\t[NumberOfClients: %d]\n",inf->cliNum);
    	mvprintw(3, 0, " | .__/ |_|| .__/_|\n");
    	mvprintw(4, 0, " |_|       |_|\n");

}

void clearmain()
{
	int row, col, i, q;
	getmaxyx(stdscr, row, col);

	for(q = 1; q < 20; q++){
		for(i = 0; i < col; i++){
			mvprintw(row-q, i, " ");
		}
	}
}

void clastrow()
{

	int row, col, i;
	getmaxyx(stdscr, row, col);

	for(i = 0; i < col; i++){
		mvprintw(row-1, i, " ");
	}

}
