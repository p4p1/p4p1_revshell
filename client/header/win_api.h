#ifndef _WIN_API_H_
#define _WIN_API_H_

/* the windows api wrapper for p4p1
** and some misc functions that i wasnt happy with using
*/

#include "../main.h"

typedef HRESULT (WINAPI *UDTF)(LPVOID, LPCTSTR, LPCTSTR, DWORD, LPVOID);

int setup_win(struct main_struct *);
int download_web(char *, char *);
int my_strcmp(char *, char *);
int my_strcpy(char *, char *, int);

#endif
