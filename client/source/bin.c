#include "../header/bin.h"

int process_command(struct main_struct *m_s)
{
	if(my_strcmp(m_s->cmd, "exit\n"))
		m_s->cn = SOCKET_ERROR;

	return 0;
}

int exe(struct main_struct *m_s)// execute commands that are unknown to the
{						// shell.
	FILE * cmd_f = _popen(m_s->cmd, "r");	// open the terminal for commands
	char ch;
	int i;

	i = ch = 1;
	if(cmd_f == NULL) {
		m_s->buf = malloc((23+strlen(m_s->cmd)) * sizeof(char));	// create a string of size 23
		sprintf(m_s->buf, "[Err]Command %s : not found!\n", m_s->cmd);	// to tell that <- command not found
	} else {
		while((ch = fgetc(cmd_f)) != EOF)			// get initial size of the buffer
			i++;
		m_s->buf = malloc(i * sizeof(char));			// allocate it
		//if(m_s->buf == NULL)					// process errors
			//return -1;
		i = 0;
		_pclose(cmd_f);						// close and reopen the file stream bc you cant
		cmd_f = _popen(m_s->cmd, "r");	// just rewind it sadly
		while((ch = fgetc(cmd_f)) != EOF) {// write back out the buffer to the given
			m_s->buf[i] = ch;								// char *
			i++;
		}
		m_s->buf[i] = '\0';
	}
	_pclose(cmd_f);							// close terminal
	return 0;
}
