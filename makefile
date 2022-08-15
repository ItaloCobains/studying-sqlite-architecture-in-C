CC=gcc
CFLAGS=-Wall -g -std=gnu90

all: mainapp

mainapp: main.o
	$(CC) $(CFLAGS) main.o -o mainapp

%.o: %.c
	$(CC) $(CFLAGS) -c $^

clear: 
	rm a.out main.o *.o mainapp
