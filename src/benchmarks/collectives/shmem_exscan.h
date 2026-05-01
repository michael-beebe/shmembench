/**
  @file shmem_exscan.h
  @brief Header file for shmem_sum_exscan benchmarks.
  @author Michael Beebe (Texas Tech University)
 */

#ifndef SHMEM_EXSCAN_H
#define SHMEM_EXSCAN_H

#include <shmem.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "shmembench.h"
#include "parse_opts.h"

/**
  @brief Run the bandwidth benchmark for shmem_sum_exscan
  @param opts Benchmark options given by the user 
 */
void bench_shmem_sum_exscan_bw(options * opts);

#endif /* SHMEM_EXSCAN_H */
