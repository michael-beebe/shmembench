#!/bin/bash

echo "Currently in $(pwd)"
cd ..
echo "Currently in $(pwd)"
rm -rf build
mkdir build
cd build
echo "Currently in $(pwd)"


export CC=oshcc
export CXX=oshc++

cmake -DUSE_15=ON ../

make


