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
W32CEXEC=p4p1-o_32_Vx.exe
W64CEXEC=p4p1-o_64_Vx.exe
LSEXEC=p4p1-s_Vx
LCEXEC=p4p1-o_Vx

all: linux windows

linux: serverlinux clientlinux

windows: clientwindows

clientwindows: p4p1-h_win.h p4p1-o_win.c
	$(CC32) p4p1-o_win.c $(WFLAGS) -o $(W32CEXEC)
	$(CC64) p4p1-o_win.c $(WFLAGS) -o $(W64CEXEC)

serverlinux: p4p1-s_linux.c
	$(CC) p4p1-s_linux.c $(LFLAGS) -o $(LSEXEC)

clientlinux: p4p1-o_linux.c p4p1-h_linux.h
	$(CC) p4p1-o_linux.c $(LFLAGS) -o $(LCEXEC)

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(LSEXEC)
	rm -rf $(LCEXEC)
	rm -rf $(WC32CEXEC)
	rm -rf $(WC64CEXEC)
