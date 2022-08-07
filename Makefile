CC=g++
CFLAGS=-Wall


%.o: %.cpp *.h
	$(CC) $(CFLAGS) -c -o $@ $<

tak: piece.o game.o main.o
	$(CC) -o tak piece.o game.o main.o

all: tak
	