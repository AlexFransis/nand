CC=g++
CFLAGS=-g -std=c++17 -Wall
OFILES=main.o parser.o vm_command.o translator.o command_translator.o

main: $(OFILES)
	$(CC) $(CFLAGS) $(OFILES) -o main

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

parser.o: parser.cpp parser.h
	$(CC) $(CFLAGS) -c parser.cpp

vm_command.o: vm_command.cpp vm_command.h
	$(CC) $(CFLAGS) -c vm_command.cpp

translator.o: translator.cpp translator.h
	$(CC) $(CFLAGS) -c translator.cpp

command_translator.o: command_translator.cpp command_translator.h
	$(CC) $(CFLAGS) -c command_translator.cpp

clean:
	rm -rf *.o main
