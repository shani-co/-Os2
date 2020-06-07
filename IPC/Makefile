CC = gcc 
FLAGS = -Wall -g

all: run

run: main
	./main

main: main.o
	$(CC) $(FLAGS) -o main main.o 

main.o: main.c
	$(CC) $(FLAGS) -c main.c 




.PHONY: clean all

clean:
	rm -f *.o main