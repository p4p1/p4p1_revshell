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
  }

  fclose(fp);

  while(1){

      //Start up socket and try connecting.

  }

  return 0;
}
