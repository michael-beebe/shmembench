#!/bin/bash

# --- Clean build
rm -rf build
mkdir build
cd build


export CC=`which oshcc`
# --- Configure build with SOS
cmake \
  -DUSE_14=ON \
  ../

# --- Compile
make -j 50

# --- Run
# cd ../
# ./RUN.sh
