/**
  @file shmem_alltoalls.h
  @brief Header file for shmem_alltoalls benchmarks
  @author Michael Beebe (Texas Tech University)
*/

#ifndef SHMEM_ALLTOALLS_H
#define SHMEM_ALLTOALLS_H

#include <shmem.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shmembench.h"

/**
  @brief Run the bandwidth benchmark for shmem_alltoalls
  @param min_msg_size Minimum message size for test in bytes
  @param max_msg_size Maximum message size for test in bytes
  @param ntimes Number of times the benchmark should run
 */
void bench_shmem_alltoalls_bw(int min_msg_size, int max_msg_size, int ntimes);

/**
  @brief Run the latency benchmark for shmem_alltoalls
  @param min_msg_size Minimum message size for test in bytes
  @param max_msg_size Maximum message size for test in bytes
  @param ntimes Number of times the benchmark should run
 */
void bench_shmem_alltoalls_latency(int min_msg_size, int max_msg_size,
                                   int ntimes);

#endif /* SHMEM_ALLTOALLS_H */
