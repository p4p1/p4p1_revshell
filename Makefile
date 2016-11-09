# Make file for p4p1
# This is the best i can do for a makefile
#       _ _       _
#  _ __| | | _ __/ |
# | '_ \_  _| '_ \ |
# | .__/ |_|| .__/_|
# |_|       |_|



CC=gcc
CCWIN=i686-w64-mingw32-gcc
WFLAGS=-lwsock32
LFLAGS=-Wall -pthread -lpthread -lncurses -std=c99
WEXEC=p4p1-o_Vx.exe
LSEXEC=p4p1-s_Vx

all: linux windows

windows: p4p1-h_win.h p4p1-o_win.c
	$(CCWIN) p4p1-o_win.c $(WFLAGS) -o $(WEXEC)

linux:
	$(CC) $(LFLAGS) -c server/*.c
	$(CC) -o $(LSEXEC) $(LFLAGS) *.o $(LFLAGS)

clean:
	rm -rf *.o

mrproper:
	rm -rf $(LSEXEC)
	rm -rf $(WEXEC)
