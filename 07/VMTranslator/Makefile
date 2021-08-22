CC=g++
CFLAGS=-std=c++17 -Wall
OFILES=main.o parser.o

main: $(OFILES)
	$(CC) $(CFLAGS) $(OFILES) -o main

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

parser.o: parser.cpp parser.h
	$(CC) $(CFLAGS) -c parser.cpp

clean:
	rm -rf *.o main
