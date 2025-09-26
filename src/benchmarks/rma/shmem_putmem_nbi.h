/**
  @file shmem_putmem_nbi.h
  @author Michael Beebe (Texas Tech University)
*/

#ifndef SHMEM_PUTMEM_NBI_H
#define SHMEM_PUTMEM_NBI_H

#include <shmem.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shmembench.h"

/**
  @brief Run the bandwidth benchmark for shmem_putmem_nbi
  @param min_msg_size Minimum message size for test in bytes
  @param max_msg_size Maximum message size for test in bytes
  @param ntimes Number of repetitions to get the avgs from
 */
void bench_shmem_putmem_nbi_bw(int min_msg_size, int max_msg_size, int ntimes);

/**
  @brief Run the bidirectional bandwidth benchmark for shmem_putmem_nbi
  @param min_msg_size Minimum message size for test in bytes
  @param max_msg_size Maximum message size for test in bytes
  @param ntimes Number of repetitions to get the avgs from
 */
void bench_shmem_putmem_nbi_bibw(int min_msg_size, int max_msg_size, int ntimes);

#endif /* SHMEM_PUTMEM_NBI_H */
