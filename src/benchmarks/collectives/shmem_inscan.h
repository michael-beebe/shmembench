/**
  @file shmem_inscan.h
  @brief Header file for shmem_sum_inscan benchmarks.
  @author Michael Beebe (Texas Tech University)
 */

#ifndef SHMEM_INSCAN_H
#define SHMEM_INSCAN_H

#include <shmem.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "shmembench.h"
#include "parse_opts.h"

/**
  @brief Run the bandwidth benchmark for shmem_sum_inscan
  @param opts Benchmark options given by the user 
 */
void bench_shmem_sum_inscan_bw(options * opts);

#endif /* SHMEM_INSCAN_H */
