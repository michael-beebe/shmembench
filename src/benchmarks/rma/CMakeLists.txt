#####################################
# src/benchmarks/rma/CMakeLists.txt
#####################################
# --- Set C standard
# set(CMAKE_C_STANDARD 11)
# set(CMAKE_C_STANDARD_REQUIRED True)

# # -- Add the source files tests
add_library(shmem_put shmem_put.c shmem_put.h)

# # -- Link the libraries with the additional linker flags
target_link_libraries(shmem_put ${CMAKE_C_LINKER_FLAGS})