#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>

#include "./nqueen.h"

#define MAX_GENERATIONS 20000


Chromosome get_random_state(size_t n) {
  Chromosome state;
  state.pos = malloc(sizeof(int) * n);
  state.size = n;
  
  for (int i = 0; i < n; i++) {
    state.pos[i] = i;
  }

  for (int i = n - 1; i > 0; i--) {
    int j = rand() % (i + 1);

    int tmp = state.pos[i];
    state.pos[i] = state.pos[j];
    state.pos[j] = tmp;
  }
  return state;
}

void populate(int n, Chromosome *population, size_t size) {
  for (int i = 0; i < size ; i ++ ) {
    population[i] = get_random_state(n);
  }
}

int fitness(Chromosome state) {
  int diag_sum = 0;
  for (int i = 0; i < state.size; i ++) {
    for (int j = i+1; j < state.size; j ++) {
      if (abs(state.pos[j] -state.pos[i]) == abs(j - i) ) {
        diag_sum += 1;
      }
    }
  }
  return diag_sum;
}

int compare_int(const void *a, const void *b) {
  const Chromosome *ca = a;
  const Chromosome *cb = b;

  int x = ca->fitness;
  int y = cb->fitness;
  
  return (x > y) - (x < y);
}

void print_chromosome(Chromosome state) { 
  for(int i = 0; i < state.size; i ++) {
    printf("%d,", state.pos[i]);
  }
  printf("\nfitness %d\n", fitness(state));
}


void selection(Chromosome* population, size_t pop_len, Chromosome* parents, int amnt_parent) {
  qsort(population, pop_len, sizeof(Chromosome), compare_int);

  for (int i = 0; i < amnt_parent; i ++ ) {
    parents[i].pos = malloc(sizeof(int) * population[i].size);
    for (int j = 0; j < population[i].size; j ++ ) {
      parents[i].pos[j] = population[i].pos[j];
    }
    parents[i].size = population[i].size;
  }
  
}

// 2 parents
Chromosome crossover(Chromosome parent1, Chromosome parent2, float mutation_rate) {

  Chromosome child = {0};
  size_t n = parent1.size;
  child.pos = malloc(sizeof(int) * n);
  child.size = n;
  // copy the first half

  // too keep track of used values
  bool used[n];
  for (int i = 0; i < n; i ++){
    used[i] = false;
  }

  // copy the first half from parent 1x
  int pos = 0;
  for (int i = 0; i < n/2; i ++) {
    child.pos[i] = parent1.pos[i];
    used[parent1.pos[i]] = true;
    pos += 1;
  }

  // fill in the rest from parent 2
  for (int i = 0; i < n; i ++) {
    int value = parent2.pos[i];
    if(used[value] != true) {
      child.pos[pos] = value;
      pos += 1;
    }
  }
  // mutate if needed
  float ran = (float)rand() / (float)RAND_MAX;
  if (ran < mutation_rate) {
    int i = rand() % child.size;
    int j = rand() % child.size;

    while (j == i)
      j = rand() % child.size;

    int tmp = child.pos[i];
    child.pos[i] = child.pos[j];
    child.pos[j] = tmp;
  }
  
  return child;
}

int nqueen(Config config) {
  assert(config.pop_len >= config.amnt_parents);
  assert(config.n >= 4);
  assert(config.amnt_parents >= 2);

  
  Chromosome* population = malloc(sizeof(Chromosome)*config.pop_len);
  populate(config.n, population, config.pop_len);

  // as default it is the max generations
  int sum = MAX_GENERATIONS;

  for (int i = 0; i < MAX_GENERATIONS; i ++) {
    Chromosome* parents = malloc(sizeof(Chromosome) * config.amnt_parents);
    selection(population, config.pop_len, parents, config.amnt_parents);
    Chromosome child = {0};
    for (int j = 0; j < config.pop_len; j ++) {
      // TODO select randomly 2 parents
      child = crossover(parents[0], parents[1], config.mutation_rate);
      // free populations
      free(population[j].pos);
      population[j] = child;
      child.fitness = fitness(child);
      if (child.fitness == 0) {
        sum = i;
        goto DONE;
      }
    }
    for (int j = 0; j < config.amnt_parents; j ++ ) {
      free(parents[j].pos);
    }
    free(parents);
  }

 // free the population and return the sum
 DONE:
  for (int j = 0; j < config.pop_len; j ++ ) {
      free(population[j].pos);
    }
    free(population);
  
  return sum;
}

  
