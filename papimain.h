#ifndef papimain_h
#define papimain_h

#include "papi.h"

// important functions
int main_loop(struct server_info * inf);
int connection_handler(int t, struct server_info * inf);
int commandInterpreter(struct server_info * inf, int * t);
void randptoScreen(int t);
void bnlisten(struct server_info *);
void ansetupcon(struct server_info *, int sock);
void clearbuf(char * buf, int size);

//print functions
void printFirstScreen(struct server_info *);
void printPrompt(int s);
void printAcceptedConnection(struct server_info *, char *);

// Special command functions!
void help();
void changeClient(int * t);
void download(int sock);

#endif  /*end of papimain.h*/
