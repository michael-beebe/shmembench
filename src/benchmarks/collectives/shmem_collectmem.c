/**
  @file shmem_collectmem.c
  @brief Implementation of the shmem_collectmem benchmarks.
  @author Michael Beebe (Texas Tech University)
 */

#include "shmem_collectmem.h"

/**
  @brief Run the bandwidth benchmark for shmem_collectmem
  @param min_msg_size Minimum message size for the test in bytes
  @param max_msg_size Maximum message size for the test in bytes
  @param ntimes Number of times to run the benchmark
 */
void bench_shmem_collectmem_bw(int min_msg_size, int max_msg_size, int ntimes) {
  /* Ensure there are at least 2 PEs available to run the benchmark */
  if (!check_if_atleast_2_pes()) {
    return;
  }

  /* Variables for message sizes, times, and bandwidths */
  int *msg_sizes;
  double *times, *bandwidths;
  int num_sizes = 0;

  /* Setup benchmark */
  setup_bench(min_msg_size, max_msg_size, &num_sizes, &msg_sizes, &times,
              &bandwidths);

  /* Get the number of processing elements (PEs) and PE number */
  int npes = shmem_n_pes();

  /* Run the benchmark */
  for (int i = 0, size = min_msg_size; size <= max_msg_size; size *= 2, i++) {
    /* Save the message size */
    msg_sizes[i] = size;

    /* Allocate memory for source and destination arrays */
    unsigned char *source = (unsigned char *)shmem_malloc(size);
    unsigned char *dest = (unsigned char *)shmem_malloc(npes * size);

    /* Initialize the source buffer with data */
    for (int j = 0; j < size; j++) {
      source[j] = (unsigned char) j;
    }

    double start_time, end_time;

    /* Sync PEs */
    shmem_barrier_all();

    /* Start timer */
    start_time = mysecond();

    /* Perform the shmem_collectmem operation for the specified number of times */
    for (int j = 0; j < ntimes; j++) {
#if defined(USE_15)
      shmem_collectmem(SHMEM_TEAM_WORLD, dest, source, size);
#endif
    }
    shmem_quiet();
    
    /* Stop timer */
    end_time = mysecond();

    /* Calculate average time per operation in useconds */
    times[i] = (end_time - start_time) * 1e6 / ntimes;

    /* Calculate bandwidth*/
    bandwidths[i] = calculate_bw(size, times[i]);

    /* Free the allocated memory for source and destination arrays */
    shmem_free(source);
    shmem_free(dest);
  }

  /* Display results */
  shmem_barrier_all();
  if (shmem_my_pe() == 0) {
    display_results(times, msg_sizes, bandwidths, "bw", num_sizes);
  }
  shmem_barrier_all();

  /* Free the memory allocated for message sizes, times, and bandwidths */
  free(msg_sizes);
  free(times);
  free(bandwidths);
}
