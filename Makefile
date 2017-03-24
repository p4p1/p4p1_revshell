CC=i686-w64-mingw32-gcc
FLAGS=-lwsock32 -Wall
EXEC=p4p1.exe

HEADER=client/header/bin.h\
	client/header/setup.h\
	client/header/command.h\
	client/header/win_api.h\
	client/header/network.h\
	client/header/reverse.h\
	client/main.h

SOURCE=client/source/bin.c\
	client/source/network.c\
	client/source/setup.c\
	client/source/command.c\
	client/source/reverse.c\
	client/source/win_api.c\
	client/main.c

OBJECTS=bin.o\
	network.o\
	setup.o\
	command.o\
	reverse.o\
	win_api.o\
	main.o

all: buildp4p1
run:
	./$(EXEC)

buildp4p1: $(HEADER) $(SOURCE)
	$(CC) $(FLAGS) -c $(SOURCE)
	$(CC) $(OBJECTS) $(FLAGS) -o $(EXEC)

.PHONY: clean

clean:
	rm -rf *.o

mrproper:
	rm -rf $(EXEC)
