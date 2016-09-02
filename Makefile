#Make file for p4p1

#       _ _       _
#  _ __| | | _ __/ |
# | '_ \_  _| '_ \ |
# | .__/ |_|| .__/_|
# |_|       |_|



CC=gcc
CCWIN=i686-w64-mingw32-gcc
WFLAGS=-lwsock32
LFLAGS=-Wall -pthread -lpthread -lncurses
WEXEC=p4p1-o_Vx.exe
LSEXEC=p4p1-s_Vx

all: linux windows

windows: p4p1-h_win.h p4p1-o_win.c
	$(CCWIN) p4p1-o_win.c $(WFLAGS) -o $(WEXEC)

linux: p4p1-s_linux.c papimain.c papi.h papimain.h
	$(CC) $(LFLAGS) -c p4p1-s_linux.c papimain.c
	$(CC) -o $(LSEXEC) $(LFLAGS) p4p1-s_linux.o papimain.o $(LFLAGS)

clean:
	rm -rf *.o

mrproper:
	rm -rf $(LSEXEC)
	rm -rf $(WEXEC)
