/**
  @file shmem_atomic_add.h
  @brief Header file for shmem_atomic_add latency benchmark without size
  parameter
*/

#ifndef SHMEM_ATOMIC_ADD_H
#define SHMEM_ATOMIC_ADD_H

#include <shmem.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shmembench.h"

/**
  @brief Run the latency benchmark for shmem_atomic_add
  @param ntimes Number of iterations for the benchmark
 */
void bench_shmem_atomic_add_latency(int ntimes);

#endif /* SHMEM_ATOMIC_ADD_H */
