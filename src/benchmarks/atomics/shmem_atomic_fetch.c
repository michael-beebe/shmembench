/**
  @file shmem_atomic_fetch.c
  @author Michael Beebe (Texas Tech University)
*/

#include "shmem_atomic_fetch.h"

/*************************************************************
  @brief Run the latency benchmark for shmem_atomic_fetch
  @param ntimes Number of repetitions to get the avgs from
 *************************************************************/
void bench_shmem_atomic_fetch_latency(int ntimes) {
  /* Check the number of PEs before doing anything */
  if (!check_if_atleast_2_pes()) {
    return;
  }

  /* Get the number of PEs */
  int npes = shmem_n_pes();

  /* Allocate memory for source and result arrays */
  BENCHMARK_TYPE_PTR(source) = BENCHMARK_MALLOC(npes);
  BENCHMARK_TYPE_PTR(result) = BENCHMARK_MALLOC(1);

  /* Initialize the source array */
  for (int i = 0; i < npes; i++) {
    BENCHMARK_INIT_ELEMENT(source, i, i);
  }

  /* Initialize total time */
  double total_time = 0.0;

  /* Sync PEs */
  shmem_barrier_all();

  /* Perform the shmem_atomic_fetch operation ntimes and measure latency */
  for (int j = 0; j < ntimes; j++) {
    double start_time = mysecond();

    /* Perform atomic fetch operation */
    *result = BENCHMARK_ATOMIC_FETCH(&source[shmem_my_pe()], shmem_my_pe());

    double end_time = mysecond();
    total_time += (end_time - start_time) * 1e6;
  }

  /* Calculate average latency per operation in microseconds */
  double avg_latency = total_time / ntimes;

  /* Display results */
  shmem_barrier_all();
  if (shmem_my_pe() == 0) {
    display_atomic_latency_results("shmem_atomic_fetch", avg_latency / npes,
                                   ntimes);
  }
  shmem_barrier_all();

  /* Free memory */
  BENCHMARK_FREE(source);
  BENCHMARK_FREE(result);
}
