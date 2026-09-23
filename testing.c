#include "nqueen.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#ifndef THREADS
#define THREADS 16
#endif
#ifndef ROUNDS
#define ROUNDS 50
#endif
#ifndef START_N
#define START_N 4
#endif
#ifndef END_N
#define END_N 13
#endif

int pop_mod = 2;
int par_percent = 10;
int mut_percent = 5;

typedef struct {
  int mean;
  int median;
  float success_rate;

} Result;

typedef struct {
  int rounds;
  size_t start;
  size_t end;
  Result *results;
} Arg;

int compare_int(const void *a, const void *b) {
  int ca = *((int*)a);
  int cb = *((int*)b);
  return cb - ca;
}


Result get_rounds_sum(Config config, int rounds) {
  Result res = {0};
  int times[rounds];
  int times_c = 0;
  for (size_t i = 0; i < rounds; i ++) {
    int generations = nqueen(config);
    if (generations != MAX_GENERATIONS) {
      res.success_rate += 1;
    }
    res.mean += generations;
    times[times_c++] = generations;
  }

  qsort(times, times_c, sizeof(int), compare_int);
  res.median = times[times_c/2];
  res.mean /= rounds;
  res.success_rate /= rounds;

  return res;
}

void *worker(void *arg_) {
  Arg *arg = (Arg*)arg_;

  for ( int i = arg->start; i < arg->end; i ++){

    Config config = {0};
    config.n = (i);
    config.pop_len = config.n * pop_mod;
    config.amnt_parents = (int)(config.pop_len * par_percent)/100;
    if (config.amnt_parents <= 2) config.amnt_parents = 2;
    config.mutation_rate = (float)mut_percent/100;

    unsigned int seed = (unsigned int)time(NULL) ^ (unsigned int)pthread_self();
    config.random_seed = &seed;
    Result res = get_rounds_sum(config, arg->rounds);
    printf("n = %d : %d %d%%\n", i, res.mean, (int)(res.success_rate*100));
    (arg->results)[i] = res;
  }
  return 0;
}

int main(int argc, char** argv) {
  
  if (argc >= 2) {
    pop_mod = atoi(argv[1]);
  }
  else {
    printf("usage -- \n Expects 3 arguments all integers\n nqueen <population moddifer> <parent percentage of population> <mutation percentage>\n default nqueen 2 10 5\n");
    return 1;
  }
  if (argc >= 3) {
    par_percent = atoi(argv[2]);
  }
  if (argc >= 4) {
    mut_percent = atoi(argv[3]);
  }


  srand(time(NULL));

  size_t work_start = START_N;
  size_t work_end = END_N;
  size_t work_size = work_end - work_start;

  pthread_t threads[THREADS];
  Arg args[THREADS];
  Result results[work_end];
  
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
  FILE* file = fopen("data.txt", "a");
  if (file == NULL) {
    exit(1);
  }

  for (int i = work_start; i < work_end; i++) {
    fprintf(file, "%d %d %d\n",i, results[i].mean, results[i].median);
  }

  fclose(file);
  #ifdef __unix__
  printf("generated plot ./plot.png\n");

  //system("python plot.py data.txt");
  //system("gnuplot -e 'set terminal pngcairo size 1200,800; set output \"plot.png\"; plot \"data.txt\" using 1:2 with linespoints title \"Mean\", \"data.txt\" using 1:3 with linespoints title \"Median\"'");

 #endif
  return 0;
}
