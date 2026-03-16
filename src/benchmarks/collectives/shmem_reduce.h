/**
  @file shmem_reduce.h
  @brief Header file for shmem_reduce benchmarks.
  @author Michael Beebe (Texas Tech University)
 */

#ifndef SHMEM_REDUCE_H
#define SHMEM_REDUCE_H

#include <shmem.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "shmembench.h"
#include "parse_opts.h"

/**
  @brief Run the bandwidth benchmark for shmem_<operation>_reduce
  @param opts Benchmark options given by the user 
 */
#define BENCH_SHMEM_REDUCE_HEADER_DEFINITION(_op)  \
  void bench_shmem_##_op##_reduce_bw(options * opts);

#define BENCH_SHMEM_REDUCE_OPERATIONS(X) \
  X(and) \
  X(or) \
  X(xor) \
  X(max) \
  X(min) \
  X(sum) \
  X(prod) 

#define BENCH_SHMEM_REDUCE(_op) \
  BENCH_SHMEM_REDUCE_HEADER_DEFINITION(_op)

BENCH_SHMEM_REDUCE_OPERATIONS(BENCH_SHMEM_REDUCE)
#undef BENCH_SHMEM_REDUCE

#endif /* SHMEM_REDUCE_H */
