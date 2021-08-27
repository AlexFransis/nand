CC=g++
CFLAGS=-g -std=c++17 -Wall
OFILES=main.o parser.o command.o translator.o

main: $(OFILES)
	$(CC) $(CFLAGS) $(OFILES) -o main

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

parser.o: parser.cpp parser.h
	$(CC) $(CFLAGS) -c parser.cpp

command.o: command.cpp command.h
	$(CC) $(CFLAGS) -c command.cpp

translator.o: translator.cpp translator.h
	$(CC) $(CFLAGS) -c translator.cpp

clean:
	rm -rf *.o main
