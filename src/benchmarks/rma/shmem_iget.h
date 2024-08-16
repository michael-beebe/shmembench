/**
  @file shmem_iget.h
*/

#ifndef SHMEM_IGET_H
#define SHMEM_IGET_H

#include <shmem.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "shmembench.h"

/**
  @brief Run the bandwidth benchmark for shmem_iget
  @param min_msg_size Minimum message size for test in bytes
  @param max_msg_size Maximum message size for test in bytes
 */
void bench_shmem_iget_bw(int min_msg_size, int max_msg_size);

/**
  @brief Run the bidirectional bandwidth benchmark for shmem_iget
  @param min_msg_size Minimum message size for test in bytes
  @param max_msg_size Maximum message size for test in bytes
 */
void bench_shmem_iget_bibw(int min_msg_size, int max_msg_size);

/**
  @brief Run the latency benchmark for shmem_iget
  @param min_msg_size Minimum message size for test in bytes
  @param max_msg_size Maximum message size for test in bytes
 */
void bench_shmem_iget_latency(int min_msg_size, int max_msg_size);

#endif /* SHMEM_IGET_H */
