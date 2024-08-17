/**
  @file shmem_broadcast.h
  @brief Header file for shmem_broadcast benchmarks.
 */

#ifndef SHMEM_BROADCAST_H
#define SHMEM_BROADCAST_H

#include <shmem.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "shmembench.h"

/**
  @brief Run the bandwidth benchmark for shmem_broadcast
  @param min_msg_size Minimum message size for the test in bytes
  @param max_msg_size Maximum message size for the test in bytes
  @param ntimes Number of times to run the benchmark
 */
void bench_shmem_broadcast_bw(int min_msg_size, int max_msg_size, int ntimes);

/**
  @brief Run the latency benchmark for shmem_broadcast
  @param min_msg_size Minimum message size for the test in bytes
  @param max_msg_size Maximum message size for the test in bytes
  @param ntimes Number of times to run the benchmark
 */
void bench_shmem_broadcast_latency(int min_msg_size, int max_msg_size, int ntimes);

#endif /* SHMEM_BROADCAST_H */
