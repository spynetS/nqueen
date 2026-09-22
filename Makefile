CC = gcc -Wall -pedantic


nqueen: nqueen.c testing.c check-gnuplot
	$(CC) nqueen.c testing.c -o nqueen

check-gnuplot:
	@command -v gnuplot >/dev/null || \
		(echo "Error: gnuplot is not installed")

image:
	gnuplot -e 'set terminal pngcairo size 1200,800; set output "plot.png"; plot "data.txt" using 1:2 with linespoints title "Mean", "data.txt" using 1:3 with linespoints title "Median"'

run: nqueen
	./nqueen

