/**
  @file shmem_broadcast.c
  @brief Implementation of the shmem_broadcast benchmarks.
  @author Michael Beebe (Texas Tech University)
*/

#include "shmem_broadcast.h"

/**
  @brief Run the bandwidth benchmark for shmem_broadcast
  @param min_msg_size Minimum message size for the test in bytes
  @param max_msg_size Maximum message size for the test in bytes
  @param ntimes Number of times to run the benchmark
 */
void bench_shmem_broadcast_bw(int min_msg_size, int max_msg_size, int ntimes) {
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

#if defined(USE_14)
  /* Setup pSync array */
  long *pSync = (long *)shmem_malloc(SHMEM_BCAST_SYNC_SIZE * sizeof(long));
  for (int i = 0; i < SHMEM_BCAST_SYNC_SIZE; i++) {
    pSync[i] = SHMEM_SYNC_VALUE;
  }
  shmem_barrier_all();

  /* Get the number of processing elements (PEs) - needed for USE_14 */
  int npes = shmem_n_pes();
#endif

  /* Run the benchmark */
  for (int i = 0, size = min_msg_size; size <= max_msg_size; size *= 2, i++) {
    /* Validate the message size for the long datatype */
    int valid_size = validate_typed_size(size, sizeof(long), "long");
    msg_sizes[i] = valid_size;

    /* Calculate the number of elements based on the validated size */
    int elem_count = calculate_elem_count(valid_size, sizeof(long));

    /* Allocate memory for source and destination arrays */
    long *source = (long *)shmem_malloc(elem_count * sizeof(long));
    long *dest = (long *)shmem_malloc(elem_count * sizeof(long));

    /* Initialize the source buffer with data */
    for (int j = 0; j < elem_count; j++) {
      source[j] = j;
    }

    double start_time, end_time;

    /* Sync PEs */
    shmem_barrier_all();

    /* Start timer */
    start_time = mysecond();

    /* Perform the shmem_broadcast operation for the specified number of times
     */
    for (int j = 0; j < ntimes; j++) {
#if defined(USE_14)
      shmem_broadcast64(dest, source, elem_count, 0, 0, 0, npes, pSync);
#elif defined(USE_15)
      shmem_broadcast(SHMEM_TEAM_WORLD, dest, source, elem_count, 0);
#endif
    }
    shmem_quiet();

    /* Stop timer */
    end_time = mysecond();

    /* Calculate average time per operation in useconds */
    times[i] = (end_time - start_time) * 1e6 / ntimes;
      
    /* Calculate bandwidth */
    bandwidths[i] = calculate_bw(valid_size, times[i]);

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

#if defined(USE_14)
  /* Free pSync allocated for OpenSHMEM 1.4 path */
  shmem_free(pSync);
#endif

  /* Free the memory allocated for message sizes, times, and bandwidths */
  free(msg_sizes);
  free(times);
  free(bandwidths);
}
