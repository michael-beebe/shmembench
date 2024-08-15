#####################################
# src/CMakeLists.txt
#####################################

# ---- Specify C++ standard
# set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_CXX_STANDARD_REQUIRED True)

# --- Specify C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# ---- Add include directories
include_directories(${CMAKE_SOURCE_DIR}/src/include)

# ---- Add library directories
link_directories(${CMAKE_LIBRARY_PATH})

# ---- Collect source files
set(SOURCES
  main.c
  shmembench.c
)

# ---- Add the main executable
add_executable(shmembench ${SOURCES})

# ---- Link the libraries with the additional linker flags
target_link_libraries(
  shmembench ${CMAKE_C_LINKER_FLAGS}

  # ============== Performance ==============
  # TODO: include performance libraries here

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
