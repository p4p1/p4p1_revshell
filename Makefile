#Make file for p4p1

#       _ _       _
#  _ __| | | _ __/ |
# | '_ \_  _| '_ \ |
# | .__/ |_|| .__/_|
# |_|       |_|



CC=gcc
CC32=i586-mingw32msvc-gcc
CC64=i686-w64-mingw32-gcc
WFLAGS=-lwsock32
LFLAGS=-Wall -pthread -lpthread
W32EXEC=p4p1-o_32_Vx.exe
W64EXEC=p4p1-o_64_Vx.exe
LEXEC=p4p1-s_linux

all: windows linux

windows: p4p1-h_win.h p4p1-o_win.c
	$(CC32) p4p1-o_win.c $(WFLAGS) -o $(W32EXEC)
	$(CC64) p4p1-o_win.c $(WFLAGS) -o $(W64EXEC)

linux: p4p1-s_linux.c
	$(CC) p4p1-s_linux.c $(LFLAGS) -o $(LEXEC)

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(LEXEC)
	rm -rf $(W32EXEC)
	rm -rf $(W64EXEC)
