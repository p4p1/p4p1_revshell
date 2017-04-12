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
        #ifdef _DEBUG
          printf("changedir path: %s\n", path);
        #endif
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
        #ifdef _DEBUG
          printf("wget url: %s\nwget filename: %s\n", link, save);
        #endif
	download_web(link, save);			// download it
	free(link);					// free the memory
	free(save);					// free the memory
	return 0;
}

int get_file(char *fname, int sock)
{
    int fd_file;
    char buf[4096];
    char temp[strlen(fname)];
    ssize_t nread, nwritten;
    int i, ii;

    if(fname[0] == 'g' && fname[1] == 'e' && fname[2] == 't' && fname[3] == '-' && fname[4] == 'f' && fname[5] == 'i' && fname[6] == 'l' && fname[7] == 'e' && fname[8] == ' '){
        for( i = 9, ii = 0; i < strlen(fname); i++, ii++) {
            temp[ii] = fname[i];
        }
    }
    #ifdef _DEBUG
      printf("get-file file :%s\n", temp);
    #endif
    fd_file = _open(temp, _O_RDONLY);
    if(fd_file < 0)
        return -1;
    while( nread = read(fd_file, buf, sizeof buf), nread > 0)
    {
        #ifdef _DEBUG
          printf("get-file data: %s\n", buf);
        #endif
        nwritten = write(sock, buf, nread);
        if(nwritten != nread) {
          return -1;
        }
    }
    _close(fd_file);
    return 0;
}
