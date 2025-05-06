cmake_minimum_required(VERSION 3.12)
project(cpp_linalg_regression VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Build types
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type" FORCE)
endif()

# Treat warnings as errors in Debug
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_compile_options(-Wall -Wextra -Werror)
endif()

# Enable testing
include(CTest)
enable_testing()

# Create library target
add_library(linalg
    src/Vector.cpp
    src/Matrix.cpp
    src/LinearSystem.cpp
)
target_include_directories(linalg PUBLIC include)

# Create regression demo executable
add_executable(RegressionDemo src/RegressionDemo.cpp)
target_link_libraries(RegressionDemo PRIVATE linalg)

# Install rules
install(TARGETS linalg RegressionDemo
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

# Add tests subdirectory
add_subdirectory(tests)
