CC=i686-w64-mingw32-gcc
FLAGS=-lwsock32 -Wall
SERVER_EXEC=cd server; ./main.py 4441
EXEC=p4p1.exe

HEADER=client/header/bin.h\
	client/header/setup.h\
	client/header/command.h\
	client/header/win_api.h\
	client/header/network.h\
	client/header/reverse.h\
	client/main.h

EXECUTION_SOURCE=client/source/command.c\
	client/source/bin.c\
	client/source/reverse.c

SETUP_SOURCE=client/source/setup.c\
	client/source/win_api.c

NETWORKING_SOURCE=client/source/network.c

MAIN_SOURCE=client/main.c

SOURCE=$(MAIN_SOURCE)\
	$(NETWORKING_SOURCE)\
	$(SETUP_SOURCE)\
	$(EXECUTION_SOURCE)

OBJECTS=$(SOURCE:.c=.o)

all: buildp4p1

run:
	$(SERVER_EXEC)

buildp4p1: $(OBJECTS)
	$(CC) $(OBJECTS) $(FLAGS) -o $(EXEC)

.c.o:	$(HEADER) $(SOURCE)
	$(CC) $(FLAGS) -o $@ -c $<

.PHONY: clean

clean:
	rm -rf $(OBJECTS) 

mrproper:
	rm -rf $(EXEC)
