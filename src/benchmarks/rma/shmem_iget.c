/**
  @file shmem_iget.c
  @brief Source file for shmem_iget benchmarks with enhanced debugging and error handling
*/

#include "shmem_iget.h"

/*************************************************************
  @brief Run the bandwidth benchmark for shmem_iget
  @param min_msg_size Minimum message size for test in bytes
  @param max_msg_size Maximum message size for test in bytes
  @param ntimes Number of repetitions to get the avgs from
  @param stride Stride for shmem_iget
 *************************************************************/
void bench_shmem_iget_bw(int min_msg_size, int max_msg_size, int ntimes, int stride) {
  /* Check the number of PEs before doing anything */
  if (!check_if_exactly_2_pes()) {
    return;
  }

  int *msg_sizes;
  double *times, *bandwidths;
  int num_sizes = 0;

  /* Setup the benchmark */
  setup_bench(min_msg_size, max_msg_size, &num_sizes, &msg_sizes, &times, &bandwidths);

  /* Run the benchmark */
  for (int i = 0, size = min_msg_size; size <= max_msg_size; size *= 2, i++) {
    msg_sizes[i] = size;

    /* Allocate memory for source and destination buffers */
    long *source = (long *)shmem_malloc(size * stride * sizeof(long));
    long *dest = (long *)shmem_malloc(size * stride * sizeof(long));

    if (source == NULL || dest == NULL) {
      fprintf(stderr, "PE %d: Memory allocation failed\n", shmem_my_pe());
      shmem_global_exit(1);
    }

    /* Initialize source buffer */
    for (int j = 0; j < size * stride; j++) {
      source[j] = j;
    }

    /* Sync PEs */
    shmem_barrier_all();

    /* Start timer */
    double start_time = mysecond();

    /* Perform ntimes shmem_igets */
    for (int j = 0; j < ntimes; j++) {
#if defined(USE_14) || defined(USE_15)
      shmem_iget(dest, source, 1, stride, size, 1);
#endif
    }
    shmem_quiet();  // Ensure completion of operations before stopping timer

    /* Stop timer */
    double end_time = mysecond();

    /* Calculate average time per operation in microseconds */
    times[i] = (end_time - start_time) * 1e6 / ntimes;

    /* Calculate bandwidth */
    bandwidths[i] = calculate_bw(size * sizeof(long), times[i]);

    /* Debugging: Print bandwidth information */
    // printf("PE %d: shmem_iget BW test - Msg Size: %d, Avg Time: %.2f us, Bandwidth: %.2f MB/s\n",
    //        shmem_my_pe(), size, times[i], bandwidths[i]);

    /* Free buffers */
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

/*************************************************************
  @brief Run the bidirectional bandwidth benchmark for shmem_iget
  @param min_msg_size Minimum message size for test in bytes
  @param max_msg_size Maximum message size for test in bytes
  @param ntimes Number of repetitions to get the avgs from
  @param stride Stride for shmem_iget
 *************************************************************/
void bench_shmem_iget_bibw(int min_msg_size, int max_msg_size, int ntimes, int stride) {
  /* Check the number of PEs before doing anything */
  if (!check_if_exactly_2_pes()) {
    return;
  }

  int *msg_sizes;
  double *times, *bandwidths;
  int num_sizes = 0;

  /* Setup the benchmark */
  setup_bench(min_msg_size, max_msg_size, &num_sizes, &msg_sizes, &times, &bandwidths);

  /* Run the benchmark */
  for (int i = 0, size = min_msg_size; size <= max_msg_size; size *= 2, i++) {
    msg_sizes[i] = size;

    /* Allocate memory for source and destination buffers */
    long *source = (long *)shmem_malloc(size * stride * sizeof(long));
    long *dest = (long *)shmem_malloc(size * stride * sizeof(long));

    if (source == NULL || dest == NULL) {
      fprintf(stderr, "PE %d: Memory allocation failed\n", shmem_my_pe());
      shmem_global_exit(1);
    }

    /* Initialize source buffer */
    for (int j = 0; j < size * stride; j++) {
      source[j] = j;
    }

    /* Sync PEs */
    shmem_barrier_all();

    /* Start timer */
    double start_time = mysecond();

    /* Perform ntimes bidirectional shmem_igets */
    for (int j = 0; j < ntimes; j++) {
#if defined(USE_14) || defined(USE_15)
      shmem_iget(dest, source, 1, stride, size, 1); /* PE 0 sends to PE 1 */
      shmem_iget(source, dest, 1, stride, size, 0); /* PE 1 sends to PE 0 */
#endif
    }
    shmem_quiet();  // Ensure completion of all operations

    /* Stop timer */
    double end_time = mysecond();

    /* Calculate average time per operation in microseconds */
    times[i] = (end_time - start_time) * 1e6 / (2 * ntimes);

    /* Calculate bidirectional bandwidth */
    bandwidths[i] = calculate_bibw(size * sizeof(long), times[i]);

    /* Debugging: Print bidirectional bandwidth information */
    // printf("PE %d: shmem_iget BiBW test - Msg Size: %d, Avg Time: %.2f us, BiBW: %.2f MB/s\n",
    //        shmem_my_pe(), size, times[i], bandwidths[i]);

    /* Free buffers */
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

/*************************************************************
  @brief Run the latency benchmark for shmem_iget
  @param min_msg_size Minimum message size for test in bytes
  @param max_msg_size Maximum message size for test in bytes
  @param ntimes Number of repetitions to get the avgs from
  @param stride Stride for shmem_iget
 *************************************************************/
void bench_shmem_iget_latency(int min_msg_size, int max_msg_size, int ntimes, int stride) {
  /* Check the number of PEs before doing anything */
  if (!check_if_exactly_2_pes()) {
    return;
  }

  int *msg_sizes;
  double *times, *latencies;
  int num_sizes = 0;

  /* Setup the benchmark */
  setup_bench(min_msg_size, max_msg_size, &num_sizes, &msg_sizes, &times, &latencies);

  /* Run the benchmark */
  for (int i = 0, size = min_msg_size; size <= max_msg_size; size *= 2, i++) {
    msg_sizes[i] = size;

    /* Allocate memory for source and destination buffers */
    long *source = (long *)shmem_malloc(size * stride * sizeof(long));
    long *dest = (long *)shmem_malloc(size * stride * sizeof(long));

    if (source == NULL || dest == NULL) {
      fprintf(stderr, "PE %d: Memory allocation failed\n", shmem_my_pe());
      shmem_global_exit(1);
    }

    /* Initialize source buffer */
    for (int j = 0; j < size; j++) {
      source[j] = j;
    }

    /* Initialize total time */
    double total_time = 0.0;

    /* Sync PEs */
    shmem_barrier_all();

    /* Perform ntimes shmem_igets and accumulate total time */
    for (int j = 0; j < ntimes; j++) {
      double start_time = mysecond();
#if defined(USE_14) || defined(USE_15)
      shmem_iget(dest, source, 1, 1, size, 1);
#endif
      shmem_quiet();
      double end_time = mysecond();
      total_time += (end_time - start_time) * 1e6;
    }

    /* Calculate average latency per operation in microseconds */
    times[i] = total_time / ntimes;
    latencies[i] = times[i];

    /* Debugging: Print latency information */
    // printf("PE %d: shmem_iget Latency test - Msg Size: %d, Avg Latency: %.2f us\n",
    //        shmem_my_pe(), size, times[i]);

    /* Free buffers */
    shmem_free(source);
    shmem_free(dest);
  }

  /* Display results */
  shmem_barrier_all();
  if (shmem_my_pe() == 0) {
    display_results(times, msg_sizes, latencies, "latency", num_sizes);
  }
  shmem_barrier_all();

  /* Free memory */
  free(msg_sizes);
  free(times);
  free(latencies);
}
