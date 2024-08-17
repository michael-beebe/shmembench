/**
  @file shmem_alltoall.h
  @author Michael Beebe (Texas Tech University)
*/

#ifndef SHMEM_ALLTOALL_H
#define SHMEM_ALLTOALL_H

#include <shmem.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "shmembench.h"

/**
  @brief Run the bandwidth benchmark for shmem_alltoall
  @param min_msg_size Minimum message size for test in bytes
  @param max_msg_size Maximum message size for test in bytes
  @param ntimes Number of iterations for the benchmark
 */
void bench_shmem_alltoall_bw(int min_msg_size, int max_msg_size, int ntimes);

/**
  @brief Run the latency benchmark for shmem_alltoall
  @param min_msg_size Minimum message size for test in bytes
  @param max_msg_size Maximum message size for test in bytes
  @param ntimes Number of iterations for the benchmark
 */
void bench_shmem_alltoall_latency(int min_msg_size, int max_msg_size, int ntimes);

#endif /* SHMEM_ALLTOALL_H */
