#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <pthread.h>

#define BUFSIZE 9999
#define ADDCHAR(chr) \
if (result < len - 1) \
{ \
*buf++ = (chr); \
result++; \
}

void *connection_handler(void *);
int int_to_str(int value, char *buf, unsigned int len);

int main(int argc, char *argv[])
{
    
    FILE * fp = fopen("port.cfg", "r");
    int s, new_s, c, *new_sock, portno;
    struct sockaddr_in server, client;
    char buf[BUFSIZE];
    char * sidChar = "5";
    char * hostaddrp;    


    if(fp == NULL){
        portno = 4441;
    } else {
        
        printf("Getting port..\n");
        char z;
        int i = 0;
        char fbuf[50];
        while( ( z = fgetc(fp) ) != EOF){
            fbuf[i] = z;
            i++;
        }
        portno = atoi(fbuf);
        
        printf("[*] Custom port %d\n", portno);
    }
    
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons( portno );

    printf("[*] Getting session id up\n");
	printf("[*] Creating socket\n");
	printf("[*] Listening on %s:%d\n", inet_ntoa(server.sin_addr), portno);
    //Create socket:
    s = socket(AF_INET, SOCK_STREAM, 0);
    if(s == -1){
        printf("Could not create socket.\n");
    }
    
    if( bind(s, (struct sockaddr *)&server, sizeof(server)) < 0 ){
        puts("bind failed.");
        return 1;
    }
    
    //Listen
    listen(s, 3);
    
    //Accept and incoming connection
    printf("Waiting for incoming connections on LOCALHOST:%d", portno);
    c = sizeof(struct sockaddr_in);
    while( (new_s = accept(s, (struct sockaddr *)&client, (socklen_t*)&c)) ){
        
        hostaddrp = inet_ntoa(client.sin_addr);
        
        if(hostaddrp == NULL){
            perror("Error in inet_ntoa()");
        }
        
        printf("\n[!] Connection accepted from: %s:%d\n", hostaddrp, portno);
        //sessionID++;
        
        printf("[*] Created Session ID\n");
        
        //Send Session ID
        write(new_s, sidChar, strlen(sidChar));
        printf("[*] Sent the Session ID of: %s\n", sidChar);
        
        pthread_t sniffer_thread;
        new_sock = malloc(1);
        *new_sock = new_s;
        
        if( pthread_create( &sniffer_thread, NULL, connection_handler, (void*) new_sock ) < 0 ){
            perror("could not create thread.");
            return 1;
        }
        
        printf("[!] Handler assigned to %s:%d\n\tOn socket: %d\n", hostaddrp,portno, new_s);
    }
    
    if(new_s < 0){
        perror("accept failed");
        return 1;
    }
    
    return 0;
}

/*
 *This will handle the connections for each client.
 **/
void *connection_handler(void *socket_desc)
{
    printf("-------------------------\n");
    
    printf("       _ _       _\n");
    printf("  _ __| | | _ __/ |\n");
    printf(" | '_ \\_  _| '_ \\ |\n");
    printf(" | .__/ |_|| .__/_|\n");
    printf(" |_|       |_|\n");
    
    //Get the socket_descriptor
    int sock = *(int *)socket_desc;
    char buf[BUFSIZE];
   
	strcpy(buf, "");
 
    printf("[&] In socket: %d\n", sock);
    
    while(1){
        //Ask for input to user.
        printf("\n<p4p1 -%d-/> ", sock);
        fgets(buf, BUFSIZE, stdin);
        
        if( buf[0] == '!' ){ // To change which socket is  connected
            
            char temp_sockChar[5];
            int temp_sock;
            
            //Get socket to connect
            printf("[x] Enter socket to send data to: ");
            fgets(temp_sockChar, 5, stdin);
            temp_sock = atoi(temp_sockChar);
            
            strcpy(buf, "_");
            
            write(temp_sock, buf, BUFSIZE);			//Send data to get some test data
            bzero(buf, BUFSIZE);
            
            read(temp_sock, buf, BUFSIZE);
            printf("[x]Test data:\n%s", buf);		//Get test data to see if socket is existant
            
            if(strlen(buf) > 0){
                sock = temp_sock;
            } else {
                printf("[x] Socket non Existant");
            }
            
        } else if( buf[0] == '_'){	// get number of clients connected
            
            //printf("Number of clients: %d", sessionID);
            
        } else if( buf[0] == '&' ){
            
            strcpy(buf, "&q");
            write(sock, buf, BUFSIZE);
            break;
            
        } else {
            
            write(sock, buf, BUFSIZE);
            bzero(buf, BUFSIZE);
            read(sock, buf, BUFSIZE);
            
            printf("%s", buf);
        }
    }
    //Free the socket_desc
    free(socket_desc);
    return 0;
}

/* int to char */
int int_to_str(int value, char *buf, unsigned int len)
{
    unsigned result = 0;
    
    if (!buf)
        return -1;
    
    int j = 0;
    char int_to_str[16];
    
    if (value < 0)
        ADDCHAR('-');
    
    char *ptr = int_to_str + sizeof(int_to_str) - 1;
    do
    {
        int modulo = value % 10;
        modulo = (modulo<0)?-modulo:modulo;
        *ptr-- =  modulo + '0';
        value /= 10;
        j++;
    } while (value);
    
    for (; j > 0; j--)
        ADDCHAR(int_to_str[sizeof(int_to_str) - j]);
    
    *buf = '\0';
    return 0;
}
