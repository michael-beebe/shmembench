/**
  @file shmem_getmem.h
  @author Michael Beebe (Texas Tech University)
*/

#ifndef _SHMEM_GETMEM_H_
#define _SHMEM_GETMEM_H_

#include "shmembench.h"

/**
  @brief Run the bandwidth benchmark for shmem_getmem
  @param min_msg_size Minimum message size for test in bytes
  @param max_msg_size Maximum message size for test in bytes
  @param ntimes Number of repetitions to get the avgs from
 */
void bench_shmem_getmem_bw(int min_msg_size, int max_msg_size, int ntimes);

/**
  @brief Run the bidirectional bandwidth benchmark for shmem_getmem
  @param min_msg_size Minimum message size for test in bytes
  @param max_msg_size Maximum message size for test in bytes
  @param ntimes Number of repetitions to get the avgs from
 */
void bench_shmem_getmem_bibw(int min_msg_size, int max_msg_size, int ntimes);

#endif /* _SHMEM_GETMEM_H_ */
