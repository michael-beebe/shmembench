# SHMEMBENCH
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