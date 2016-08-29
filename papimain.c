#include "papimain.h"

int main_loop(struct server_info * inf)
{
	int c, new_s, *new_sock;
	bnlisten(inf);

	c = sizeof(struct sockaddr_in);
	while( (new_s = accept(inf->s, (struct sockaddr *)&inf->client, (socklen_t*)&c)) ){

		inf->hostaddrp = inet_ntoa(inf->client.sin_addr);
		if(inf->hostaddrp == NULL){
			error("inet_ntoa()", 1);
		}

		printw("Connection from %s:%d", inf->hostaddrp, inf->portno);
		refresh();
	}

	return 0;
}

void bnlisten(struct server_info * inf)
{

	if( bind(inf->s, (struct sockaddr *)&inf->server, sizeof(inf->server)) < 0 ){
		error("bind failed", 1);
	}

	listen(inf->s, 3);

	printw("Listening on %s:%d\n", inf->ip, inf->portno);
	refresh();

}
