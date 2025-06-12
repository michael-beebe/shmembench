/**
  @file shmem_atomic_set.h
  @brief Header file for shmem_atomic_set latency benchmark with support for
  OpenSHMEM 1.4 and 1.5
*/

#ifndef SHMEM_ATOMIC_SET_H
#define SHMEM_ATOMIC_SET_H

#include <shmem.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shmembench.h"

/**
  @brief Run the latency benchmark for shmem_atomic_set
  @param ntimes Number of iterations for the benchmark
 */
void bench_shmem_atomic_set_latency(int ntimes);

#endif /* SHMEM_ATOMIC_SET_H */
