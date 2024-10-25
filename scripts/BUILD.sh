#!/bin/bash

cd ..
rm -rf build
mkdir build
cd build


export CC=oshcc
export CXX=oshc++

cmake -DUSE_15=ON ../

make


