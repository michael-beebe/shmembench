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
