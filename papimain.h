#ifndef papimain_h
#define papimain_h

#include "papi.h"

int main_loop(struct server_info * inf);
void connection_handler(int t, struct server_info * inf);
void commandInterpreter(struct server_info * inf, int t);
void bnlisten(struct server_info *);
void ansetupcon(struct server_info *, int sock);
void clearbuf(char * buf, int size);
void printFirstScreen(struct server_info *);
void printPrompt(int s);
void printAcceptedConnection(struct server_info *, char *);
void printConHandler(int t);
int getclinum();

// Special command functions!
void help();
void whoami();

#endif
