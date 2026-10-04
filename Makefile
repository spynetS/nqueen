CC = gcc 
CFLAGS = -Wall -pedantic -O3 -lm
CFLAGS += $(ROULETTE)	

nqueen: nqueen.c testing.c 
	$(CC) $(CFLAGS) nqueen.c testing.c -o nqueen

check-gnuplot:
	@command -v gnuplot >/dev/null || \
		(echo "Error: gnuplot is not installed")

pyplot:
	python ./plot.py data.txt

gnuplot:
	gnuplot -e 'set terminal pngcairo size 1200,800; set output "plot.png"; plot "data.txt" using 1:2 with linespoints title "Mean", "data.txt" using 1:3 with linespoints title "Median"'

pytest: nqueen
	python testing.py

clear:
	rm data.txt

run: nqueen
	./nqueen 5 90

