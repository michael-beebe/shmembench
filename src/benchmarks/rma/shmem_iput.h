/**
  @file shmem_iput.h
  @author Michael Beebe (Texas Tech University)
*/

#ifndef SHMEM_IPUT_H
#define SHMEM_IPUT_H

#include <shmem.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shmembench.h"

/**
  @brief Run the bandwidth benchmark for shmem_iput
  @param min_msg_size Minimum message size for test in bytes
  @param max_msg_size Maximum message size for test in bytes
  @param ntimes Number of repetitions to get the avgs from
  @param stride Stride for shmem_iput
 */
void bench_shmem_iput_bw(int min_msg_size, int max_msg_size, int ntimes,
                         int stride);

/**
  @brief Run the bidirectional bandwidth benchmark for shmem_iput
  @param min_msg_size Minimum message size for test in bytes
  @param max_msg_size Maximum message size for test in bytes
  @param ntimes Number of repetitions to get the avgs from
  @param stride Stride for shmem_iput
 */
void bench_shmem_iput_bibw(int min_msg_size, int max_msg_size, int ntimes,
                           int stride);

/**
  @brief Run the latency benchmark for shmem_iput
  @param min_msg_size Minimum message size for test in bytes
  @param max_msg_size Maximum message size for test in bytes
  @param ntimes Number of repetitions to get the avgs from
  @param stride Stride for shmem_iput
 */
void bench_shmem_iput_latency(int min_msg_size, int max_msg_size, int ntimes,
                              int stride);

#endif /* SHMEM_IPUT_H */
