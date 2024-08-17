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
Usage:  oshrun -np <num PEs> shmembench --bench <routine> --benchtype <type> [options]

Required Options:
  --bench <routine>      Specify the routine to benchmark
                          Available options:
                            shmem_put
                            shmem_iput
                            shmem_get
                            shmem_iget
                            shmem_put_nbi
                            shmem_get_nbi
                            shmem_alltoall
                            shmem_alltoalls
                            shmem_broadcast
                            shmem_collect
                            shmem_fcollect
  --benchtype <type>     Set the benchmark type (bw, bibw, latency)

Optional Parameters:
  --min <size>           Set the minimum message size in bytes (default: 1)
  --max <size>           Set the maximum message size in bytes (default: 1024)
  --ntimes <count>       Number of iterations (default: 10)
  --stride <value>       Stride value for strided operations, only used by
                         the shmem_iput and shmem_iget (default: 10)
  --help                 Display this help message

Example Usage:
   oshrun -np 2 shmembench --bench shmem_put --benchtype bw --min 128 --max 1024 --ntimes 100
```

