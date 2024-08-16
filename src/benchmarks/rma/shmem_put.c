/**
  @file shmem_put.c
*/

#include "shmem_put.h"

/*************************************************************
  @brief Run the bandwidth benchmark for shmem_put
  @param min_msg_size Minimum message size for test in bytes
  @param max_msg_size Maximum message size for test in bytes
 *************************************************************/
void bench_shmem_put_bw(int min_msg_size, int max_msg_size) {
  ///////////////////////////////////////////////////////////////
  // TODO: move this chunk into a check_pes() function
  ///////////////////////////////////////////////////////////////
  /* Check the number of PEs before doing anything */
  if (shmem_n_pes() != 2) {
    if (shmem_my_pe() == 0) {
      printf(RED_COLOR "\nERROR: " RESET_COLOR "This test requires"
             " exacty 2 PEs!\n\n");
    }
    return;
  }
  ///////////////////////////////////////////////////////////////
  
  int *msg_sizes;
  double *times, *bandwidths;
  int num_sizes = 0;

  ///////////////////////////////////////////////////////////////
  // TODO: move this chunk into a setup_bench() function
  ///////////////////////////////////////////////////////////////
  /* Determine the number of message sizes */
  for (int size = min_msg_size; size <= max_msg_size; size *=2) {
    num_sizes++;
  }

  /* Allocate memory for arrays */
  msg_sizes = (int *)malloc(num_sizes * sizeof(int));
  times = (double *)malloc(num_sizes * sizeof(double));
  bandwidths = (double *)malloc(num_sizes * sizeof(double));
  ///////////////////////////////////////////////////////////////


  ///////////////////////////////////////////////////////////////
  // TODO: move this chunk into a bench_bw() function
  ///////////////////////////////////////////////////////////////
  /* Run the benchmark */
  for (int i = 0, size = min_msg_size; size <= max_msg_size; size *= 2, i++) {
    msg_sizes[i] = size;
    
    /* Source and destination arrays for the shmem_put */
    long *source = (long *)shmem_malloc(size * sizeof(long));
    long *dest = (long *)shmem_malloc(size * sizeof(long));

    /* Initialize source buffer */
    for (int j = 0; j < size; j++) {
      source[j] = j;
    }

    /* Initialize start and end time */
    double start_time, end_time;

    /* Sync PEs */
    shmem_barrier_all();

    /* Start timer */
    start_time = mysecond();

    /* Perform NTIMES shmem_puts */
    for (int j = 0; j < NTIMES; j++) {
      shmem_put(dest, source, size, 1);
      shmem_quiet();
    }

    /* Stop timer */
    end_time = mysecond();

    /* Calculate average time per operation in useconds */
    times[i] = (end_time - start_time) * 1e6 / NTIMES;

    /* Calculate bandwidth */
    bandwidths[i] = calculate_bw(size * sizeof(long), times[i]);

    /* Free the buffers */
    shmem_free(source);
    shmem_free(dest);
  }
  ///////////////////////////////////////////////////////////////

  /* Display results */
  shmem_barrier_all();
  if (shmem_my_pe() == 0) {
    display_results(times, msg_sizes, bandwidths, "bw", num_sizes);
  }
  shmem_barrier_all();

  /* Free memory */
  free(msg_sizes);
  free(times);
  free(bandwidths);
}

/*************************************************************
  @brief Run the bidirectional bandwidth benchmark for shmem_put
  @param min_msg_size Minimum message size for test in bytes
  @param max_msg_size Maximum message size for test in bytes
 *************************************************************/
void bench_shmem_put_bibw(int min_msg_size, int max_msg_size) {
  // TODO:
}

/*************************************************************
  @brief Run the latency benchmark for shmem_put
  @param min_msg_size Minimum message size for test in bytes
  @param max_msg_size Maximum message size for test in bytes
 *************************************************************/
void bench_shmem_put_latency(int min_msg_size, int max_msg_size) {
 // TODO:
}
