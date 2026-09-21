#include "nqueen.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define THREADS 12
#define ROUNDS 100
#define START_N 4
#define END_N 20

typedef struct {
  int rounds;
  size_t start;
  size_t end;
  int *results;
} Arg;

typedef struct {
  int generations;
  int average;
  float success_rate;

} Result;

int get_avarage(Config config, int rounds) {
  int sum = 0;

  for (size_t i = 0; i < rounds; i ++) {
    int generations = nqueen(config);
    sum += generations;
    //printf("generations: %d \n", generations);
  }
  return sum/rounds;

}

void *worker(void *arg_) {
  Arg *arg = (Arg*)arg_;

  for ( int i = arg->start; i < arg->end; i ++){

    Config config = {0};
    config.n = (i);
    config.pop_len = config.n*7;
    config.amnt_parents = (int) (config.n*15)/10;
    config.mutation_rate = 0.05f;
    int a = get_avarage(config, arg->rounds);
    printf("n = %d : %d\n", i, a);
    (arg->results)[i] = a;
  }
  return 0;
}

int main() {
  srand(time(NULL));

  size_t work_start = START_N;
  size_t work_end = END_N;

  size_t work_size = work_end - work_start;

  pthread_t threads[THREADS];
  Arg args[THREADS];
  int results[work_end];
  
  for (int  i = 0; i < THREADS; i ++ ){
    size_t start = work_start + i * work_size / THREADS;
    size_t end   = work_start + (i + 1) * work_size / THREADS;
    
    args[i] = (Arg){
      ROUNDS,
      start,
      end,
      results,
    };
    pthread_create(&threads[i], NULL, worker, &args[i]);

  }
  for (int  i = 0; i < THREADS; i ++ ){
    pthread_join(threads[i], NULL);
  }

  // handling output
  FILE* file = fopen("data.txt", "w");
  if (file == NULL) {
    exit(1);
  }

  for (int i = work_start; i < work_end; i++) {
    fprintf(file, "%d %d\n",i, results[i]);
  }

  fclose(file);
  #ifdef __unix__
  printf("generated plot ./plot.png\n");
  system("gnuplot -e 'set terminal png; set output \"plot.png\"; plot \"data.txt\" using 1:2 with lines'");
  #endif
  return 0;
}
