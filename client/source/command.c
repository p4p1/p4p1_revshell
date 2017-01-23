#include "../header/command.h"

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
