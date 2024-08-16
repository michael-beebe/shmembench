/**
  @file shmem_put_nbi.h
*/

#ifndef SHMEM_PUT_NBI_H
#define SHMEM_PUT_NBI_H

#include <shmem.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "shmembench.h"

/**
  @brief Run the bandwidth benchmark for shmem_put_nbi
  @param min_msg_size Minimum message size for test in bytes
  @param max_msg_size Maximum message size for test in bytes
 */
void bench_shmem_put_nbi_bw(int min_msg_size, int max_msg_size);

/**
  @brief Run the bidirectional bandwidth benchmark for shmem_put_nbi
  @param min_msg_size Minimum message size for test in bytes
  @param max_msg_size Maximum message size for test in bytes
 */
void bench_shmem_put_nbi_bibw(int min_msg_size, int max_msg_size);

/**
  @brief Run the latency benchmark for shmem_put_nbi
  @param min_msg_size Minimum message size for test in bytes
  @param max_msg_size Maximum message size for test in bytes
 */
void bench_shmem_put_nbi_latency(int min_msg_size, int max_msg_size);

#endif /* SHMEM_PUT_NBI_H */

