/**
  @file shmem_put.h
*/

#ifndef SHMEM_PUT_H
#define SHMEM_PUT_H

#include <shmem.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "shmembench.h"

/**
  TODO:
  need to rething the return value. maybe return a matrix
  that contain (time | message size | bandwidth/latency)
  for reporting results, or maybe we write a print_results()
  funtion that is called in the benchmarks themselves
 */

/**
  @brief Run the bandwidth benchmark for shmem_put
  @param min_msg_size Minimum message size for test in bytes
  @param max_msg_size Maximum message size for test in bytes
 */
void bench_shmem_put_bw(int min_msg_size, int max_msg_size);

/**
  @brief Run the bidirectional bandwidth benchmark for shmem_put
  @param min_msg_size Minimum message size for test in bytes
  @param max_msg_size Maximum message size for test in bytes
 */
void bench_shmem_put_bibw(int min_msg_size, int max_msg_size);

/**
  @brief Run the latency benchmark for shmem_put
  @param min_msg_size Minimum message size for test in bytes
  @param max_msg_size Maximum message size for test in bytes
 */
void bench_shmem_put_latency(int min_msg_size, int max_msg_size);

#endif /* SHMEM_PUT_H */
