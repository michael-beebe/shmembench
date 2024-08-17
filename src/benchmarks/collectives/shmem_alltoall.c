/**
  @file shmem_alltoall.c
*/

#include "shmem_alltoall.h"

/*************************************************************
  @brief Run the bandwidth benchmark for shmem_alltoall
  @param min_msg_size Minimum message size for test in bytes
  @param max_msg_size Maximum message size for test in bytes
  @param ntimes Number of iterations for the benchmark
 *************************************************************/
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
  setup_bench(min_msg_size, max_msg_size, &num_sizes,
              &msg_sizes, &times, &bandwidths);

  /* Get the number of PEs */
  int npes = shmem_n_pes();

  /* Run the benchmark */
  for (int i = 0, size = min_msg_size; size <= max_msg_size; size *= 2, i++) {
    msg_sizes[i] = size;

    /* Source and destination arrays for shmem_alltoall */
    long *source = (long *)shmem_malloc(size * npes * sizeof(long));
    long *dest = (long *)shmem_malloc(size * npes * sizeof(long));

    /* Initialize source buffer */
    for (int j = 0; j < size * npes; j++) {
      source[j] = shmem_my_pe() + j;
    }

    /* Initialize start and end time */
    double start_time, end_time;

    /* Sync PEs */
    shmem_barrier_all();

    /* Start timer */
    start_time = mysecond();

    /* Perform NTIMES shmem_alltoall operations */
    for (int j = 0; j < ntimes; j++) {
      shmem_alltoall(SHMEM_TEAM_WORLD, dest, source, size);
    }

    /* Stop timer */
    end_time = mysecond();

    /* Calculate average time per operation in useconds */
    times[i] = (end_time - start_time) * 1e6 / ntimes;

    /* Calculate bandwidth */
    bandwidths[i] = calculate_bw(size * npes * sizeof(long), times[i]);

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
  @brief Run the latency benchmark for shmem_alltoall
  @param min_msg_size Minimum message size for test in bytes
  @param max_msg_size Maximum message size for test in bytes
  @param ntimes Number of iterations for the benchmark
 *************************************************************/
void bench_shmem_alltoall_latency(int min_msg_size, int max_msg_size, int ntimes) {
  /* Check the number of PEs before doing anything */
  if (!check_if_atleast_2_pes()) {
    return;
  }

  /* Stuff that will be used throughout the benchmark */
  int *msg_sizes;
  double *times, *latencies;
  int num_sizes = 0;

  /* Setup the benchmark */
  setup_bench(min_msg_size, max_msg_size, &num_sizes,
              &msg_sizes, &times, &latencies);

  /* Get the number of PEs */
  int npes = shmem_n_pes();

  /* Run the benchmark */
  for (int i = 0, size = min_msg_size; size <= max_msg_size; size *= 2, i++) {
    msg_sizes[i] = size;

    /* Source and destination arrays for shmem_alltoall */
    long *source = (long *)shmem_malloc(size * npes * sizeof(long));
    long *dest = (long *)shmem_malloc(size * npes * sizeof(long));

    /* Initialize source buffer */
    for (int j = 0; j < size * npes; j++) {
      source[j] = shmem_my_pe() + j;
    }

    /* Initialize start and end time */
    double start_time, end_time;

    /* Sync PEs */
    shmem_barrier_all();

    /* Start timer */
    start_time = mysecond();

    /* Perform a single shmem_alltoall operation */
    shmem_alltoall(SHMEM_TEAM_WORLD, dest, source, size);

    /* Stop timer */
    end_time = mysecond();

    /* Calculate latency for the single operation in microseconds */
    times[i] = (end_time - start_time) * 1e6;

    /* Record latency */
    latencies[i] = times[i] / ntimes;

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
