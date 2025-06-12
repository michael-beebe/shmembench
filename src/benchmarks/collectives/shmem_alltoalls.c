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
  @param ntimes Number of repetitions to get the avgs from
 *************************************************************/
void bench_shmem_alltoalls_bw(int min_msg_size, int max_msg_size, int ntimes) {
  /* Check the number of PEs before doing anything */
  if (!check_if_atleast_2_pes()) {
    return;
  }

  /* Stuff that will be used throughout the benchmark */
  int *msg_sizes;
  double *times, *bandwidths;
  int num_sizes = 0;
  int npes = shmem_n_pes();

  /* Setup the benchmark */
  setup_bench(min_msg_size, max_msg_size, &num_sizes, &msg_sizes, &times,
              &bandwidths);

#if defined(USE_14)
  /* Setup pSync array for OpenSHMEM 1.4 */
  long *pSync = (long *)shmem_malloc(SHMEM_ALLTOALLS_SYNC_SIZE * sizeof(long));
  for (int i = 0; i < SHMEM_ALLTOALLS_SYNC_SIZE; i++) {
    pSync[i] = SHMEM_SYNC_VALUE;
  }
#endif

  /* Run the benchmark */
  for (int i = 0, size = min_msg_size; size <= max_msg_size; size *= 2, i++) {
    /* Validate the message size for the benchmark datatype */
    int valid_size = validate_typed_size(size, BENCHMARK_DATATYPE_SIZE,
                                         BENCHMARK_DATATYPE_NAME);
    msg_sizes[i] = valid_size;

    /* Allocate memory for source and destination arrays */
    BENCHMARK_TYPE_PTR(source) = BENCHMARK_MALLOC(valid_size * npes);
    BENCHMARK_TYPE_PTR(dest) = BENCHMARK_MALLOC(valid_size * npes);

    /* Initialize the source buffer with data */
    for (int j = 0; j < valid_size * npes; j++) {
      BENCHMARK_INIT_ELEMENT(source, j, shmem_my_pe() + j);
    }

    double start_time, end_time;

    /* Synchronize all PEs before starting the benchmark */
    shmem_barrier_all();
    start_time = mysecond(); /* Record the start time */

    /* Perform the shmem_alltoalls operation for the specified number of times
     */
    for (int j = 0; j < ntimes; j++) {
      // printf("PE %d: Before shmem_alltoalls64, size = %d, npes = %d\n",
      // shmem_my_pe(), size, npes);
#if defined(USE_14)
      shmem_alltoalls64(dest, source, 1, valid_size, valid_size, 0, 0, npes,
                        pSync);
#elif defined(USE_15)
      shmem_alltoalls(SHMEM_TEAM_WORLD, dest, source, 1, valid_size, 1);
#endif
    }
    shmem_quiet();
    end_time = mysecond(); /* Record the end time */

    /* Calculate the average time per operation and bandwidth */
    times[i] = (end_time - start_time) * 1e6 / ntimes;
    bandwidths[i] =
        calculate_bw(valid_size * BENCHMARK_DATATYPE_SIZE, times[i]);

    /* Free the allocated memory for source and destination arrays */
    BENCHMARK_FREE(source);
    BENCHMARK_FREE(dest);
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
#if defined(USE_14)
  shmem_free(pSync);
#endif
}

/*************************************************************
  @brief Run the latency benchmark for shmem_alltoalls
  @param min_msg_size Minimum message size for test in bytes
  @param max_msg_size Maximum message size for test in bytes
  @param ntimes Number of repetitions to get the avgs from
 *************************************************************/
void bench_shmem_alltoalls_latency(int min_msg_size, int max_msg_size,
                                   int ntimes) {
  /* Check the number of PEs before doing anything */
  if (!check_if_atleast_2_pes()) {
    return;
  }

  /* Stuff that will be used throughout the benchmark */
  int *msg_sizes;
  double *times, *latencies;
  int num_sizes = 0;
  int npes = shmem_n_pes();

  /* Setup the benchmark */
  setup_bench(min_msg_size, max_msg_size, &num_sizes, &msg_sizes, &times,
              &latencies);

#if defined(USE_14)
  /* Setup pSync array for OpenSHMEM 1.4 */
  long *pSync = (long *)shmem_malloc(SHMEM_ALLTOALLS_SYNC_SIZE * sizeof(long));
  for (int i = 0; i < SHMEM_ALLTOALLS_SYNC_SIZE; i++) {
    pSync[i] = SHMEM_SYNC_VALUE;
  }
#endif

  /* Run the benchmark */
  for (int i = 0, size = min_msg_size; size <= max_msg_size; size *= 2, i++) {
    /* Validate the message size for the benchmark datatype */
    int valid_size = validate_typed_size(size, BENCHMARK_DATATYPE_SIZE,
                                         BENCHMARK_DATATYPE_NAME);
    msg_sizes[i] = valid_size;

    /* Allocate memory for source and destination arrays */
    BENCHMARK_TYPE_PTR(source) = BENCHMARK_MALLOC(valid_size * npes);
    BENCHMARK_TYPE_PTR(dest) = BENCHMARK_MALLOC(valid_size * npes);

    /* Initialize the source buffer with data */
    for (int j = 0; j < valid_size * npes; j++) {
      BENCHMARK_INIT_ELEMENT(source, j, shmem_my_pe() + j);
    }

    double start_time, end_time;

    /* Synchronize all PEs before starting the benchmark */
    shmem_barrier_all();
    start_time = mysecond(); /* Record the start time */

    /* Perform the shmem_alltoalls operation for the specified number of times
     */
    for (int j = 0; j < ntimes; j++) {
#if defined(USE_14)
      shmem_alltoalls64(dest, source, 1, valid_size, valid_size, 0, 0, npes,
                        pSync);
#elif defined(USE_15)
      shmem_alltoalls(SHMEM_TEAM_WORLD, dest, source, 1, valid_size, 1);
#endif
    }
    shmem_quiet();

    end_time = mysecond(); /* Record the end time */

    /* Calculate the average time per operation and record latency */
    times[i] = (end_time - start_time) * 1e6 / ntimes;
    latencies[i] = calculate_latency(times[i]);

    /* Free the allocated memory for source and destination arrays */
    BENCHMARK_FREE(source);
    BENCHMARK_FREE(dest);
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
#if defined(USE_14)
  shmem_free(pSync);
#endif
}
