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
W32SEXEC=p4p1-s_32_Vx.exe
W64SEXEC=p4p1-s_64_Vx.exe
LSEXEC=p4p1-s_Vx
MCEXEC=p4p1-o_Vx
LVEXEC=p4p1-v_Vx

all: linux windows mac

mac: serverlinux clientmac vpnlinux

linux: serverlinux vpnlinux

windows: clientwindows serverwindows

clientwindows: p4p1-h_win.h p4p1-o_win.c
	$(CC32) p4p1-o_win.c $(WFLAGS) -o $(W32CEXEC)
	$(CC64) p4p1-o_win.c $(WFLAGS) -o $(W64CEXEC)

serverwindows: p4p1-s_win.c
	$(CC32) p4p1-s_win.c $(WFLAGS) -o $(W32SEXEC)
	$(CC64) p4p1-s_win.c $(WFLAGS) -o $(W64SEXEC)

serverlinux: p4p1-s_linux.c
	$(CC) p4p1-s_linux.c $(LFLAGS) -o $(LSEXEC)

clientmac: p4p1-o_mac.c p4p1-h_mac.h
	$(CC) p4p1-o_mac.c $(LFLAGS) -o $(MCEXEC)

vpnlinux:
	$(CC) p4p1-v_linux.c $(LFLAGS) -o $(LVEXEC)

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(LSEXEC)
	rm -rf $(MCEXEC)
	rm -rf $(WC32CEXEC)
	rm -rf $(WC64CEXEC)
