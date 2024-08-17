/**
  @file shmem_collect.c
  @brief Implementation of the shmem_collect benchmarks.
 */

#include "shmem_collect.h"

/*************************************************************
  @brief Run the bandwidth benchmark for shmem_collect
  @param min_msg_size Minimum message size for the test in bytes
  @param max_msg_size Maximum message size for the test in bytes
  @param ntimes Number of times to run the benchmark
 *************************************************************/
void bench_shmem_collect_bw(int min_msg_size, int max_msg_size, int ntimes) {
  if (!check_if_atleast_2_pes()) {
    return;
  }

  int *msg_sizes;
  double *times, *bandwidths;
  int num_sizes = 0;

  setup_bench(min_msg_size, max_msg_size, &num_sizes, &msg_sizes, &times, &bandwidths);

  for (int i = 0, size = min_msg_size; size <= max_msg_size; size *= 2, i++) {
    msg_sizes[i] = size;

    long *source = (long *)shmem_malloc(size * sizeof(long));
    long *dest = (long *)shmem_malloc(size * shmem_n_pes() * sizeof(long));

    for (int j = 0; j < size; j++) {
      source[j] = j;
    }

    double start_time, end_time;

    shmem_barrier_all();
    start_time = mysecond();

    for (int j = 0; j < ntimes; j++) {
      shmem_collect(SHMEM_TEAM_WORLD, dest, source, size);
    }

    end_time = mysecond();

    times[i] = (end_time - start_time) * 1e6 / ntimes;
    bandwidths[i] = calculate_bw(size * sizeof(long), times[i]);

    shmem_free(source);
    shmem_free(dest);
  }

  shmem_barrier_all();
  if (shmem_my_pe() == 0) {
    display_results(times, msg_sizes, bandwidths, "bw", num_sizes);
  }
  shmem_barrier_all();

  free(msg_sizes);
  free(times);
  free(bandwidths);
}

/*************************************************************
  @brief Run the latency benchmark for shmem_collect
  @param min_msg_size Minimum message size for the test in bytes
  @param max_msg_size Maximum message size for the test in bytes
  @param ntimes Number of times to run the benchmark
 *************************************************************/
void bench_shmem_collect_latency(int min_msg_size, int max_msg_size, int ntimes) {
  if (!check_if_atleast_2_pes()) {
    return;
  }

  int *msg_sizes;
  double *times, *latencies;
  int num_sizes = 0;

  setup_bench(min_msg_size, max_msg_size, &num_sizes, &msg_sizes, &times, &latencies);

  for (int i = 0, size = min_msg_size; size <= max_msg_size; size *= 2, i++) {
    msg_sizes[i] = size;

    long *source = (long *)shmem_malloc(size * sizeof(long));
    long *dest = (long *)shmem_malloc(size * shmem_n_pes() * sizeof(long));

    for (int j = 0; j < size; j++) {
      source[j] = j;
    }

    double start_time, end_time;

    shmem_barrier_all();
    start_time = mysecond();

    for (int j = 0; j < ntimes; j++) {
      shmem_collect(SHMEM_TEAM_WORLD, dest, source, size);
    }

    end_time = mysecond();

    times[i] = (end_time - start_time) * 1e6 / ntimes;
    latencies[i] = calculate_latency(times[i]);

    shmem_free(source);
    shmem_free(dest);
  }

  shmem_barrier_all();
  if (shmem_my_pe() == 0) {
    display_results(times, msg_sizes, latencies, "latency", num_sizes);
  }
  shmem_barrier_all();

  free(msg_sizes);
  free(times);
  free(latencies);
}
