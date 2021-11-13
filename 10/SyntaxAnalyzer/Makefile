CC=g++
CFLAGS=-g -std=c++17 -Wall
OFILES=main.o tokenizer.o parser.o

main: $(OFILES)
	$(CC) $(CFLAGS) $(OFILES) -o main

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

parser.o: parser.cpp parser.h
	$(CC) $(CFLAGS) -c parser.cpp

tokenizer.o: tokenizer.cpp tokenizer.h
	$(CC) $(CFLAGS) -c tokenizer.cpp

clean:
	rm -rf *.o main
