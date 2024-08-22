#####################################
# src/CMakeLists.txt
#####################################

# ---- Specify C++ standard
# set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_CXX_STANDARD_REQUIRED True)

# --- Specify C standard
# set(CMAKE_C_STANDARD 11)
# set(CMAKE_C_STANDARD_REQUIRED True)

# ---- Add include directories
include_directories(${CMAKE_SOURCE_DIR}/src/include)

# ---- Add library directories
link_directories(${CMAKE_LIBRARY_PATH})

# ---- Collect source files
set(SOURCES
  main.c
  shmembench.c
  parse_opts.c
)

# ---- Add the main executable
add_executable(shmembench ${SOURCES})

# ---- Link the libraries with the additional linker flags
target_link_libraries(
  shmembench ${CMAKE_C_LINKER_FLAGS}

  # ============== Benchmarks libs ==============
  # --- Pt2pt RMA
  shmem_put
  shmem_get
  shmem_iput
  shmem_iget
  shmem_put_nbi
  shmem_get_nbi

  # --- Collectives
  shmem_alltoall
  shmem_alltoalls
  shmem_broadcast
  shmem_collect
  shmem_fcollect
  shmem_barrier_all

  # --- Atomics
  shmem_atomic_add
  shmem_atomic_compare_swap
  shmem_atomic_fetch_nbi
  shmem_atomic_fetch
  shmem_atomic_inc
  shmem_atomic_set
)

# ---- Specify the output directory for the executable
set_target_properties(shmembench PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# ---- Add the performance directory
add_subdirectory(benchmarks)

# ---- Install the main executable
install(TARGETS shmembench
  RUNTIME DESTINATION bin
)
