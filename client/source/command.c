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
		path[z] = '\0';
	} else {
		path = malloc(strlen(buf) * sizeof(char));
		if(path == NULL)
			return -1;
		my_strcpy(path, buf, strlen(buf));
	}
	chdir(path);			// change directory part.
	free(path);			// free alocated path after moving.
	return 0;
}

int wget(char *buf)	// download from the web command
{
	int i, len, z;
	char *link;		// main url : this is retreived for the buffer
	char *save;		// save file name: truncated from the end of the url

	z = len = i = 0;
	for(i = 5; i < strlen(buf); i++)		// get the length of url without counting the wget at the start
		len++;
	link = malloc(len * sizeof(char));		// allocate memory
	len = 0;
	for(i = strlen(buf); buf[i] != '/'; i--)	// get the length if save file name by going backwords the buffer until u touch a '\'
		len++;
	save = malloc(len * sizeof(char));		// allocate memory
	for(i = 5; i < strlen(buf); i++) {		// start moving each characters around
		link[z] = buf[i];			// create the link
		z++;
	}
	z = 0;
	for(i = strlen(buf); buf[i] != '/'; i--)
		;
	for(z = 0; i < strlen(buf); z++)
		save[z] = buf[++i];			// create the file name
	download_web(link, save);			// download it
	free(link);					// free the memory
	free(save);					// free the memory
	return 0;
}
