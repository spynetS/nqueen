//  === Authors ===
//  Alfred Roos, Stefan Strand, Oliver Fiala, Leo Modin
#ifndef NQUEEN_H
#define NQUEEN_H

#include <stdio.h>

#define MAX_GENERATIONS 20000

typedef struct {
  int n;
  int pop_len;
  float mutation_rate;
  unsigned int* random_seed;
	int selection; // 0: tournament k = 2, 1: tournament k = p / 2, 2: roulettewheel 
} Config;

// list of floatsn
typedef struct {
  int* pos;
  size_t size;
  int fitness;
} Chromosome;

int nqueen(Config config);


#endif
