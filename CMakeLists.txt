#####################################
# CMakeLists.txt
#####################################

# ---- Specify the minimum version for CMake
cmake_minimum_required(VERSION 3.10)

# ---- Project name and version
project(shmembench VERSION 0.1)

# ---- Enable Testing 
enable_testing()

# ---- Add options
option(USE_14 "Enable OpenSHMEM v1.4" OFF)
option(USE_15 "Enable OpenSHMEM v1.5" OFF)
option(DEBUG "Enable debugging output" OFF)

# ---- Ensure mutual exclusivity of USE_14 and USE_15
if (USE_14 AND USE_15)
  message(FATAL_ERROR "Cannot enable both USE_14 and USE_15. Please choose one.")
endif()

# ---- Ensure at least one version is selected
if (NOT USE_14 AND NOT USE_15)
  message(FATAL_ERROR "You must specify either USE_14 or USE_15.")
endif()

# ---- Add definitions
if (USE_14)
  add_definitions(-DUSE_14)
endif()

if (USE_15)
  add_definitions(-DUSE_15)
endif()

if (DEBUG)
  add_definitions(-D_DEBUG_)
  set(CMAKE_BUILD_TYPE Debug)
endif()

# ---- Add include directory
include_directories(${CMAKE_SOURCE_DIR}/src/include)

# ---- Add subdirectory for src
add_subdirectory(src)
