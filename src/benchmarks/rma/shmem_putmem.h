/**
  @file shmem_putmem.h
  @author Michael Beebe (Texas Tech University)
*/

#ifndef SHMEM_PUTMEM_H
#define SHMEM_PUTMEM_H

#include <shmem.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shmembench.h"

/**
  @brief Run the bandwidth benchmark for shmem_putmem
  @param min_msg_size Minimum message size for test in bytes
  @param max_msg_size Maximum message size for test in bytes
 */
void bench_shmem_putmem_bw(int min_msg_size, int max_msg_size, int ntimes);

/**
  @brief Run the bidirectional bandwidth benchmark for shmem_putmem
  @param min_msg_size Minimum message size for test in bytes
  @param max_msg_size Maximum message size for test in bytes
 */
void bench_shmem_putmem_bibw(int min_msg_size, int max_msg_size, int ntimes);

#endif /* SHMEM_PUTMEM_H */
