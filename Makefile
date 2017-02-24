CC=i686-w64-mingw32-gcc
FLAGS=-lwsock32 -Wall
EXEC=p4p1.exe

client:
	$(CC) client/*.c client/source/*.c $(FLAGS) -o $(EXEC)

all:
	make client

clean:
	rm -rf *.o

mrproper:
	rm -rf $(EXEC)
