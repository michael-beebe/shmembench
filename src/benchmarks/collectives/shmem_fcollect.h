/**
  @file shmem_fcollect.h
  @brief Header file for shmem_fcollect benchmarks.
  @author Michael Beebe (Texas Tech University)
 */

#ifndef SHMEM_FCOLLECT_H
#define SHMEM_FCOLLECT_H

#include <shmem.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "shmembench.h"

/**
  @brief Run the bandwidth benchmark for shmem_fcollect
  @param min_msg_size Minimum message size for the test in bytes
  @param max_msg_size Maximum message size for the test in bytes
  @param ntimes Number of times to run the benchmark
 */
void bench_shmem_fcollect_bw(int min_msg_size, int max_msg_size, int ntimes);

/**
  @brief Run the latency benchmark for shmem_fcollect
  @param min_msg_size Minimum message size for the test in bytes
  @param max_msg_size Maximum message size for the test in bytes
  @param ntimes Number of times to run the benchmark
 */
void bench_shmem_fcollect_latency(int min_msg_size, int max_msg_size,
                                  int ntimes);

#endif /* SHMEM_FCOLLECT_H */
