/**
  @file shmem_atomic_fetch_nbi.c
  @brief Source file for shmem_atomic_fetch_nbi latency benchmark with support for OpenSHMEM 1.5
*/

#include "shmem_atomic_fetch_nbi.h"

/*************************************************************
  @brief Run the latency benchmark for shmem_atomic_fetch_nbi
  @param ntimes Number of iterations for the benchmark
 *************************************************************/
void bench_shmem_atomic_fetch_nbi_latency(int ntimes) {
#if defined(USE_15)
  /* Check the number of PEs before doing anything */
  if (!check_if_atleast_2_pes()) {
    return;
  }

  /* Allocate memory for the destination and source variables */
  int npes = shmem_n_pes();
  long *dest = (long *)shmem_malloc(npes * sizeof(long));
  long *source = (long *)shmem_malloc(sizeof(long));
  if (dest == NULL || source == NULL) {
    if (shmem_my_pe() == 0) {
      fprintf(stderr, "PE %d: shmem_malloc failed\n", shmem_my_pe());
    }
    shmem_global_exit(1);
  }

  /* Allocate memory for timing variables */
  double *local_total_time = (double *)shmem_malloc(sizeof(double));
  double *total_time = (double *)shmem_malloc(sizeof(double));
  if (local_total_time == NULL || total_time == NULL) {
    if (shmem_my_pe() == 0) {
      fprintf(stderr, "PE %d: shmem_malloc failed\n", shmem_my_pe());
    }
    shmem_global_exit(1);
  }

  *local_total_time = 0.0;
  *total_time = 0.0;

  /* Sync PEs */
  shmem_barrier_all();

  /* Perform the shmem_atomic_fetch_nbi operation ntimes and measure latency */
  for (int i = 0; i < ntimes; i++) {
    int pe = rand() % npes; /* Randomly select a target PE */
    double start_time = mysecond();

    /* Perform atomic fetch non-blocking */
    shmem_atomic_fetch_nbi(source, &dest[shmem_my_pe()], pe);

    /* Ensure completion */
    shmem_quiet();

    double end_time = mysecond();
    *local_total_time += (end_time - start_time) * 1e6; /* Convert to microseconds */
  }

  /* Aggregate and display results */
  shmem_double_sum_reduce(SHMEM_TEAM_WORLD, total_time, local_total_time, 1);

  if (shmem_my_pe() == 0) {
    display_atomic_latency_results("shmem_atomic_fetch_nbi", *total_time / npes, ntimes);
  }

  shmem_barrier_all();

  /* Free memory */
  shmem_free(dest);
  shmem_free(source);
  shmem_free(local_total_time);
  shmem_free(total_time);

#else
  if (shmem_my_pe() == 0) {
    fprintf(stderr, "shmem_atomic_fetch_nbi is not supported by this OpenSHMEM version!\n");
  }
#endif
}
