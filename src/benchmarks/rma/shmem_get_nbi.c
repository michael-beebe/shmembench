/**
  @file shmem_get_nbi.c
  @author Michael Beebe (Texas Tech University)
*/

#include "shmem_get_nbi.h"

/*************************************************************
  @brief Run the bandwidth benchmark for shmem_get_nbi
  @param min_msg_size Minimum message size for test in bytes
  @param max_msg_size Maximum message size for test in bytes
  @param ntimes Number of repetitions to get the avgs from
 *************************************************************/
void bench_shmem_get_nbi_bw(int min_msg_size, int max_msg_size, int ntimes) {
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
    msg_sizes[i] = size;

    /* Source and destination arrays for the shmem_get_nbi */
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

    /* Perform ntimes shmem_get_nbi */
    for (int j = 0; j < ntimes; j++) {
      shmem_get_nbi(dest, source, size, 1);
      shmem_quiet();
    }

    /* Stop timer */
    end_time = mysecond();

    /* Calculate average time per operation in microseconds */
    times[i] = (end_time - start_time) * 1e6 / ntimes;

    /* Calculate bandwidth */
    bandwidths[i] = calculate_bw(size * sizeof(long), times[i]);

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

/*************************************************************
  @brief Run the bidirectional bandwidth benchmark for shmem_get_nbi
  @param min_msg_size Minimum message size for test in bytes
  @param max_msg_size Maximum message size for test in bytes
  @param ntimes Number of repetitions to get the avgs from
 *************************************************************/
void bench_shmem_get_nbi_bibw(int min_msg_size, int max_msg_size, int ntimes) {
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
    msg_sizes[i] = size;

    /* Source and destination arrays for the shmem_get_nbi */
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

    /* Perform ntimes bidirectional shmem_get_nbi */
    for (int j = 0; j < ntimes; j++) {
      shmem_get_nbi(dest, source, size, 1); /* PE 0 sends to PE 1 */
      shmem_get_nbi(source, dest, size, 0); /* PE 1 sends to PE 0 */
      shmem_quiet();
    }

    /* Stop timer */
    end_time = mysecond();

    /* Calculate average time per operation in microseconds */
    times[i] = (end_time - start_time) * 1e6 / (2 * ntimes);

    /* Calculate bidirectional bandwidth */
    bandwidths[i] = calculate_bw(size * sizeof(long), times[i]);

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

/*************************************************************
  @brief Run the latency benchmark for shmem_get_nbi
  @param min_msg_size Minimum message size for test in bytes
  @param max_msg_size Maximum message size for test in bytes
  @param ntimes Number of repetitions to get the avgs from
 *************************************************************/
void bench_shmem_get_nbi_latency(int min_msg_size, int max_msg_size,
                                 int ntimes) {
  /* Check the number of PEs before doing anything */
  if (!check_if_exactly_2_pes()) {
    return;
  }

  /* Stuff that will be used throughout the benchmark */
  int *msg_sizes;
  double *times, *latencies;
  int num_sizes = 0;

  /* Setup the benchmark */
  setup_bench(min_msg_size, max_msg_size, &num_sizes, &msg_sizes, &times,
              &latencies);

  /* Run the benchmark */
  for (int i = 0, size = min_msg_size; size <= max_msg_size; size *= 2, i++) {
    msg_sizes[i] = size;

    /* Source and destination arrays for the shmem_get_nbi */
    long *source = (long *)shmem_malloc(size * sizeof(long));
    long *dest = (long *)shmem_malloc(size * sizeof(long));

    /* Initialize source buffer */
    for (int j = 0; j < size; j++) {
      source[j] = j;
    }

    /* Initialize total time */
    double total_time = 0.0;

    /* Sync PEs */
    shmem_barrier_all();

    /* Perform ntimes shmem_get_nbi and accumulate total time */
    for (int j = 0; j < ntimes; j++) {
      double start_time = mysecond();
      shmem_get_nbi(dest, source, size, 1);
      shmem_quiet();
      double end_time = mysecond();
      total_time += (end_time - start_time) * 1e6;
    }

    /* Calculate average latency per operation in microseconds */
    times[i] = total_time / ntimes;

    /* Record latency */
    latencies[i] = times[i];

    /* Free the buffers */
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
