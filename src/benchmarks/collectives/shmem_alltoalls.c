/**
  @file shmem_alltoalls.c
  @brief Source file for shmem_alltoalls benchmarks
  @author Michael Beebe (Texas Tech University)
*/

#include "shmem_alltoalls.h"

/*************************************************************
  @brief Run the bandwidth benchmark for shmem_alltoalls
  @param min_msg_size Minimum message size for test in bytes
  @param max_msg_size Maximum message size for test in bytes
  @param ntimes Number of times the benchmark should run
 *************************************************************/
void bench_shmem_alltoalls_bw(int min_msg_size, int max_msg_size, int ntimes) {
  /* Ensure there are at least 2 PEs available to run the benchmark */
  if (!check_if_atleast_2_pes()) {
    return;
  }

  /* Variables for message sizes, times, and bandwidths */
  int *msg_sizes;
  double *times, *bandwidths;
  int num_sizes = 0;

  /* Initialize the benchmark setup, including message sizes, times, and bandwidths */
  setup_bench(min_msg_size, max_msg_size, &num_sizes, &msg_sizes, &times, &bandwidths);

  /* Get the number of processing elements (PEs) */
  int npes = shmem_n_pes();

#if defined(USE_14)
  /* Define and initialize pSync for OpenSHMEM 1.4 */
  long pSync[SHMEM_ALLTOALLS_SYNC_SIZE];
  for (int i = 0; i < SHMEM_ALLTOALLS_SYNC_SIZE; i++) {
    pSync[i] = SHMEM_SYNC_VALUE;
  }
  /* Ensure all PEs are synchronized before starting the benchmark */
  shmem_barrier_all();
#endif

  /* Loop through each message size, doubling the size at each iteration */
  for (int i = 0, size = min_msg_size; size <= max_msg_size; size *= 2, i++) {
    msg_sizes[i] = size;

    /* Allocate memory for source and destination arrays */
    long *source = (long *)shmem_malloc(size * npes * sizeof(long));
    long *dest = (long *)shmem_malloc(size * npes * sizeof(long));

    /* Initialize the source buffer with data */
    for (int j = 0; j < size * npes; j++) {
      source[j] = shmem_my_pe() + j;
    }

    double start_time, end_time;

    /* Synchronize all PEs before starting the benchmark */
    shmem_barrier_all();
    start_time = mysecond();  /* Record the start time */

    /* Perform the shmem_alltoalls operation for the specified number of times */
    for (int j = 0; j < ntimes; j++) {
#if defined(USE_14)
      shmem_alltoalls64(dest, source, 1, size, size, 0, 0, npes, pSync);
#elif defined(USE_15)
      shmem_alltoalls(SHMEM_TEAM_WORLD, dest, source, 1, size, 1);
#endif
    }

    end_time = mysecond();  /* Record the end time */

    /* Calculate the average time per operation and bandwidth */
    times[i] = (end_time - start_time) * 1e6 / ntimes;
    bandwidths[i] = calculate_bw(size * sizeof(long), times[i]);

    /* Free the allocated memory for source and destination arrays */
    shmem_free(source);
    shmem_free(dest);
  }

  /* Synchronize all PEs before displaying the results */
  shmem_barrier_all();
  if (shmem_my_pe() == 0) {
    /* Display the benchmark results */
    display_results(times, msg_sizes, bandwidths, "bw", num_sizes);
  }
  shmem_barrier_all();

  /* Free the memory allocated for message sizes, times, and bandwidths */
  free(msg_sizes);
  free(times);
  free(bandwidths);
}

/*************************************************************
  @brief Run the latency benchmark for shmem_alltoalls
  @param min_msg_size Minimum message size for test in bytes
  @param max_msg_size Maximum message size for test in bytes
  @param ntimes Number of times the benchmark should run
 *************************************************************/
void bench_shmem_alltoalls_latency(int min_msg_size, int max_msg_size, int ntimes) {
  /* Ensure there are at least 2 PEs available to run the benchmark */
  if (!check_if_atleast_2_pes()) {
    return;
  }

  /* Variables for message sizes, times, and latencies */
  int *msg_sizes;
  double *times, *latencies;
  int num_sizes = 0;

  /* Initialize the benchmark setup, including message sizes, times, and latencies */
  setup_bench(min_msg_size, max_msg_size, &num_sizes, &msg_sizes, &times, &latencies);

  /* Get the number of processing elements (PEs) */
  int npes = shmem_n_pes();

#if defined(USE_14)
  /* Define and initialize pSync for OpenSHMEM 1.4 */
  long pSync[SHMEM_ALLTOALLS_SYNC_SIZE];
  for (int i = 0; i < SHMEM_ALLTOALLS_SYNC_SIZE; i++) {
    pSync[i] = SHMEM_SYNC_VALUE;
  }
  /* Ensure all PEs are synchronized before starting the benchmark */
  shmem_barrier_all();
#endif

  /* Loop through each message size, doubling the size at each iteration */
  for (int i = 0, size = min_msg_size; size <= max_msg_size; size *= 2, i++) {
    msg_sizes[i] = size;

    /* Allocate memory for source and destination arrays */
    long *source = (long *)shmem_malloc(size * npes * sizeof(long));
    long *dest = (long *)shmem_malloc(size * npes * sizeof(long));

    /* Initialize the source buffer with data */
    for (int j = 0; j < size * npes; j++) {
      source[j] = shmem_my_pe() + j;
    }

    double start_time, end_time;

    /* Synchronize all PEs before starting the benchmark */
    shmem_barrier_all();
    start_time = mysecond();  /* Record the start time */

    /* Perform the shmem_alltoalls operation for the specified number of times */
    for (int j = 0; j < ntimes; j++) {
#if defined(USE_14)
      shmem_alltoalls64(dest, source, 1, size, size, 0, 0, npes, pSync);
#elif defined(USE_15)
      shmem_alltoalls(SHMEM_TEAM_WORLD, dest, source, 1, size, 1);
#endif
    }

    end_time = mysecond();  /* Record the end time */

    /* Calculate the average time per operation and record latency */
    times[i] = (end_time - start_time) * 1e6 / ntimes;
    latencies[i] = calculate_latency(times[i]);

    /* Free the allocated memory for source and destination arrays */
    shmem_free(source);
    shmem_free(dest);
  }

  /* Synchronize all PEs before displaying the results */
  shmem_barrier_all();
  if (shmem_my_pe() == 0) {
    /* Display the benchmark results */
    display_results(times, msg_sizes, latencies, "latency", num_sizes);
  }
  shmem_barrier_all();

  /* Free the memory allocated for message sizes, times, and latencies */
  free(msg_sizes);
  free(times);
  free(latencies);
}



