/*
  === Authors ===
  Alfred Roos, Stefan Strand, Oliver Fiala, Leo Modin

  If ROULETTE is defined the roulette wheel selection method will be used instead of the tournament selection

*/

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <time.h>
#include <pthread.h>

#include "./nqueen.h"


/* #ifndef */
/* //#define ROULETTE */
/* #endif */
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

  return (state.size * (state.size - 1) / 2)  - diag_sum;
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

#ifdef ROULETTE
size_t selection(Chromosome* population, size_t pop_len, unsigned int *seed) {
  // roulette wheel selection
  int total_fitness = 0;

  for (size_t i = 0; i < pop_len; i++) {
    total_fitness += population[i].fitness;
  }
  assert(total_fitness > 0);
  int r = rand_r(seed) % total_fitness;
  int cumulative = 0;
      
  for (size_t i = 0; i < pop_len; i++) {
    cumulative += population[i].fitness;

    if (r < cumulative) {
      return i;
    }
  }

  return pop_len - 1;
}
#else
size_t selection(Chromosome* population, size_t pop_len, unsigned int * seed) {
  // Tournament selection
  assert(pop_len >= 2);
  size_t k = 2;
  size_t fittest = rand_r(seed) % (pop_len);

  // we use i = 1 becuase the fittest is already choosen
  for(int i = 1; i < k ; i++){
    size_t candidate = rand_r(seed) % pop_len;
    if (population[candidate].fitness > population[fittest].fitness)
      fittest = candidate;
  }

  return fittest;
}
#endif

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
  int split = rand_r(seed) % (n - 1) + 1;
  for (int i = 0; i < split; i ++) {
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
  assert(config.n >= 4);

  // create the initial population
  Chromosome* population = malloc(sizeof(Chromosome)*config.pop_len);
  populate(config.n, population, config.pop_len, config.random_seed);
  for (int i = 0; i < config.pop_len; i ++) {
    population[i].fitness = fitness(population[i]);
  }

  // as default it is the max generations
  int sum = MAX_GENERATIONS;

  for (int i = 0; i < MAX_GENERATIONS; i ++) {
    Chromosome *new_population = malloc(sizeof(Chromosome) * config.pop_len);
    
    Chromosome child = {0};
    for (int j = 0; j < config.pop_len; j ++) {

      // pick randomly the parents from the parents array
      size_t parent1 = selection(population, config.pop_len, config.random_seed);
      size_t parent2 = selection(population, config.pop_len, config.random_seed);

      child = crossover(population[parent1], population[parent2], config.mutation_rate, config.random_seed);
      
      child.fitness = fitness(child);
      new_population[j] = child;

      // check child fitness (if it's complete we stop)
      int max = (config.n * (config.n - 1) / 2);
      if (child.fitness == max) {
        sum = i;
        goto DONE;
      }
    }
    for (int j = 0; j < config.pop_len; j ++) {
      free(population[j].pos);
    }
    free(population);
    population = new_population;
  }

 // free the population and return the sum
 DONE:
  for (int j = 0; j < config.pop_len; j ++ ) {
      free(population[j].pos);
    }
    free(population);
  
  return sum;
}

  
