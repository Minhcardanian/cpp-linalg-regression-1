cmake_minimum_required(VERSION 3.12)
project(cpp_linalg_regression VERSION 1.0 LANGUAGES CXX)

# --- Options --------------------------------------------------------------
option(BUILD_SHARED_LIBS "Build as shared libraries" OFF)
option(ENABLE_COVERAGE "Enable coverage reporting" OFF)

# --- C++ Standard --------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
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

# --- Tests ----------------------------------------------------------------
# Automatically discover and build all test_*.cpp files in tests/
file(GLOB TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_*.cpp")
foreach(test_src ${TEST_SOURCES})
  get_filename_component(test_name ${test_src} NAME_WE)
  add_executable(${test_name} ${test_src})
  target_link_libraries(${test_name} PRIVATE linalg Catch2::Catch2)
  add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()

# --- Installation ---------------------------------------------------------
install(TARGETS linalg RegressionDemo
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
)
install(DIRECTORY include/ DESTINATION include)

# --- Coverage Report ------------------------------------------------------
if(ENABLE_COVERAGE)
  find_program(LCOV_PATH lcov)
  find_program(GENHTML_PATH genhtml)
  if(LCOV_PATH AND GENHTML_PATH)
    add_custom_target(coverage
      COMMAND ${LCOV_PATH} --capture --directory . --output-file coverage.info
      COMMAND ${LCOV_PATH} --remove coverage.info '/usr/*' --output-file coverage.cleaned.info
      COMMAND ${GENHTML_PATH} coverage.cleaned.info --output-directory coverage-report
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
      COMMENT "Generating code coverage report"
      VERBATIM
    )
  endif()
endif()
