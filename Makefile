CC = gcc -Wall -pedantic

nqueen: nqueen.c testing.c
	$(CC) nqueen.c testing.c -o nqueen

run: nqueen
	./nqueen
