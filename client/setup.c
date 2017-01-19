#include "setup.h"

void setup_vars(struct main_struct *m_s)
{
	FILE *fport = fopen("port.cfg", "r");
	FILE *fip = fopen("ip.cfg", "r");
	char fport_buf[BUFSIZ];
	char fip_buf[BUFSIZ];
	int ip_len;

	ip_len = 0;
	if (fport == NULL){
		m_s->port = DEFAULT_PORT;
	} else {
		first_line_to_buffer(fport, (char *)&fport_buf);
		m_s->port = atoi(fport_buf);
	}
	if(fip == NULL){
		m_s->ip = malloc(DEFAULT_IP_SIZE * sizeof(char));
		strcpy(m_s->ip, DEFAULT_IP);
	} else {
		first_line_to_buffer(fip, (char*)&fip_buf);
		ip_len = strlen(fip_buf);
		m_s->ip = malloc(ip_len * sizeof(char));
		strcpy(m_s->ip, fip_buf);
	}

}

void setup_connection(struct main_struct *m_s)
{
	m_s->s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(m_s->s == INVALID_SOCKET)
		error("socket");
	m_s->server.sin_family = AF_INET;
	m_s->server.sin_port = htons(m_s->port);
	m_s->server.sin_addr.s_addr = inet_addr(m_s->ip);
}

int setup(struct main_struct *m_s)
{
	setup_win(m_s);
	setup_vars(m_s);
	setup_connection(m_s);

	return 0;
}
