#!/bin/bash

# --- Clean build
rm -rf build
mkdir build
cd build

# --- Configure build with SOS
cmake \
  -DCMAKE_LIBRARY_PATH=$SOS_LIB         \
  -DCMAKE_C_LINKER_FLAGS="-lpmi_simple -lsma" \
  -DCMAKE_INSTALL_PREFIX=$SWHOME/shmemvv \
  -DDEBUG=OFF \
  -DVERBOSE=ON \
  ../

# --- Compile
make -j 50

# --- Move back to project root
echo ; echo
cd ../

# --- Set path to shmembench binary
exe=build/bin/shmembench
if [ ! -f "$exe" ]; then
  echo "Executable $exe not found. Please build the project first."
  exit 1
fi

# --- Run shmembench with CPU pinning
if [ "$(which oshcc)" == "$HOME/sw/el9-x86_64/ompi/bin/oshcc" ]; then
  flags+=" --mca btl ^openib"
fi
flags="--bind-to core --map-by core"

# --- Set message size for testing
min=1
# max=1024
max=1048576

# --- Help message
oshrun $flags -np 1 $exe --help

# # --- shmem_put
# oshrun $flags -np 2 $exe --bench shmem_put --benchtype bw --min $min --max $max
# oshrun $flags -np 2 $exe --bench shmem_put --benchtype bibw --min $min --max $max
# oshrun $flags -np 2 $exe --bench shmem_put --benchtype latency --min $min --max $max

# --- shmem_iput
oshrun $flags -np 2 $exe --bench shmem_iput --benchtype bw --min $min --max $max
oshrun $flags -np 2 $exe --bench shmem_iput --benchtype bibw --min $min --max $max
oshrun $flags -np 2 $exe --bench shmem_iput --benchtype latency --min $min --max $max

# # --- shmem_get
# oshrun $flags -np 2 $exe --bench shmem_get --benchtype bw --min $min --max $max
# oshrun $flags -np 2 $exe --bench shmem_get --benchtype bibw --min $min --max $max
# oshrun $flags -np 2 $exe --bench shmem_get --benchtype latency --min $min --max $max

# --- shmem_iget
oshrun $flags -np 2 $exe --bench shmem_iget --benchtype bw --min $min --max $max
oshrun $flags -np 2 $exe --bench shmem_iget --benchtype bibw --min $min --max $max
oshrun $flags -np 2 $exe --bench shmem_iget --benchtype latency --min $min --max $max

# # --- shmem_put_nbi
# oshrun $flags -np 2 $exe --bench shmem_put_nbi --benchtype bw --min $min --max $max
# oshrun $flags -np 2 $exe --bench shmem_put_nbi --benchtype bibw --min $min --max $max
# oshrun $flags -np 2 $exe --bench shmem_put_nbi --benchtype latency --min $min --max $max

# # --- shmem_get_nbi
# oshrun $flags -np 2 $exe --bench shmem_get_nbi --benchtype bw --min $min --max $max
# oshrun $flags -np 2 $exe --bench shmem_get_nbi --benchtype bibw --min $min --max $max
# oshrun $flags -np 2 $exe --bench shmem_get_nbi --benchtype latency --min $min --max $max


