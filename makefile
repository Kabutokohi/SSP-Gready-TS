CC = g++
CFLAGS = -Wall -Wextra -g

all: main

main: main.o File_RW.o
	$(CC) $(CFLAGS) main.o File_RW.o -o main

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp -o main.o

File_RW.o: File_RW.cpp
	$(CC) $(CFLAGS) -c File_RW.cpp -o File_RW.o


clean:
	rm -f *.o main