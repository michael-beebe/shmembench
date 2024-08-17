#####################################
# src/benchmarks/collectives/CMakeLists.txt
#####################################
# -- Add the source files for collective tests
add_library(shmem_alltoall shmem_alltoall.c shmem_alltoall.h)
add_library(shmem_alltoalls shmem_alltoalls.c shmem_alltoalls.h)
add_library(shmem_broadcast shmem_broadcast.c shmem_broadcast.h)
add_library(shmem_collect shmem_collect.c shmem_collect.h)
add_library(shmem_fcollect shmem_fcollect.c shmem_fcollect.h)
add_library(shmem_barrier_all shmem_barrier_all.c shmem_barrier_all.h)

# -- Link the libraries with the additional linker flags
target_link_libraries(shmem_alltoall ${CMAKE_C_LINKER_FLAGS})
target_link_libraries(shmem_alltoalls ${CMAKE_C_LINKER_FLAGS})
target_link_libraries(shmem_broadcast ${CMAKE_C_LINKER_FLAGS})
target_link_libraries(shmem_collect ${CMAKE_C_LINKER_FLAGS})
target_link_libraries(shmem_fcollect ${CMAKE_C_LINKER_FLAGS})
target_link_libraries(shmem_barrier_all ${CMAKE_C_LINKER_FLAGS})
