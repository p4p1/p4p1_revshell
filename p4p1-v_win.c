#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <windows.h>

int main(int argc ,char *argv[])
{
	HWND stealth;
	AllocConsole();
	stealth=FindWindowA("ConsoleWindowClass", NULL);
	ShowWindow(stealth, 0);
	return 0;
}
