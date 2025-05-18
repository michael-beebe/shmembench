/**
  @file shmem_alltoall.c
  @author Michael Beebe (Texas Tech University)
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
  setup_bench(min_msg_size, max_msg_size, &num_sizes, &msg_sizes, &times,
              &bandwidths);

  /* Get the number of PEs */
  int npes = shmem_n_pes();

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

    /* Source and destination arrays for shmem_alltoall */
    long *source = (long *)shmem_malloc(valid_size * npes * sizeof(long));
    long *dest = (long *)shmem_malloc(valid_size * npes * sizeof(long));

    /* Initialize source buffer */
    for (int j = 0; j < valid_size * npes; j++) {
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
#if defined(USE_14)
      shmem_alltoall64(dest, source, valid_size, 0, 0, npes, pSync);
#elif defined(USE_15)
      shmem_alltoall(SHMEM_TEAM_WORLD, dest, source, valid_size);
#endif
    }
    shmem_quiet();

    /* Stop timer */
    end_time = mysecond();

    /* Calculate average time per operation in useconds */
    times[i] = (end_time - start_time) * 1e6 / ntimes;

    /* Calculate bandwidth */
    bandwidths[i] = calculate_bw(valid_size * npes * sizeof(long), times[i]);

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
void bench_shmem_alltoall_latency(int min_msg_size, int max_msg_size,
                                  int ntimes) {
  /* Check the number of PEs before doing anything */
  if (!check_if_atleast_2_pes()) {
    return;
  }

  /* Stuff that will be used throughout the benchmark */
  int *msg_sizes;
  double *times, *latencies;
  int num_sizes = 0;

  /* Setup the benchmark */
  setup_bench(min_msg_size, max_msg_size, &num_sizes, &msg_sizes, &times,
              &latencies);

  /* Get the number of PEs */
  int npes = shmem_n_pes();

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

    /* Source and destination arrays for shmem_alltoall */
    long *source = (long *)shmem_malloc(valid_size * npes * sizeof(long));
    long *dest = (long *)shmem_malloc(valid_size * npes * sizeof(long));

    /* Initialize source buffer */
    for (int j = 0; j < valid_size * npes; j++) {
      source[j] = shmem_my_pe() + j;
    }

    /* Sync PEs */
    shmem_barrier_all();

    /* Initialize total time */
    double total_time = 0.0;

    /* Perform a single shmem_alltoall operation */
    for (int j = 0; j < ntimes; j++) {
      double start_time = mysecond();
#if defined(USE_14)
      shmem_alltoall64(dest, source, valid_size, 0, 0, npes, pSync);
#elif defined(USE_15)
      shmem_alltoall(SHMEM_TEAM_WORLD, dest, source, valid_size);
#endif
      double end_time = mysecond();
      total_time += (end_time - start_time) * 1e6;
    }

    /* Calculate latency for the single operation in microseconds */
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
