/**
  @file shmem_atomic_swap.h
  @brief Header file for shmem_atomic_swap latency benchmark with support for OpenSHMEM 1.4 and 1.5
*/

#ifndef SHMEM_ATOMIC_SWAP_H
#define SHMEM_ATOMIC_SWAP_H

#include <shmem.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "shmembench.h"

/**
  @brief Run the latency benchmark for shmem_atomic_swap
  @param ntimes Number of iterations for the benchmark
 */
void bench_shmem_atomic_swap_latency(int ntimes);

#endif /* SHMEM_ATOMIC_SWAP_H */
