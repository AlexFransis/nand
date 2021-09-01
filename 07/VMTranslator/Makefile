CC=g++
CFLAGS=-g -std=c++17 -Wall
OFILES=main.o parser.o vm_command.o translator.o asm_commands.o

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

asm_commands.o: asm_commands.cpp asm_commands.h
	$(CC) $(CFLAGS) -c asm_commands.cpp

clean:
	rm -rf *.o main
