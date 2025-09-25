/**
  @file shmem_alltoall.c
  @brief Implementation of the shmem_alltoall benchmark
  @author Michael Beebe (Texas Tech University)
*/

#include "shmem_alltoall.h"

/**
  @brief Run the bandwidth benchmark for shmem_alltoall
  @param min_msg_size Minimum message size for test in bytes
  @param max_msg_size Maximum message size for test in bytes
  @param ntimes Number of iterations for the benchmark
 */
void bench_shmem_alltoall_bw(int min_msg_size, int max_msg_size, int ntimes) {
  /* Check the number of PEs before doing anything */
  if (!check_if_atleast_2_pes()) {
    return;
  }

  /* Stuff that will be used throughout the benchmark */
  int *msg_sizes;
  double *times, *bandwidths;
  int num_sizes = 0;

  /* Setup the benchmark */
  setup_bench(min_msg_size, max_msg_size, &num_sizes, &msg_sizes, &times,
              &bandwidths);

  /* Get the number of PEs and PE number */
  int npes = shmem_n_pes();
  int mype = shmem_my_pe();

#if defined(USE_14)
  /* Setup pSync array */
  long *pSync = (long *)shmem_malloc(SHMEM_ALLTOALL_SYNC_SIZE * sizeof(long));
  for (int i = 0; i < SHMEM_ALLTOALL_SYNC_SIZE; i++) {
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

    /* Source and destination arrays for shmem_alltoall */
    long *source = (long *)shmem_malloc(elem_count * npes * sizeof(long));
    long *dest = (long *)shmem_malloc(elem_count * npes * sizeof(long));

    /* Initialize source buffer */
    for (int j = 0; j < elem_count * npes; j++) {
      source[j] = mype + j;
    }

    /* Initialize start and end time */
    double start_time, end_time;

    /* Sync PEs */
    shmem_barrier_all();

    /* Start timer */
    start_time = mysecond();

    /* Perform NTIMES shmem_alltoall operations */
    for (int j = 0; j < ntimes; j++) {
#if defined(USE_14)
      shmem_alltoall64(dest, source, elem_count, 0, 0, npes, pSync);
#elif defined(USE_15)
      shmem_alltoall(SHMEM_TEAM_WORLD, dest, source, elem_count);
#endif
    }
    shmem_quiet();

    /* Stop timer */
    end_time = mysecond();

    /* Calculate average time per operation in useconds */
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

  /* Free memory */
  free(msg_sizes);
  free(times);
  free(bandwidths);
}
