#####################################
# src/performance/rma/CMakeLists.txt
#####################################
# --- Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# # -- Add the source files tests
# add_library(cxx_shmem_put cxx_shmem_put.cpp cxx_rma_tests.h)

# # -- Link the libraries with the additional linker flags
# target_link_libraries(cxx_shmem_put ${CMAKE_C_LINKER_FLAGS})