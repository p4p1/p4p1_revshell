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
LFLAGS=-Wall -pthread -lpthread -lncurses
W32CEXEC=p4p1-o_32_Vx.exe
W64CEXEC=p4p1-o_64_Vx.exe
W32VEXEC=p4p1-v_32_Vx.exe
W64VEXEC=p4p1-v_64_Vx.exe
LSEXEC=p4p1-s_Vx

all: linux windows

linux: serverlinux

windows: clientwindows vpnwindows

clientwindows: p4p1-h_win.h p4p1-o_win.c
	$(CC32) p4p1-o_win.c $(WFLAGS) -o $(W32CEXEC)
	$(CC64) p4p1-o_win.c $(WFLAGS) -o $(W64CEXEC)

vpnwindows: p4p1-v_win.c p4p1-h_win.h
	$(CC32) p4p1-v_win.c $(WFLAGS) -o $(W32VEXEC)
	$(CC64) p4p1-v_win.c $(WFLAGS) -o $(W64VEXEC)

serverwindows: p4p1-s_win.c
	$(CC32) p4p1-s_win.c $(WFLAGS) -o $(W32SEXEC)
	$(CC64) p4p1-s_win.c $(WFLAGS) -o $(W64SEXEC)

serverlinux: p4p1-s_linux.c papimain.c papi.h papimain.h
	$(CC) $(LFLAGS) -c p4p1-s_linux.c papimain.c
	$(CC) -o $(LSEXEC) $(LFLAGS) p4p1-s_linux.o papimain.o $(LFLAGS)


mrproper:
	rm -rf $(LSEXEC)
	rm -rf $(W32CEXEC)
	rm -rf $(W64CEXEC)
	rm -rf $(W32VEXEC)
	rm -rf $(W64VEXEC)
