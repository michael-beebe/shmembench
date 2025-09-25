/**
  @file shmem_alltoallsmem.h
  @brief Header file for shmem_alltoallsmem benchmarks
  @author Michael Beebe (Texas Tech University)
*/

#ifndef SHMEM_ALLTOALLSMEM_H
#define SHMEM_ALLTOALLSMEM_H

#include <shmem.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "shmembench.h"

/**
  @brief Run the bandwidth benchmark for shmem_alltoallsmem
  @param min_msg_size Minimum message size for test in bytes
  @param max_msg_size Maximum message size for test in bytes
  @param ntimes Number of times the benchmark should run
 */
void bench_shmem_alltoallsmem_bw(int min_msg_size, int max_msg_size, int ntimes);

#endif /* SHMEM_ALLTOALLSMEM_H */
