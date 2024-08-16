#####################################
# src/benchmarks/rma/CMakeLists.txt
#####################################
# --- Set C standard
# set(CMAKE_C_STANDARD 11)
# set(CMAKE_C_STANDARD_REQUIRED True)

# # -- Add the source files tests
add_library(shmem_put shmem_put.c shmem_put.h)
add_library(shmem_get shmem_get.c shmem_get.h)

add_library(shmem_iput shmem_iput.c shmem_iput.h)
add_library(shmem_iget shmem_iget.c shmem_iget.h)

add_library(shmem_put_nbi shmem_put_nbi.c shmem_put_nbi.h)
add_library(shmem_get_nbi shmem_get_nbi.c shmem_get_nbi.h)

# # -- Link the libraries with the additional linker flags
target_link_libraries(shmem_put ${CMAKE_C_LINKER_FLAGS})
target_link_libraries(shmem_get ${CMAKE_C_LINKER_FLAGS})

target_link_libraries(shmem_iput ${CMAKE_C_LINKER_FLAGS})
target_link_libraries(shmem_iget ${CMAKE_C_LINKER_FLAGS})

target_link_libraries(shmem_put_nbi ${CMAKE_C_LINKER_FLAGS})
target_link_libraries(shmem_get_nbi ${CMAKE_C_LINKER_FLAGS})

