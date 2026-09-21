CC = gcc -Wall -pedantic


nqueen: nqueen.c testing.c check-gnuplot
	$(CC) nqueen.c testing.c -o nqueen

check-gnuplot:
	@command -v gnuplot >/dev/null || \
		(echo "Error: gnuplot is not installed")

run: nqueen
	./nqueen

