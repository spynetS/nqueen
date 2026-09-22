#ifndef NQUEEN_H
#define NQUEEN_H

#include <stdio.h>


typedef struct {
  int n;
  int pop_len;
  int amnt_parents;
  float mutation_rate;
} Config;

// list of floatsn
typedef struct {
  int* pos;
  size_t size;
  int fitness;
} Chromosome;

int nqueen(Config config);


#endif
