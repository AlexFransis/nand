CC=g++
CFLAGS=-g -std=c++17 -Wall
OFILES=main.o parser.o command.o file_handler.o command_translator.o

main: $(OFILES)
	$(CC) $(CFLAGS) $(OFILES) -o main

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

parser.o: parser.cpp parser.h
	$(CC) $(CFLAGS) -c parser.cpp

command.o: command.cpp command.h
	$(CC) $(CFLAGS) -c command.cpp

file_handler.o: file_handler.cpp file_handler.h
	$(CC) $(CFLAGS) -c file_handler.cpp

command_translator.o: command_translator.cpp command_translator.h
	$(CC) $(CFLAGS) -c command_translator.cpp

clean:
	rm -rf *.o main
