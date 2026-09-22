#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <pthread.h>

#include "./nqueen.h"

#define MAX_GENERATIONS 20000

#define THREADS 8

int fitness(Chromosome state);

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
      if (abs(state.pos[j] - state.pos[i]) == abs(j - i) ) {
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

typedef struct {
  Chromosome* population;
  int start;
  int end;
  bool found;
} FitnessArg;


void* fitness_worker(void *arg_) {
  FitnessArg* arg = (FitnessArg*)arg_;

  for (int j = arg->start; j < arg->end; j ++){
	  int fit = fitness(arg->population[j]);
    arg->population[j].fitness = fit;
    if (fit == 0) {
      arg->found = true;
      break;
    }
  }
}


int nqueen(Config config) {
  assert(config.pop_len >= config.amnt_parents);
  assert(config.n >= 4);
  assert(config.amnt_parents >= 2);

  
  Chromosome* population = malloc(sizeof(Chromosome)*config.pop_len);
  populate(config.n, population, config.pop_len);

  // as default it is the max generations
  int sum = MAX_GENERATIONS;

  pthread_t threads[THREADS];
  FitnessArg args[THREADS];

  for (int i = 0; i < MAX_GENERATIONS; i ++) {

    int chunk = config.pop_len / THREADS;
    for (int t = 0; t < THREADS; t ++) {

      args[t].start = t * chunk;

      if (t == THREADS - 1)
        args[t].end = config.pop_len;
      else
        args[t].end = (t + 1) * chunk;

      args[t].found = false;
      args[t].population = population;

      pthread_create(&threads[t], NULL, fitness_worker, &args[t]);
    }

    for (int k = 0; k < THREADS; k ++) {
      pthread_join(threads[k], NULL);
      if (args[k].found == true) {
        sum = i;
        goto DONE;
      }
    }

    Chromosome* parents = malloc(sizeof(Chromosome) * config.amnt_parents);
    selection(population, config.pop_len, parents, config.amnt_parents);
    Chromosome child = {0};
    // So this forloop basicly
    for (int j = 0; j < config.pop_len; j ++) {
      int index1 = rand() % (config.amnt_parents);
      int index2 = rand() % (config.amnt_parents);
      while(index2 == index1){
        index2 = rand() % (config.amnt_parents);
      }

      child = crossover(parents[index1], parents[index2], config.mutation_rate);
      // free populations
      free(population[j].pos);
      population[j] = child;
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

  
