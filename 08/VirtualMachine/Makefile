CC=g++
CFLAGS=-g -std=c++17 -Wall
OFILES=main.o parser.o translator.o instr_mapper.o

main: $(OFILES)
	$(CC) $(CFLAGS) $(OFILES) -o main

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

parser.o: parser.cpp parser.h
	$(CC) $(CFLAGS) -c parser.cpp

translator.o: translator.cpp translator.h
	$(CC) $(CFLAGS) -c translator.cpp

instr_mapper.o: instr_mapper.cpp instr_mapper.h
	$(CC) $(CFLAGS) -c instr_mapper.cpp

clean:
	rm -rf *.o main
