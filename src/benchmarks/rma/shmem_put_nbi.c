/**
  @file shmem_put_nbi.c
  @author Michael Beebe (Texas Tech University)
*/

#include "shmem_put_nbi.h"

/**
  @brief Run the bandwidth benchmark for shmem_put_nbi
  @param min_msg_size Minimum message size for test in bytes
  @param max_msg_size Maximum message size for test in bytes
  @param ntimes Number of repetitions to get the avgs from
 */
void bench_shmem_put_nbi_bw(int min_msg_size, int max_msg_size, int ntimes) {
  /* Check the number of PEs before doing anything */
  if (!check_if_exactly_2_pes()) {
    return;
  }

  /* Stuff that will be used throughout the benchmark */
  int *msg_sizes;
  double *times, *bandwidths;
  int num_sizes = 0;

  /* Setup the benchmark */
  setup_bench(min_msg_size, max_msg_size, &num_sizes, &msg_sizes, &times,
              &bandwidths);

  /* Run the benchmark */
  for (int i = 0, size = min_msg_size; size <= max_msg_size; size *= 2, i++) {
    /* Validate the message size for the long datatype */
    int valid_size = validate_typed_size(size, sizeof(long), "long");
    msg_sizes[i] = valid_size;

    /* Calculate the number of elements based on the validated size */
    int elem_count = calculate_elem_count(valid_size, sizeof(long));

    /* Source and destination arrays for the shmem_put_nbi */
    long *source = (long *)shmem_malloc(elem_count * sizeof(long));
    long *dest = (long *)shmem_malloc(elem_count * sizeof(long));

    /* Initialize source buffer */
    for (int j = 0; j < elem_count; j++) {
      source[j] = j;
    }

    /* Initialize start and end time */
    double start_time, end_time;

    /* Sync PEs */
    shmem_barrier_all();

    /* Start timer */
    start_time = mysecond();

    /* Perform ntimes shmem_put_nbis */
    for (int j = 0; j < ntimes; j++) {
#if defined(USE_14) || defined(USE_15)
      shmem_put_nbi(dest, source, elem_count, 1);
#endif
    }
    shmem_quiet();

    /* Stop timer */
    end_time = mysecond();

    /* Calculate average time per operation in useconds */
    times[i] = (end_time - start_time) * 1e6 / ntimes;

    /* Calculate bandwidth using valid size */
    bandwidths[i] = calculate_bw(valid_size, times[i]);

    /* Free the buffers */
    shmem_free(source);
    shmem_free(dest);
  }

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

/**
  @brief Run the bidirectional bandwidth benchmark for shmem_put_nbi
  @param min_msg_size Minimum message size for test in bytes
  @param max_msg_size Maximum message size for test in bytes
  @param ntimes Number of repetitions to get the avgs from
 */
void bench_shmem_put_nbi_bibw(int min_msg_size, int max_msg_size, int ntimes) {
  /* Check the number of PEs before doing anything */
  if (!check_if_exactly_2_pes()) {
    return;
  }

  /* Stuff that will be used throughout the benchmark */
  int *msg_sizes;
  double *times, *bandwidths;
  int num_sizes = 0;

  /* Setup the benchmark */
  setup_bench(min_msg_size, max_msg_size, &num_sizes, &msg_sizes, &times,
              &bandwidths);

  /* Run the benchmark */
  for (int i = 0, size = min_msg_size; size <= max_msg_size; size *= 2, i++) {
    /* Validate the message size for the long datatype */
    int valid_size = validate_typed_size(size, sizeof(long), "long");
    msg_sizes[i] = valid_size;

    /* Calculate the number of elements based on the validated size */
    int elem_count = calculate_elem_count(valid_size, sizeof(long));

    /* Source and destination arrays for the shmem_put_nbi */
    long *source = (long *)shmem_malloc(elem_count * sizeof(long));
    long *dest = (long *)shmem_malloc(elem_count * sizeof(long));

    /* Initialize source buffer */
    for (int j = 0; j < elem_count; j++) {
      source[j] = j;
    }

    /* Initialize start and end time */
    double start_time, end_time;

    /* Sync PEs */
    shmem_barrier_all();

    /* Start timer */
    start_time = mysecond();

    /* Perform ntimes bidirectional shmem_put_nbis */
    for (int j = 0; j < ntimes; j++) {
#if defined(USE_14) || defined(USE_15)
      shmem_put_nbi(dest, source, elem_count, 1); /* PE 0 sends to PE 1 */
      shmem_put_nbi(source, dest, elem_count, 0); /* PE 1 sends to PE 0 */
      shmem_quiet();
#endif
    }

    /* Stop timer */
    end_time = mysecond();

    /* Calculate average time per operation in useconds */
    times[i] = (end_time - start_time) * 1e6 / (2 * ntimes);

    /* Calculate bidirectional bandwidth using valid size */
    bandwidths[i] = calculate_bibw(valid_size, times[i]);

    /* Free the buffers */
    shmem_free(source);
    shmem_free(dest);
  }

  /* Display results */
  shmem_barrier_all();
  if (shmem_my_pe() == 0) {
    display_results(times, msg_sizes, bandwidths, "bibw", num_sizes);
  }
  shmem_barrier_all();

  /* Free memory */
  free(msg_sizes);
  free(times);
  free(bandwidths);
}
