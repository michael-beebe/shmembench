/**
  @file shmem_barrier_all.h
  @brief Header file for shmem_barrier_all benchmark routines
*/

#ifndef SHMEM_BARRIER_ALL_H
#define SHMEM_BARRIER_ALL_H

#include <shmem.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shmembench.h"

/**
  @brief Run the latency benchmark for shmem_barrier_all
  @param ntimes Number of times to repeat the operation
 */
void bench_shmem_barrier_all_latency(int ntimes);

#endif /* SHMEM_BARRIER_ALL_H */
