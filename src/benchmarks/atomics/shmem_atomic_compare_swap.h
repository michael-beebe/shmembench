/**
  @file shmem_atomic_compare_swap.h
  @brief Header file for shmem_atomic_compare_swap latency benchmark with
  improvements
*/

#ifndef SHMEM_ATOMIC_COMPARE_SWAP_H
#define SHMEM_ATOMIC_COMPARE_SWAP_H

#include <shmem.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shmembench.h"

/**
  @brief Run the latency benchmark for shmem_atomic_compare_swap
  @param ntimes Number of iterations for the benchmark
 */
void bench_shmem_atomic_compare_swap_latency(int ntimes);

#endif /* SHMEM_ATOMIC_COMPARE_SWAP_H */
