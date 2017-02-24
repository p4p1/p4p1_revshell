#include "file_retreiver.h"

int main(int ac, char *av[])
{

	fd_from = open(from, O_RDONLY);
	if (fd_from < 0)
		return -1;

	fd_to = open(to, O_WRONLY | O_CREAT | O_EXCL, 0666);
	if (fd_to < 0)
		return -1;

	return 0;
}

int cp(int fd_to, int fd_from)
{
    char buf[4096];
    ssize_t nread;
    int saved_errno;

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

        /* Success! */
        return 0;
    }

  out_error:
    saved_errno = errno;

    errno = saved_errno;
    return -1;
}
