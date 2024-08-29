#!/bin/bash

# --- Clean build
rm -rf build
mkdir build
cd build


export CC=`which oshcc`
export CXX=`which oshc++`

# --- Configure build with SOS
cmake \
  -DCMAKE_LIBRARY_PATH=$OSSS_LIB         \
  -DCMAKE_INSTALL_PREFIX=$SWHOME/shmemvv \
  -DDEBUG=OFF \
  -DUSE_14=ON \
  ../

# --- Compile
make -j 50

# --- Run
# cd ../
# ./RUN.sh
