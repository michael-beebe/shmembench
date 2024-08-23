/**
  @file shmem_atomic_fetch.h
  @brief Header file for shmem_atomic_fetch latency benchmark with support for OpenSHMEM 1.4 and 1.5
*/

#ifndef SHMEM_ATOMIC_FETCH_H
#define SHMEM_ATOMIC_FETCH_H

#include <shmem.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "shmembench.h"

/**
  @brief Run the latency benchmark for shmem_atomic_fetch
  @param ntimes Number of iterations for the benchmark
 */
void bench_shmem_atomic_fetch_latency(int ntimes);

#endif /* SHMEM_ATOMIC_FETCH_H */
