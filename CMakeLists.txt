cmake_minimum_required(VERSION 3.12)
project(cpp_linalg_regression VERSION 1.0 LANGUAGES CXX)

# --- Options --------------------------------------------------------------
option(BUILD_SHARED_LIBS        "Build as shared libraries" OFF)
option(ENABLE_COVERAGE          "Enable coverage reporting" OFF)

# --- C++ Standard --------------------------------------------------------
set(CMAKE_CXX_STANDARD          17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Build Type -----------------------------------------------------------
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type (Debug or Release)" FORCE)
endif()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# --- Compiler Flags -------------------------------------------------------
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_compile_options(-Wall -Wextra -Werror)
  if(ENABLE_COVERAGE)
    message(STATUS "Enabling coverage flags")
    add_compile_options(--coverage)
    add_link_options(--coverage)
  endif()
endif()

# --- Dependencies & Testing ----------------------------------------------
include(CTest)
enable_testing()

# Fetch Catch2 for unit tests
include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v2.13.10
)
FetchContent_MakeAvailable(Catch2)

# --- Targets --------------------------------------------------------------
# Linear algebra library
add_library(linalg
  src/Vector.cpp
  src/Matrix.cpp
  src/LinearSystem.cpp
)
target_include_directories(linalg PUBLIC include)

# Regression demo executable
add_executable(RegressionDemo src/RegressionDemo.cpp)
target_link_libraries(RegressionDemo PRIVATE linalg)

# --- Tests ---------------------------------------------------------------
file(GLOB TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_*.cpp")
add_executable(unit_tests ${TEST_SOURCES})
target_link_libraries(unit_tests PRIVATE linalg Catch2::Catch2)
add_test(NAME unit_tests COMMAND unit_tests)

# ensure tests run with project root as cwd so data/machine.data is visible
set_tests_properties(unit_tests PROPERTIES
  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
)

# --- Installation ---------------------------------------------------------
install(TARGETS linalg RegressionDemo
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
)
install(DIRECTORY include/ DESTINATION include)
