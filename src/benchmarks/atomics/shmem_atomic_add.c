/**
  @file shmem_atomic_add.c
  @brief Source file for shmem_atomic_add latency benchmark with improvements
*/

#include "shmem_atomic_add.h"

/*************************************************************
  @brief Run the latency benchmark for shmem_atomic_add
  @param ntimes Number of iterations for the benchmark
 *************************************************************/
void bench_shmem_atomic_add_latency(int ntimes) {
  /* Check the number of PEs before doing anything */
  if (!check_if_atleast_2_pes()) {
    return;
  }

  /* Get the number of PEs */
  int npes = shmem_n_pes();

  /* Allocate memory for the destination variables on the symmetric heap */
  long *dest = (long *)shmem_malloc(npes * sizeof(long));
  if (dest == NULL) {
    if (shmem_my_pe() == 0) {
      fprintf(stderr, "PE %d: shmem_malloc failed\n", shmem_my_pe());
    }
    shmem_global_exit(1);
  }

  /* Allocate memory for timing variables on the symmetric heap */
  double *local_total_time = (double *)shmem_malloc(sizeof(double));
  double *total_time = (double *)shmem_malloc(sizeof(double));
  if (local_total_time == NULL || total_time == NULL) {
    if (shmem_my_pe() == 0) {
      fprintf(stderr, "PE %d: shmem_malloc failed\n", shmem_my_pe());
    }
    shmem_global_exit(1);
  }

  /* Initialize destination and timing variables */
  for (int j = 0; j < npes; j++) {
    dest[j] = 0;
  }
  *local_total_time = 0.0;
  *total_time = 0.0;

  /* Sync PEs */
  shmem_barrier_all();

  /* Perform the shmem_atomic_add operation ntimes and measure latency */
  for (int i = 0; i < ntimes; i++) {
    int pe = rand() % npes; /* Randomly select a target PE */
    double start_time = mysecond();
    shmem_atomic_add(&dest[shmem_my_pe()], 1, pe); /* Atomic add of 1 */
    double end_time = mysecond();
    *local_total_time += (end_time - start_time) * 1e6; /* Convert to microseconds */
  }

  /* Aggregate and display results */
  shmem_double_sum_reduce(SHMEM_TEAM_WORLD, total_time, local_total_time, 1);

  if (shmem_my_pe() == 0) {
    display_atomic_latency_results("shmem_atomic_add", *total_time / npes, ntimes);
  }

  shmem_barrier_all();

  /* Free memory */
  shmem_free(dest);
  shmem_free(local_total_time);
  shmem_free(total_time);
}