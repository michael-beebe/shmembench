/**
  @file shmem_alltoalls.c
  @brief Implementation of the shmem_alltoalls benchmarks
  @author Michael Beebe (Texas Tech University)
*/

#include "shmem_alltoalls.h"

/**
  @brief Run the bandwidth benchmark for shmem_alltoalls
  @param min_msg_size Minimum message size for test in bytes
  @param max_msg_size Maximum message size for test in bytes
  @param ntimes Number of times the benchmark should run
 */
void bench_shmem_alltoalls_bw(int min_msg_size, int max_msg_size, int ntimes) {
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

#if defined(USE_14)
  /* Setup pSync array */
  long *pSync = (long *)shmem_malloc(SHMEM_ALLTOALLS_SYNC_SIZE * sizeof(long));
  for (int i = 0; i < SHMEM_ALLTOALLS_SYNC_SIZE; i++) {
    pSync[i] = SHMEM_SYNC_VALUE;
  }
  shmem_barrier_all();
#endif

  /* Run the benchmark */
  for (int i = 0, size = min_msg_size; size <= max_msg_size; size *= 2, i++) {
    /* Validate the message size for the long datatype */
    int valid_size = validate_typed_size(size, sizeof(long), "long");
    msg_sizes[i] = valid_size;

    /* Calculate the number of elements based on the validated size */
    int elem_count = calculate_elem_count(valid_size, sizeof(long));

    /* Allocate memory for source and destination arrays */
    long *source = (long *)shmem_malloc(elem_count * npes * sizeof(long));
    long *dest = (long *)shmem_malloc(elem_count * npes * sizeof(long));

    /* Initialize the source buffer with data */
    for (int j = 0; j < elem_count * npes; j++) {
      source[j] = mype + j;
    }

    /* Initialize start and end time */
    double start_time, end_time;

    /* Sync PEs */
    shmem_barrier_all();

    /* Start timer */
    start_time = mysecond();

    /* Perform NTIMES shmem_alltoalls operations */
    for (int j = 0; j < ntimes; j++) {
#if defined(USE_14)
      shmem_alltoalls64(dest, source, 1, elem_count, elem_count, 0, 0, npes, pSync);
#elif defined(USE_15)
      shmem_alltoalls(SHMEM_TEAM_WORLD, dest, source, 1, elem_count, elem_count);
#endif
    }
    shmem_quiet();

    /* Stop timer */
    end_time = mysecond();

    /* Calculate the average time per operation in useconds */
    times[i] = (end_time - start_time) * 1e6 / ntimes;

    /* Calculate bandwidth */
    bandwidths[i] = calculate_bw(valid_size * npes, times[i]);

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

#if defined(USE_14)
  /* Free pSync allocated for OpenSHMEM 1.4 path */
  shmem_free(pSync);
#endif

  /* Free the memory allocated for message sizes, times, and bandwidths */
  free(msg_sizes);
  free(times);
  free(bandwidths);
}
