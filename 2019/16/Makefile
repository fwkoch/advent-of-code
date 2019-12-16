CFLAGS=-Wall
LIBS=-lcheck

.PHONY: build clean test

build: main

main: main.o fft.o
	gcc -o main main.o fft.o

main.o: main.c fft.h
	gcc $(CFLAGS) -c main.c

fft.o: fft.c fft.h
	gcc $(CFLAGS) -c fft.c

test: check_fft
	./check_fft

check_fft: check_fft.o fft.o
	gcc -o check_fft fft.o check_fft.o $(LIBS)

check_fft.o: check_fft.c fft.h
	gcc $(CFLAGS) -c check_fft.c

check_fft.c:
	checkmk check_fft.check > check_fft.c

clean:
	rm -f main check_fft main.o fft.o check_fft.o check_fft.c
