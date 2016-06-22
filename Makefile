#Make file for p4p1-o_XX.exe

all: main.o
	gcc -c main.c -o p4p1-o_32.exe
	gcc -c main.c -o p4p1-o_64.exe

clean:
	rm -rf *.o
