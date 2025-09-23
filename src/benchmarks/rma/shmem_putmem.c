/**
  @file shmem_putmem.c
  @author Michael Beebe (Texas Tech University)
*/

#include "shmem_putmem.h"

/**
  @brief Run the bandwidth benchmark for shmem_putmem
  @param min_msg_size Minimum message size for test in bytes
  @param max_msg_size Maximum message size for test in bytes
  @param ntimes Number of repetitions to get the avgs from
 */
void bench_shmem_putmem_bw(int min_msg_size, int max_msg_size, int ntimes) {
  /* Check the number of PEs before doing anything */
  if (!check_if_exactly_2_pes()) {
    return;
  }

  int mype = shmem_my_pe();

  /* Stuff that will be used throughout the benchmark */
  int *msg_sizes;
  double *times, *bandwidths;
  int num_sizes = 0;

  /* Setup the benchmark */
  setup_bench(min_msg_size, max_msg_size, &num_sizes, &msg_sizes, &times,
              &bandwidths);

  /* Run the benchmark */
  for (int i = 0, size = min_msg_size; size <= max_msg_size; size *= 2, i++) {
    /* Save message size */
    msg_sizes[i] = size;

    /* Source and destination arrays for the shmem_putmem */
    unsigned char *source = (unsigned char *)shmem_malloc(size);
    unsigned char *dest = (unsigned char *)shmem_malloc(size);

    /* Initialize source buffer */
    for (int j = 0; j < size; j++) {
      source[j] = (unsigned char) j;
    }

    /* Initialize start and end time */
    double start_time, end_time;

    /* Sync PEs */
    shmem_barrier_all();

    /* Start timer */
    start_time = mysecond();

    /* Perform ntimes shmem_puts */
    if (mype == 0) {
      for (int j = 0; j < ntimes; j++) {
#if defined(USE_14) || defined(USE_15)
        shmem_putmem(dest, source, size, 1);
        shmem_fence();
#endif
      }
      shmem_quiet();
    }

    /* Stop timer */
    end_time = mysecond();

    /* Calculate average time per operation in useconds */
    times[i] = (end_time - start_time) * 1e6 / ntimes;

    /* Calculate bandwidth using valid size */
    bandwidths[i] = calculate_bw(size, times[i]);

    /* Sync PEs */
    shmem_barrier_all();

    /* Free the buffers */
    shmem_free(source);
    shmem_free(dest);
  }

  /* Display results */
  shmem_barrier_all();
  if (mype == 0) {
    display_results(times, msg_sizes, bandwidths, "bw", num_sizes);
  }
  shmem_barrier_all();

  /* Free memory */
  free(msg_sizes);
  free(times);
  free(bandwidths);
}

/**
  @brief Run the bidirectional bandwidth benchmark for shmem_put
  @param min_msg_size Minimum message size for test in bytes
  @param max_msg_size Maximum message size for test in bytes
  @param ntimes Number of repetitions to get the avgs from
 */
void bench_shmem_putmem_bibw(int min_msg_size, int max_msg_size, int ntimes) {
  /* Check the number of PEs before doing anything */
  if (!check_if_exactly_2_pes()) {
    return;
  }

  int mype = shmem_my_pe();
  int peer = (mype == 0) ? 1 : 0;

  /* Stuff that will be used throughout the benchmark */
  int *msg_sizes;
  double *times, *bandwidths;
  int num_sizes = 0;

  /* Setup the benchmark */
  setup_bench(min_msg_size, max_msg_size, &num_sizes, &msg_sizes, &times,
              &bandwidths);

  /* Run the benchmark */
  for (int i = 0, size = min_msg_size; size <= max_msg_size; size *= 2, i++) {
    /* Save message size */
    msg_sizes[i] = size;

    /* Source and destination arrays for the shmem_put */
    unsigned char *source = (unsigned char *)shmem_malloc(size);
    unsigned char *dest = (unsigned char *)shmem_malloc(size);

    /* Initialize source buffer */
    for (int j = 0; j < size; j++) {
      source[j] = (unsigned char) j;
    }

    /* Initialize start and end time */
    double start_time, end_time;

    /* Sync PEs */
    shmem_barrier_all();

    /* Start timer */
    start_time = mysecond();

    /* Perform ntimes bidirectional shmem_puts */
    for (int j = 0; j < ntimes; j++) {
#if defined(USE_14) || defined(USE_15)
      shmem_putmem(dest, source, size, peer); /* each PE sends to other PE */
      shmem_fence(); /* ensure ordering of puts */
#endif
    }
    shmem_quiet();

    /* Stop timer */
    end_time = mysecond();

    /* Calculate average time per operation in useconds */
    times[i] = (end_time - start_time) * 1e6 / (ntimes);

    /* Calculate bidirectional bandwidth using valid size */
    bandwidths[i] = calculate_bibw(size, times[i]);

    /* Sync PEs */
    shmem_barrier_all();

    /* Free the buffers */
    shmem_free(source);
    shmem_free(dest);
  }

  /* Display results */
  shmem_barrier_all();
  if (mype == 0) {
    display_results(times, msg_sizes, bandwidths, "bibw", num_sizes);
  }
  shmem_barrier_all();

  /* Free memory */
  free(msg_sizes);
  free(times);
  free(bandwidths);
}
