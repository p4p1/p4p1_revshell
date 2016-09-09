#include "papiarg.h"

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

		} else if(!strcmp(argv[1], "-h")){

			usage(argv[0]);

		} else {

			usage(argv[0]);

		}

	} else {

		initscr();
		keypad(stdscr, TRUE);
		main_loop(inf);
		refresh();
		getch();
		endwin();

	}

}

/*
 * Function copied from http://stackoverflow.com/questions/2180079/how-can-i-copy-a-file-on-unix-using-c#2180788
 * learn how to make your own leo plis.
 */
int cp(const char *to, const char *from)
{
    int fd_to, fd_from;
    char buf[4096];
    ssize_t nread;
    int saved_errno;

    fd_from = open(from, O_RDONLY);
    if (fd_from < 0)
        return -1;

    fd_to = open(to, O_WRONLY | O_CREAT | O_EXCL, 0666);
    if (fd_to < 0)
        goto out_error;

    while (nread = read(fd_from, buf, sizeof buf), nread > 0)
    {
        char *out_ptr = buf;
        ssize_t nwritten;

        do {
            nwritten = write(fd_to, out_ptr, nread);

            if (nwritten >= 0)
            {
                nread -= nwritten;
                out_ptr += nwritten;
            }
            else if (errno != EINTR)
            {
                goto out_error;
            }
        } while (nread > 0);
    }

    if (nread == 0)
    {
        if (close(fd_to) < 0)
        {
            fd_to = -1;
            goto out_error;
        }
        close(fd_from);

        /* Success! */
        return 0;
    }

  out_error:
    saved_errno = errno;

    close(fd_from);
    if (fd_to >= 0)
        close(fd_to);

    errno = saved_errno;
    return -1;
}
