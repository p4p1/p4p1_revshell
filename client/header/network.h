#ifndef _NETWORK_H_
#define _NETWORK_H_

/* all the network configuration functions
** and the function used to connect to the server
**/

#include "../main.h"

int cnt(struct main_struct *m_s);
int close_cnt(struct main_struct *m_s);
int sender(SOCKET s, char * buf, int buf_size, int * connection_flag);
int receiver(SOCKET s, char * buf, int buf_size, int * connection_flag);

#endif
