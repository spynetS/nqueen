CC = gcc -Wall -pedantic -O3

nqueen: nqueen.c testing.c check-gnuplot
	$(CC) nqueen.c testing.c -o nqueen

check-gnuplot:
	@command -v gnuplot >/dev/null || \
		(echo "Error: gnuplot is not installed")

pyplot:
	python ./plot.py data.txt

gnuplot:
	gnuplot -e 'set terminal pngcairo size 1200,800; set output "plot.png"; plot "data.txt" using 1:2 with linespoints title "Mean", "data.txt" using 1:3 with linespoints title "Median"'

pytest:
	python testing.py

clear:
	rm data.txt

run: nqueen
	./nqueen 2 10 5

