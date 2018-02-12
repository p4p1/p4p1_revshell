# Makefile for p4p1.exe
# Made by Leo Smith
# this is freesoftware
#
# How to compiler
# 	- run make all
# 	- run make clean
# p4p1.exe should then be compiled
#
# On windows please use gcc instead

CC=i686-w64-mingw32-gcc
FLAGS=-Wall -Wextra -I./include
LFLAGS=-lwsock32
SERVER_EXEC=cd server; ./main.py 4441
EXEC=p4p1.exe

HEADER=client/header/bin.h								\
	client/header/setup.h								\
	client/header/command.h								\
	client/header/win_api.h								\
	client/header/network.h								\
	client/header/reverse.h								\
	client/main.h

SRC_PATH=$(realpath ./src)
SRC=$(SRC_PATH)/source/command.c								\
		$(SRC_PATH)/source/bin.c								\
		$(SRC_PATH)/source/reverse.c							\
		$(SRC_PATH)/source/setup.c							\
		$(SRC_PATH)/source/win_api.c							\
		$(SRC_PATH)/source/network.c							\
		$(SRC_PATH)/main.c

OBJ=$(SRC:.c=.o)

all: $(OBJ)
	$(CC) $(OBJ) $(FLAGS) -o $(EXEC) $(LFLAGS)

run:
	$(SERVER_EXEC)

.c.o:	$(HEADER) $(SRC)
	$(CC) $(FLAGS) -o $@ -c $<

clean:	$(OBJ)
	rm -rf $(OBJ)

fclean:	clean
	rm -rf $(EXEC)

re:	fclean all

.PHONY: clean re fclean
