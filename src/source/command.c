#include "main.h"

int cd(char *buf)
{
	int i, z;
	int len;
	char ch;
	char *path;

	i = len = z = ch = 0;
	if(buf[0] == 'c' && buf[1] == 'd') {
		for(len = 3; len < strlen(buf); len++)
			;
		path = malloc(len * sizeof(char));
		for(i = 3; i < strlen(buf); i++)
			path[z++] = buf[i];
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
	chdir(path);
	free(path);
	return 0;
}

int wget(char *buf)
{
	int i, len, z;
	char *link;
	char *save;

	z = len = i = 0;
	for(i = 5; i < strlen(buf); i++)
		len++;
	link = malloc(len * sizeof(char));
	len = 0;
	for(i = strlen(buf); buf[i] != '/'; i--)
		len++;
	save = malloc(len * sizeof(char));
	for(i = 5; i < strlen(buf); i++) {
		link[z] = buf[i];
		z++;
	}
	z = 0;
	for(i = strlen(buf); buf[i] != '/'; i--)
		;
	for(z = 0; i < strlen(buf); z++)
		save[z] = buf[++i];
	#ifdef _DEBUG
		printf("wget url: %s\nwget filename: %s\n", link, save);
	#endif
	download_web(link, save);
	free(link);
	free(save);
	return 0;
}

int get_file(char *fname, int sock)
{
	int fd_file;
	char buf[4096];
	char temp[strlen(fname)];
	char file_name[strlen(fname)];
	ssize_t nread, nwritten;
	int i, ii;

	if(fname[0] == 'g' && fname[1] == 'e' && fname[2] == 't' && fname[3] == '-'
			&& fname[4] == 'f' && fname[5] == 'i' && fname[6] == 'l' && fname[7] == 'e'
			&& fname[8] == ' ') {
		for( i = 9, ii = 0; i < strlen(fname); i++, ii++) {
			temp[ii] = fname[i];
		}
		if((temp[0] != '.' && temp[1] != '\\') || temp[0] != '\\') {
			file_name[0] = '.';
			file_name[1] = '\\';
			for( i = 2, ii = 0; i < strlen(fname); i++, ii++) {
				file_name[i] = temp[ii];
			}
		} else {
			for( i = 0; i < strlen(fname); i++) {
				file_name[i] = temp[i];
			}
		}
	}
	#ifdef _DEBUG
		printf("get-file file :%s\n", file_name);
	#endif
	fd_file = _open(file_name, _O_RDONLY);
	if(fd_file < 0)
		return -1;
	while( nread = read(fd_file, buf, sizeof buf), nread > 0) {
		#ifdef _DEBUG
			printf("get-file data: %s\n", buf);
		#endif
		nwritten = send(sock, buf, strlen(buf), 0);
		if(nwritten == SOCKET_ERROR) {
			return -1;
		}
	}
	_close(fd_file);
	return 0;
}
