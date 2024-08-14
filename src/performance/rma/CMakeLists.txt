#####################################
# src/performance/rma/CMakeLists.txt
#####################################
# --- Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# -- Add the source files tests
add_library(cxx_shmem_put cxx_shmem_put.cpp cxx_rma_tests.h)
add_library(cxx_shmem_p cxx_shmem_p.cpp cxx_rma_tests.h)

add_library(c11_shmem_put c11_shmem_put.c c11_rma_tests.h)
add_library(c11_shmem_p c11_shmem_p.c c11_rma_tests.h)

# -- Link the libraries with the additional linker flags
target_link_libraries(cxx_shmem_put ${CMAKE_C_LINKER_FLAGS})
target_link_libraries(cxx_shmem_p ${CMAKE_C_LINKER_FLAGS})

target_link_libraries(c11_shmem_put ${CMAKE_C_LINKER_FLAGS})
target_link_libraries(c11_shmem_p ${CMAKE_C_LINKER_FLAGS})
