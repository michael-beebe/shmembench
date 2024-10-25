![Logo](extra/logo.png)

Benchmarking suite for OpenSHMEM implementations adhering to either the [v1.4](http://www.openshmem.org/site/sites/default/site_files/OpenSHMEM-1.4.pdf) or [v1.5](http://www.openshmem.org/site/sites/default/site_files/OpenSHMEM-1.5.pdf) specification.

## Building
You can set your default C compiler to `oshcc` like so. This assumes you have `oshcc` in your PATH, shmem.h in your CPATH/C_INCLUDE_PATH, and OpenSHMEM libraries in your LD_LIBRARY_PATH:
```bash
$ mkdir build
$ cd build
$ export CC=`which oshcc`
$ cmake \
    -DUSE_<OpenSHMEM VERSION (either -DUSE_14 or -DUSE_15)>=ON \
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
                            shmem_barrier_all

                            shmem_atomic_add
                            shmem_atomic_compare_swap
                            shmem_atomic_fetch
                            shmem_atomic_fetch_nbi
                            shmem_atomic_inc
                            shmem_atomic_set
                            shmem_atomic_swap

  --benchtype <type>     Set the benchmark type (bw, bibw, latency)
                           Note: Pt2pt RMA benchmarks support bw, bibw and latency.
                           Note: Collectives benchmarks only support bw and latency.
                           Note: Atomic benchmarks only support latency.

Optional Parameters:
  --min <size>           Minimum message size in bytes (default: 1)
                            Note: Not applicable for atomic benchmarks.
                            Note: Not applicable for shmem_barrier_all benchmark.

  --max <size>           Maximum message size in bytes (default: 1048576)
                            Note: Not applicable for atomic benchmarks.
                            Note: Not applicable for shmem_barrier_all benchmark.

  --ntimes <count>       Number of repetitions.
                         Average among them is reported (default: 10)

  --stride <value>       Stride value for strided operations, only used by
                         the shmem_iput and shmem_iget (default: 10)

  --help                 Display this help message

Example Usage:
   oshrun -np 2 shmembench --bench shmem_put --benchtype bw --min 128 --max 1024 --ntimes 20
   oshrun -np 2 shmembench --bench shmem_iget --benchtype bw --min 128 --max 1024 --ntimes 20 --stride 20
   oshrun -np 64 shmembench --bench shmem_broadcast --benchtype bw --min 128 --max 1025 --ntimes 20
   oshrun -np 64 shmembench --bench shmem_barrier_all --benchtype latency --ntimes 100
   oshrun -np 6 shmembench --bench shmem_atomic_add --benchtype latency --ntimes 100
```

For runs with a large number of PEs, you may need to increase the size of `SHMEM_SYMMETRIC_SIZE` before running like so:
```bash
export SHMEM_SYMMETRIC_SIZE=1000000000    # v1.4
export SHMEM_SYMMETRIC_SIZE=1G            # v1.5
```

## Sample Output
```text

==============================================
===          Test Information              ===
==============================================
  OpenSHMEM Name:         Sandia OpenSHMEM
  OpenSHMEM Version:      1.5

  Number of PEs:          2
  Benchmark:              shmem_put
  Benchmark Type:         bw
  Min Msg Size (bytes):   1
  Max Msg Size (bytes):   1048576
  Ntimes:                 100

==============================================
===          Benchmark Results             ===
==============================================
Message Size     Avg Time (us)    Avg MB/s
1                21.18            0.36
2                21.20            0.72
4                18.86            1.62
8                18.99            3.21
16               19.01            6.42
32               19.14            12.76
64               21.63            22.57
128              31.48            31.02
256              27.33            71.46
512              28.12            138.91
1024             29.01            269.30
2048             30.27            516.15
4096             32.28            968.04
8192             38.54            1621.68
16384            49.32            2534.51
32768            69.31            3606.95
65536            122.52           4080.94
131072           276.71           3613.88
262144           558.86           3578.71
524288           1040.42          3844.61
1048576          1847.40          4330.41
```

## Contact
Michael Beebe: michael.beebe@ttu.edu
