#Make file for p4p1-o_XX.exe

CC32=i586-mingw32msvc-gcc
CC64=i686-w64-mingw32-gcc
LDFLAGS=-lwsock32

all: p4p1-head.h p4p1-o_win.c
	$(CC32) p4p1-o_win.c $(LDFLAGS) -o p4p1-o_32_Vx.exe
	$(CC64) p4p1-o_win.c $(LDFLAGS) -o p4p1-o_64_Vx.exe
	
clean:
	rm -rf *.exe
