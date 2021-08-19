CC=g++
CFLAGS=-std=c++17 -Wall
OFILES=main.o parser.o code.o assembler.o symbol_table.o

main: $(OFILES)
	$(CC) $(CFLAGS) $(OFILES) -o main

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

assembler.o: src/assembler.cpp src/assembler.h
	$(CC) $(CFLAGS) -c src/assembler.cpp

parser.o: src/parser.cpp src/parser.h
	$(CC) $(CFLAGS) -c src/parser.cpp

code.o: src/code.cpp src/code.h
	$(CC) $(CFLAGS) -c src/code.cpp

symbol_table.o: src/symbol_table.cpp src/symbol_table.h
	$(CC) $(CFLAGS) -c src/symbol_table.cpp

clean:
	rm -rf *.o main
