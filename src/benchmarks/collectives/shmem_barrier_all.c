/**
  @file shmem_barrier_all.c
  @brief Implementation of the shmem_barrier_all benchmark
*/

#include "shmem_barrier_all.h"

/**
  @brief Run the latency benchmark for shmem_barrier_all
  @param opts Benchmark options given by the user 
 */
void bench_shmem_barrier_all_latency(options * opts) {
  double start_time, end_time, total_time;
  double avg_time;

  /* Sync all PEs before starting the timer */
  shmem_barrier_all();

  /* Do warmup runs */
  for (int i = 0; i < opts->warmups; i++) {
    shmem_barrier_all();
  }

  shmem_barrier_all();

  start_time = mysecond();

  for (int i = 0; i < opts->ntimes; i++) {
    shmem_barrier_all();
  }

  end_time = mysecond();

  total_time = (end_time - start_time) * 1e6;
  avg_time = total_time / opts->ntimes;

  shmem_barrier_all();
  if (shmem_my_pe() == 0) {
    printf("==============================================\n");
    printf("===        shmem_barrier_all Latency       ===\n");
    printf("==============================================\n");
    printf("Avg Time per Barrier (us): %.2f\n", avg_time);
    printf("Total Time for %d Barriers (us): %.2f\n", opts->ntimes, total_time);
    printf("==============================================\n\n");
  }

  shmem_barrier_all();
}
