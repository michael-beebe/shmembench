/**
  @file shmem_put.h
  @author Michael Beebe (Texas Tech University)
*/

#ifndef SHMEM_PUT_H
#define SHMEM_PUT_H

#include <shmem.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shmembench.h"

/**
  @brief Run the bandwidth benchmark for shmem_put
  @param min_msg_size Minimum message size for test in bytes
  @param max_msg_size Maximum message size for test in bytes
 */
void bench_shmem_put_bw(int min_msg_size, int max_msg_size, int ntimes);

/**
  @brief Run the bidirectional bandwidth benchmark for shmem_put
  @param min_msg_size Minimum message size for test in bytes
  @param max_msg_size Maximum message size for test in bytes
 */
void bench_shmem_put_bibw(int min_msg_size, int max_msg_size, int ntimes);

/**
  @brief Run the latency benchmark for shmem_put
  @param min_msg_size Minimum message size for test in bytes
  @param max_msg_size Maximum message size for test in bytes
 */
void bench_shmem_put_latency(int min_msg_size, int max_msg_size, int ntimes);

#endif /* SHMEM_PUT_H */
