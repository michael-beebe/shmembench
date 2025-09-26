/**
  @file shmem_alltoallsmem.c
  @brief Implementation of the shmem_alltoallsmem benchmarks
  @author Michael Beebe (Texas Tech University)
*/

#include "shmem_alltoallsmem.h"

/**
  @brief Run the bandwidth benchmark for shmem_alltoallsmem
  @param min_msg_size Minimum message size for test in bytes
  @param max_msg_size Maximum message size for test in bytes
  @param ntimes Number of times the benchmark should run
 */
void bench_shmem_alltoallsmem_bw(int min_msg_size, int max_msg_size, int ntimes) {
  /* Ensure there are at least 2 PEs available to run the benchmark */
  if (!check_if_atleast_2_pes()) {
    return;
  }

  /* Variables for message sizes, times, and bandwidths */
  int *msg_sizes;
  double *times, *bandwidths;
  int num_sizes = 0;

  /* Setup the benchmark */
  setup_bench(min_msg_size, max_msg_size, &num_sizes, &msg_sizes, &times,
              &bandwidths);

  /* Get the number of processing elements (PEs) and PE number */
  int npes = shmem_n_pes();
  int mype = shmem_my_pe();

  /* Run through the benchmark */
  for (int i = 0, size = min_msg_size; size <= max_msg_size; size *= 2, i++) {
    /* Save message size */
    msg_sizes[i] = size;

    /* Allocate memory for source and destination arrays */
    unsigned char *source = (unsigned char *)shmem_malloc(size * npes);
    unsigned char *dest = (unsigned char *)shmem_malloc(size * npes);

    /* Initialize the source buffer with data */
    for (int j = 0; j < size * npes; j++) {
      source[j] = (unsigned char) (mype + j );
    }

  /* Initialize start and end time */
    double start_time, end_time;

    /* Sync PEs */
    shmem_barrier_all();

    /* Start timer */
    start_time = mysecond();

    /* Perform NTIMES shmem_alltoallsmem operations */
    for (int j = 0; j < ntimes; j++) {
#if defined(USE_15)
      shmem_alltoallsmem(SHMEM_TEAM_WORLD, dest, source, 1, size, size);
#endif
    }
    shmem_quiet();
    
    /* Stop timer */
    end_time = mysecond();

    /* Calculate the average time per operation in useconds */
    times[i] = (end_time - start_time) * 1e6 / ntimes;

    /* Calculate bandwidth */
    bandwidths[i] = calculate_bw(size * npes, times[i]);

    /* Free the allocated memory for source and destination arrays */
    shmem_free(source);
    shmem_free(dest);
  }

  /* Display results */
  shmem_barrier_all();
  if (mype == 0) {
    display_results(times, msg_sizes, bandwidths, "bw", num_sizes);
  }
  shmem_barrier_all();

  /* Free the memory allocated for message sizes, times, and bandwidths */
  free(msg_sizes);
  free(times);
  free(bandwidths);
}
