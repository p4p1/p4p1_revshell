#include "../header/network.h"

// connect to the server.
int cnt(struct main_struct *m_s)
{
    m_s->s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(m_s->s == INVALID_SOCKET)
        error("socket");
        #ifdef _DEBUG
                printf("Socket created\nconnecting....\n");
        #endif
    do {
        m_s->cn = connect(m_s->s, (SOCKADDR*)&m_s->server, sizeof(m_s->server));
    } while (m_s->cn == SOCKET_ERROR);
        #ifdef _DEBUG
                printf("connected!\n");
        #endif
    return 0;
}

// close connection
int close_cnt(struct main_struct *m_s)
{
    m_s->cn = SOCKET_ERROR;
    closesocket(m_s->s);
        #ifdef _DEBUG
            printf("Closing connection!\n");
        #endif
    sleep(2);
    return 0;
}

// send a buffer to update the connection flag
int sender(SOCKET s, char * buf, int buf_size, int * connection_flag)
{
    int bs;                                 // bytessent.

    if(buf != NULL){
        bs = 0;
        bs = send(s, buf, buf_size, 0);
        if( bs == SOCKET_ERROR ) {          // bytessent == -1;
            *connection_flag = SOCKET_ERROR;
        }
    } else {
        return -1;
    }
    return 0;
}

// recv a buffer and update the cn flag
int receiver(SOCKET s, char * buf, int buf_size, int * connection_flag)
{
    int br;                                // bytesreceived

    if(buf != NULL){
        memset(buf, 0, buf_size);
    }
    br = SOCKET_ERROR;
    while(br == SOCKET_ERROR) {            // while received nothing
        br = recv(s, buf, buf_size, 0);
        if(br == 0 || br == WSAECONNRESET || br < 0)
            *connection_flag = SOCKET_ERROR;
    }
    return 0;
}
