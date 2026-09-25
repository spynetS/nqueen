#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <time.h>
#include <pthread.h>

#include "./nqueen.h"


Chromosome get_rand_r_state(size_t n, unsigned int* seed) {
  Chromosome state;
  state.pos = malloc(sizeof(int) * n);
  state.size = n;
  
  for (int i = 0; i < n; i++) {
    state.pos[i] = i;
  }

  for (int i = n - 1; i > 0; i--) {
    int j = rand_r(seed) % (i + 1);

    int tmp = state.pos[i];
    state.pos[i] = state.pos[j];
    state.pos[j] = tmp;
  }
  return state;
}

void populate(int n, Chromosome *population, size_t size, unsigned int* seed) {
  for (int i = 0; i < size ; i ++ ) {
    population[i] = get_rand_r_state(n, seed);
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
  return state.size-diag_sum;
}

int compare_fit(const void *a, const void *b) {
  const Chromosome *ca = a;
  const Chromosome *cb = b;
  
  return cb->fitness - ca->fitness;
}

void print_chromosome(Chromosome state) { 
  for(int i = 0; i < state.size; i ++) {
    printf("%d,", state.pos[i]);
  }
  printf("\nfitness %d\n", fitness(state));
}


void selection(Chromosome* population, size_t pop_len, Chromosome* parents, int amnt_parent) {
  qsort(population, pop_len, sizeof(Chromosome), compare_fit);

  for (int i = 0; i < amnt_parent; i ++ ) {
    parents[i].pos = malloc(sizeof(int) * population[i].size);
    for (int j = 0; j < population[i].size; j ++ ) {
      parents[i].pos[j] = population[i].pos[j];
    }
    parents[i].size = population[i].size;
  }
  
 }

void mutate(Chromosome *child, unsigned int* seed) {
  int i = rand_r(seed) % child->size;
  int j = rand_r(seed) % child->size;

  while (j == i)
    j = rand_r(seed) % child->size;

  int tmp = child->pos[i];
  child->pos[i] = child->pos[j];
  child->pos[j] = tmp;
}

// 2 parents
Chromosome crossover(Chromosome parent1, Chromosome parent2, float mutation_rate, unsigned int* seed) {
  // randommize the parent size not using half allways
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
  float ran = (float)rand_r(seed) / (float)RAND_MAX;
  if (ran < mutation_rate) {
    mutate(&child, seed);
  }
  
  return child;
}



int nqueen(Config config) {
  assert(config.pop_len >= config.amnt_parents);
  assert(config.n >= 4);
  assert(config.amnt_parents >= 2);

  // create the initial population
  Chromosome* population = malloc(sizeof(Chromosome)*config.pop_len);
  populate(config.n, population, config.pop_len, config.random_seed);
  for (int i = 0; i < config.pop_len; i ++) {
    population[i].fitness = fitness(population[i]);
  }

  // as default it is the max generations
  int sum = MAX_GENERATIONS;

  for (int i = 0; i < MAX_GENERATIONS; i ++) {
    Chromosome* parents = malloc(sizeof(Chromosome) * config.amnt_parents);
    selection(population, config.pop_len, parents, config.amnt_parents);
    
    Chromosome child = {0};
    for (int j = 0; j < config.pop_len; j ++) {

      // pick randomly the parents from the parents array
      int index1 = rand_r(config.random_seed) % config.amnt_parents;
      int index2 = rand_r(config.random_seed) % config.amnt_parents;
      while (index2 == index1){
        index2 = rand_r(config.random_seed) % config.amnt_parents;
      }

      child = crossover(parents[0], parents[1], config.mutation_rate, config.random_seed);
      
      // free populations
      free(population[j].pos);
      child.fitness = fitness(child);
      population[j] = child;

      // check child fitness (if it's complete we stop)
      if (child.fitness == config.n) {
        sum = i;
        for (int j = 0; j < config.amnt_parents; j ++ ) {
          free(parents[j].pos);
        }
        free(parents);
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

  
