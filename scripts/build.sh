#!/bin/bash
make clean
export CC=$OSSS_TESTING_BIN/oshcc
make CC=$CC  USE_SHMEM_VERSION=15


