# Make file for p4p1
# This is the best i can do for a makefile
#       _ _       _
#  _ __| | | _ __/ |
# | '_ \_  _| '_ \ |
# | .__/ |_|| .__/_|
# |_|       |_|


CC=i686-w64-mingw32-gcc
FLAGS=-lwsock32
EXEC=p4p1-o_Vx.exe

all: p4p1-h_win.h p4p1-o_win.c
	$(CC) p4p1-o_win.c $(FLAGS) -o $(EXEC)

clean:
	rm -rf *.o

mrproper:
	rm -rf $(EXEC)
