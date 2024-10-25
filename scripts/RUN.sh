#!/bin/bash

cd ../
exe="build/bin/shmembench"

export OMP_NUM_THREADS=4

# --- shmem_put ---
oshrun -np 2 $exe --bench shmem_put --benchtype bw --min 1 --max 1048576 --ntimes 10
oshrun -np 2 $exe --bench shmem_put --benchtype bibw --min 1 --max 1048576 --ntimes 10
oshrun -np 2 $exe --bench shmem_put --benchtype latency --min 1 --max 1048576 --ntimes 10

# --- shmem_iget ---
oshrun -np 2 $exe --bench shmem_iget --benchtype bw --stride 2 --min 1 --max 1024 --ntimes 1
oshrun -np 2 $exe --bench shmem_iget --benchtype bibw --stride 2 --min 1 --max 1024 --ntimes 1
oshrun -np 2 $exe --bench shmem_iget --benchtype latency --stride 2 --min 1 --max 1024 --ntimes 1

# --- shmem_put_nbi ---
oshrun -np 2 $exe --bench shmem_put_nbi --benchtype bw --min 1 --max 1048576 --ntimes 10
oshrun -np 2 $exe --bench shmem_put_nbi --benchtype bibw --min 1 --max 1048576 --ntimes 10
oshrun -np 2 $exe --bench shmem_put_nbi --benchtype latency --min 1 --max 1048576 --ntimes 10

# --- shmem_get_nbi ---
oshrun -np 2 $exe --bench shmem_get_nbi --benchtype bw --min 1 --max 1048576 --ntimes 10
oshrun -np 2 $exe --bench shmem_get_nbi --benchtype bibw --min 1 --max 1048576 --ntimes 10
oshrun -np 2 $exe --bench shmem_get_nbi --benchtype latency --min 1 --max 1048576 --ntimes 10

# --- shmem_alltoall ---
oshrun -np 2 $exe --bench shmem_alltoall --benchtype bw --min 1 --max 1048576 --ntimes 10
oshrun -np 2 $exe --bench shmem_alltoall --benchtype latency --min 1 --max 1048576 --ntimes 10

# --- shmem_alltoalls ---
oshrun -np 2 $exe --bench shmem_alltoalls --benchtype bw --min 1 --max 1048576 --ntimes 10
oshrun -np 2 $exe --bench shmem_alltoalls --benchtype latency --min 1 --max 1048576 --ntimes 10

# --- shmem_broadcast ---
oshrun -np 2 $exe --bench shmem_broadcast --benchtype bw --min 1 --max 1048576 --ntimes 10
oshrun -np 2 $exe --bench shmem_broadcast --benchtype latency --min 1 --max 1048576 --ntimes 10

# --- shmem_collect ---
oshrun -np 2 $exe --bench shmem_collect --benchtype bw --min 1 --max 1048576 --ntimes 10
oshrun -np 2 $exe --bench shmem_collect --benchtype latency --min 1 --max 1048576 --ntimes 10

# --- shmem_fcollect ---
oshrun -np 2 $exe --bench shmem_fcollect --benchtype bw --min 1 --max 1048576 --ntimes 10
oshrun -np 2 $exe --bench shmem_fcollect --benchtype latency --min 1 --max 1048576 --ntimes 10

# --- shmem_atomic_add ---
oshrun -np 2 $exe --bench shmem_atomic_add --benchtype latency --min 1 --max 1048576 --ntimes 10

# --- shmem_atomic_compare_swap ---
oshrun -np 2 $exe --bench shmem_atomic_compare_swap --benchtype latency --min 1 --max 1048576 --ntimes 10

# --- shmem_atomic_fetch ---
oshrun -np 2 $exe --bench shmem_atomic_fetch --benchtype latency --min 1 --max 1048576 --ntimes 10

# --- shmem_atomic_inc ---
oshrun -np 2 $exe --bench shmem_atomic_inc --benchtype latency --min 1 --max 1048576 --ntimes 10

# --- shmem_atomic_set ---
oshrun -np 2 $exe --bench shmem_atomic_set --benchtype latency --min 1 --max 1048576 --ntimes 10

# --- shmem_atomic_swap ---
oshrun -np 2 $exe --bench shmem_atomic_swap --benchtype latency --min 1 --max 1048576 --ntimes 10

# --- shmem_barrier_all ---
oshrun -np 2 $exe --bench shmem_barrier_all --benchtype latency --min 1 --max 1048576 --ntimes 10
