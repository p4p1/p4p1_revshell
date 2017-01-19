CC=i686-w64-mingw32-gcc
FLAGS=-lwsock32
EXEC=p4p1.exe

all:
	$(CC) client/*.c $(FLAGS) -o $(EXEC)

clean:
	rm -rf *.o

mrproper:
	rm -rf $(EXEC)
