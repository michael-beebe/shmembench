#####################################
# src/benchmarks/atomics/CMakeLists.txt
#####################################
# -- Add the source files for collective tests
add_library(shmem_atomic_add shmem_atomic_add.c shmem_atomic_add.h)
add_library(shmem_atomic_compare_swap shmem_atomic_compare_swap.c shmem_atomic_compare_swap.h)
add_library(shmem_atomic_fetch_nbi shmem_atomic_fetch_nbi.c shmem_atomic_fetch_nbi.h)
add_library(shmem_atomic_fetch shmem_atomic_fetch.c shmem_atomic_fetch.h)
add_library(shmem_atomic_inc shmem_atomic_inc.c shmem_atomic_inc.h)
add_library(shmem_atomic_set shmem_atomic_set.c shmem_atomic_set.h)

# -- Link the libraries with the additional linker flags
target_link_libraries(shmem_atomic_add ${CMAKE_C_LINKER_FLAGS})
target_link_libraries(shmem_atomic_compare_swap ${CMAKE_C_LINKER_FLAGS})
target_link_libraries(shmem_atomic_fetch_nbi ${CMAKE_C_LINKER_FLAGS})
target_link_libraries(shmem_atomic_fetch ${CMAKE_C_LINKER_FLAGS})
target_link_libraries(shmem_atomic_inc ${CMAKE_C_LINKER_FLAGS})
target_link_libraries(shmem_atomic_set ${CMAKE_C_LINKER_FLAGS})

