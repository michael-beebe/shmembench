#!/bin/bash

cd ..

export CC=oshcc
export CXX=oshc++

make clean
make
