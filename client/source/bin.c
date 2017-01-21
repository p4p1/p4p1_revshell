#include "../header/bin.h"

int cd(char *buf)				// change directory function
{						// processes the command buffer
	int i, z;				// to change directory
	int len;
	char ch;
	char *path;

	i = len = z = ch = 0;
	if(buf[0] == 'c' && buf[1] == 'd') {	// cd command hasent been removed
		for(len = 3; len < strlen(buf); len++) // start at space and get the length
			;
		path = malloc(len * sizeof(char));	// create a path that is a special size
		for(i = 3; i < strlen(buf); i++)	// for the buff.
			path[z++] = buf[i];		// cp the buffer char to the path
	} else {
		path = malloc(strlen(buf) * sizeof(char));
		if(path == NULL)
			return -1;
	}
	chdir(path);			// change directory part.
	free(path);			// free alocated path after moving.
	return 0;
}

int exe(struct main_struct * m_s)// execute commands that are unknown to the
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
		if(m_s->buf == NULL)					// process errors
			return -1;
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
