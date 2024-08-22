#!/bin/bash

exe=version
rm version

oshcc shmem_get_version.c -o version -L$SOS_LIB -lsma -lpmi_simple 

