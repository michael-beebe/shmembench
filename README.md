![Logo](extra/logo.png)

Benchmarking suite for OpenSHMEM implementations adhering to the [v1.5 specification](http://www.openshmem.org/site/sites/default/site_files/OpenSHMEM-1.5.pdf)

## Building
```bash
$ mkdir build
$ cd build
$ cmake \
    -DCMAKE_LIBRARY_PATH=</path/to/openshmem/lib/dir>      \
    -DCMAKE_C_LINKER_FLAGS="<linker flags, such as -lsma>" \
    -DDEBUG=<OFF/ON>                                       \
    ../
```

## Running
```text
Usage: shmembench [options]

Options:
  --shmem_put             Enable shmem_put benchmark
  --shmem_p               Enable shmem_p benchmark
  --shmem_iput            Enable shmem_iput benchmark
  --shmem_get             Enable shmem_get benchmark
  --shmem_g               Enable shmem_g benchmark
  --shmem_iget            Enable shmem_iget benchmark
  --shmem_put_nbi         Enable shmem_put_nbi benchmark
  --shmem_get_nbi         Enable shmem_get_nbi benchmark
  --shmem_alltoall        Enable shmem_alltoall benchmark
  --shmem_alltoalls       Enable shmem_alltoalls benchmark
  --shmem_broadcast       Enable shmem_broadcast benchmark
  --shmem_collect         Enable shmem_collect benchmark
  --shmem_fcollect        Enable shmem_fcollect benchmark
  --shmem_min_reduce      Enable shmem_min_reduce benchmark
  --shmem_max_reduce      Enable shmem_max_reduce benchmark

  --benchtype <type>      Set the benchmark type (bw, bibw, latency)

  --min <size>            Set the minimum message size in bytes (default: 1)
  --max <size>            Set the maximum message size in bytes (default: 1024)

  --help                  Display this help message

Example Usage:
  shmembench --shmem_put --benchtype bw --min 128 --max 1024
```

