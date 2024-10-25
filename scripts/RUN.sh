#!/bin/bash

cd ../
exe="build/bin/shmembench"

oshrun -np 2 $exe --bench shmem_put --benchtype bw --min 1 --max 1024 --ntimes 10


