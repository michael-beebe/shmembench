/**
  @file shmem_alltoallmem.h
  @author Michael Beebe (Texas Tech University)
*/

#ifndef SHMEM_ALLTOALLMEM_H
#define SHMEM_ALLTOALLMEM_H

#include <shmem.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "shmembench.h"

/**
  @brief Run the bandwidth benchmark for shmem_alltoallmem
  @param min_msg_size Minimum message size for test in bytes
  @param max_msg_size Maximum message size for test in bytes
  @param ntimes Number of iterations for the benchmark
 */
void bench_shmem_alltoallmem_bw(int min_msg_size, int max_msg_size, int ntimes);

#endif /* SHMEM_ALLTOALLMEM_H */
