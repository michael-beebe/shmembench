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

min=1
max=1024
# max=1048576

oshrun $flags -np 1 $exe --help

# oshrun $flags -np 2 $exe --shmem_put --benchtype bw --min $min --max $max
# oshrun $flags -np 2 $exe --shmem_put --benchtype bibw --min $min --max $max
# oshrun $flags -np 2 $exe --shmem_put --benchtype latency --min $min --max $max
