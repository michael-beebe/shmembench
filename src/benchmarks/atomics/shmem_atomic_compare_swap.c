/**
  @file shmem_atomic_compare_swap.c
  @brief Source file for shmem_atomic_compare_swap latency benchmark with
  support for OpenSHMEM 1.4 and 1.5
*/

#include "shmem_atomic_compare_swap.h"

/*************************************************************
  @brief Run the latency benchmark for shmem_atomic_compare_swap
  @param ntimes Number of iterations for the benchmark
 *************************************************************/
void bench_shmem_atomic_compare_swap_latency(int ntimes) {
  /* Check the number of PEs before doing anything */
  if (!check_if_atleast_2_pes()) {
    return;
  }

  /* Allocate memory for the destination variable */
  int npes = shmem_n_pes();
  long *dest = (long *)shmem_malloc(npes * sizeof(long));
  if (dest == NULL) {
    if (shmem_my_pe() == 0) {
      fprintf(stderr, "PE %d: shmem_malloc failed\n", shmem_my_pe());
    }
    shmem_global_exit(1);
  }

  /* Initialize dest to 0 for consistent compare-and-swap testing */
  for (int i = 0; i < npes; i++) {
    dest[i] = 0;
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

#if defined(USE_14)
  /* Setup pSync and pWrk arrays for OpenSHMEM 1.4 */
  long *pSync = (long *)shmem_malloc(SHMEM_REDUCE_SYNC_SIZE * sizeof(long));
  double *pWrk =
      (double *)shmem_malloc(SHMEM_REDUCE_MIN_WRKDATA_SIZE * sizeof(double));
  for (int i = 0; i < SHMEM_REDUCE_SYNC_SIZE; i++) {
    pSync[i] = SHMEM_SYNC_VALUE;
  }
#endif

  /* Sync PEs */
  shmem_barrier_all();

  /* Perform the shmem_atomic_compare_swap operation ntimes and measure latency
   */
  for (int i = 0; i < ntimes; i++) {
    int pe = rand() % npes; /* Randomly select a target PE */
    double start_time = mysecond();

    /* Compare and swap: if dest equals 0, set it to 1 */
#if defined(USE_14) || defined(USE_15)
    shmem_atomic_compare_swap(&dest[shmem_my_pe()], 0, 1, pe);
    shmem_quiet(); // Ensure completion for accurate timing
#endif

    double end_time = mysecond();
    *local_total_time +=
        (end_time - start_time) * 1e6; /* Convert to microseconds */
  }

  /* Aggregate and display results */
#if defined(USE_14)
  shmem_double_sum_to_all(total_time, local_total_time, 1, 0, 0, npes, pWrk,
                          pSync);
#elif defined(USE_15)
  shmem_double_sum_reduce(SHMEM_TEAM_WORLD, total_time, local_total_time, 1);
#endif

  if (shmem_my_pe() == 0) {
    display_atomic_latency_results("shmem_atomic_compare_swap",
                                   *total_time / npes, ntimes);
  }

  shmem_barrier_all();

  /* Free memory */
  shmem_free(dest);
  shmem_free(local_total_time);
  shmem_free(total_time);

#if defined(USE_14)
  shmem_free(pSync);
  shmem_free(pWrk);
#endif
}
