/**
  @file benchmarks.h
  @author Michael Beebe (Texas Tech University)
  @brief
*/

#ifndef BENCHMARK_H
#define BENCHMARK_H

/* RMA benchmarks */
#include "../benchmarks/rma/shmem_get.h"
#include "../benchmarks/rma/shmem_getmem.h"
#include "../benchmarks/rma/shmem_get_nbi.h"
#include "../benchmarks/rma/shmem_getmem_nbi.h"
#include "../benchmarks/rma/shmem_iget.h"
#include "../benchmarks/rma/shmem_iput.h"
#include "../benchmarks/rma/shmem_put.h"
#include "../benchmarks/rma/shmem_putmem.h"
#include "../benchmarks/rma/shmem_put_nbi.h"
#include "../benchmarks/rma/shmem_putmem_nbi.h"

/* Collective benchmarks */
#include "../benchmarks/collectives/shmem_alltoall.h"
#include "../benchmarks/collectives/shmem_alltoalls.h"
#include "../benchmarks/collectives/shmem_broadcast.h"
#include "../benchmarks/collectives/shmem_collect.h"
#include "../benchmarks/collectives/shmem_fcollect.h"
#include "../benchmarks/collectives/shmem_barrier_all.h"

/* Atomic benchmarks */
#include "../benchmarks/atomics/shmem_atomic_add.h"
#include "../benchmarks/atomics/shmem_atomic_compare_swap.h"
#include "../benchmarks/atomics/shmem_atomic_fetch_nbi.h"
#include "../benchmarks/atomics/shmem_atomic_fetch.h"
#include "../benchmarks/atomics/shmem_atomic_inc.h"
#include "../benchmarks/atomics/shmem_atomic_set.h"
#include "../benchmarks/atomics/shmem_atomic_swap.h"

#endif /* BENCHMARK_H */
