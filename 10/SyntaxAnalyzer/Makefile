CC=g++
CFLAGS=-g -std=c++17 -Wall
OFILES=main.o tokenizer.o analyzer.o compiler.o file_reader.o file_writer.o

all: main

main: $(OFILES)
	$(CC) $(CFLAGS) $(OFILES) -o main

clean:
	rm -rf *.o main

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

analyzer.o: analyzer.cpp analyzer.h
	$(CC) $(CFLAGS) -c analyzer.cpp

compiler.o: compiler.cpp compiler.h
	$(CC) $(CFLAGS) -c compiler.cpp

tokenizer.o: tokenizer.cpp tokenizer.h
	$(CC) $(CFLAGS) -c tokenizer.cpp

file_reader.o: file_reader.cpp file_reader.h
	$(CC) $(CFLAGS) -c file_reader.cpp

file_writer.o: file_writer.cpp file_writer.h
	$(CC) $(CFLAGS) -c file_writer.cpp
