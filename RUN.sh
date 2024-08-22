#!/bin/bash

# --- Set path to shmembench binary
exe=build/bin/shmembench
if [ ! -f "$exe" ]; then
  echo "Executable $exe not found. Please build the project first."
  exit 1
fi

# --- Set flags
if [ "$(which oshcc)" == "$HOME/sw/el9-x86_64/ompi/bin/oshcc" ]; then
  flags+=" --mca btl ^openib"
fi
flags="--bind-to core --map-by core"

# --- Set message size for testing
min=1
max=1024
# max=1048576

# --- Number of repetitions
ntimes=100

# --- Set SHMEM_SYMMETRIC_SIZE
# export SHMEM_SYMMETRIC_SIZE=2G
export SHMEM_SYMMETRIC_SIZE=10000000000

#=====================================================================================
# # --- Help message
# oshrun $flags -np 1 $exe --help

# # --- shmem_put
# oshrun $flags -np 2 $exe --bench shmem_put --benchtype bw --min $min --max $max --ntimes $ntimes
# oshrun $flags -np 2 $exe --bench shmem_put --benchtype bibw --min $min --max $max --ntimes $ntimes
# oshrun $flags -np 2 $exe --bench shmem_put --benchtype latency --min $min --max $max --ntimes $ntimes

# # --- shmem_get
# oshrun $flags -np 2 $exe --bench shmem_get --benchtype bw --min $min --max $max --ntimes $ntimes
# oshrun $flags -np 2 $exe --bench shmem_get --benchtype bibw --min $min --max $max --ntimes $ntimes
# oshrun $flags -np 2 $exe --bench shmem_get --benchtype latency --min $min --max $max --ntimes $ntimes

# # --- shmem_iput
# oshrun $flags -np 2 $exe --bench shmem_iput --benchtype bw --min $min --max $max --ntimes $ntimes
# oshrun $flags -np 2 $exe --bench shmem_iput --benchtype bibw --min $min --max $max --ntimes $ntimes
# oshrun $flags -np 2 $exe --bench shmem_iput --benchtype latency --min $min --max $max --ntimes $ntimes

# # --- shmem_iget
# oshrun $flags -np 2 $exe --bench shmem_iget --benchtype bw --min $min --max $max --ntimes $ntimes
# oshrun $flags -np 2 $exe --bench shmem_iget --benchtype bibw --min $min --max $max --ntimes $ntimes
# oshrun $flags -np 2 $exe --bench shmem_iget --benchtype latency --min $min --max $max --ntimes $ntimes

# # --- shmem_put_nbi
# oshrun $flags -np 2 $exe --bench shmem_put_nbi --benchtype bw --min $min --max $max --ntimes $ntimes
# oshrun $flags -np 2 $exe --bench shmem_put_nbi --benchtype bibw --min $min --max $max --ntimes $ntimes
# oshrun $flags -np 2 $exe --bench shmem_put_nbi --benchtype latency --min $min --max $max --ntimes $ntimes

# # --- shmem_get_nbi
# oshrun $flags -np 2 $exe --bench shmem_get_nbi --benchtype bw --min $min --max $max --ntimes $ntimes
# oshrun $flags -np 2 $exe --bench shmem_get_nbi --benchtype bibw --min $min --max $max --ntimes $ntimes
# oshrun $flags -np 2 $exe --bench shmem_get_nbi --benchtype latency --min $min --max $max --ntimes $ntimes

#=====================================================================================

# # --- shmem_alltoall
# oshrun $flags -np 6 $exe --bench shmem_alltoall --benchtype bw --min $min --max $max --ntimes $ntimes
# oshrun $flags -np 6 $exe --bench shmem_alltoall --benchtype latency --min $min --max $max --ntimes $ntimes

# # --- shmem_alltoalls
# oshrun $flags -np 6 $exe --bench shmem_alltoalls --benchtype bw --min $min --max $max --ntimes $ntimes
# oshrun $flags -np 6 $exe --bench shmem_alltoalls --benchtype latency --min $min --max $max --ntimes $ntimes

# # --- shmem_broadcast
# oshrun $flags -np 6 $exe --bench shmem_broadcast --benchtype bw --min $min --max $max --ntimes $ntimes
# oshrun $flags -np 6 $exe --bench shmem_broadcast --benchtype latency --min $min --max $max --ntimes $ntimes

# # --- shmem_collect
# oshrun $flags -np 6 $exe --bench shmem_collect --benchtype bw --min $min --max $max --ntimes $ntimes
# oshrun $flags -np 6 $exe --bench shmem_collect --benchtype latency --min $min --max $max --ntimes $ntimes

# # --- shmem_fcollect
# oshrun $flags -np 6 $exe --bench shmem_fcollect --benchtype bw --min $min --max $max --ntimes $ntimes
# oshrun $flags -np 6 $exe --bench shmem_fcollect --benchtype latency --min $min --max $max --ntimes $ntimes

# # --- shmem_barrier_all
# oshrun $flags -np 100 $exe --bench shmem_barrier_all --benchtype latency --ntimes 10

#=====================================================================================

# --- Atomic Operations Latency Tests
# oshrun $flags -np 2 $exe --bench shmem_atomic_add --benchtype latency --ntimes $ntimes
# oshrun $flags -np 2 $exe --bench shmem_atomic_compare_swap --benchtype latency --ntimes $ntimes
# oshrun $flags -np 2 $exe --bench shmem_atomic_fetch --benchtype latency --ntimes $ntimes
# oshrun $flags -np 2 $exe --bench shmem_atomic_fetch_nbi --benchtype latency --ntimes $ntimes
# oshrun $flags -np 2 $exe --bench shmem_atomic_inc --benchtype latency --ntimes $ntimes
oshrun $flags -np 2 $exe --bench shmem_atomic_set --benchtype latency --ntimes $ntimes
#oshrun $flags -np 2 $exe --bench shmem_atomic_swap --benchtype latency --ntimes $ntimes


