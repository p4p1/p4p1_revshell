CC=i686-w64-mingw32-gcc
FLAGS=-lwsock32 -Wall
EXEC=p4p1.exe

all:
	$(CC) client/*.c client/source/*.c $(FLAGS) -o $(EXEC)

clean:
	rm -rf *.o

mrproper:
	rm -rf $(EXEC)
