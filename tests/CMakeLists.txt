cmake\_minimum\_required(VERSION 3.12)
project(cpp\_linalg\_regression VERSION 1.0 LANGUAGES CXX)

# --- Options --------------------------------------------------------------

option(BUILD\_SHARED\_LIBS "Build as shared libraries" OFF)
option(ENABLE\_COVERAGE "Enable coverage reporting" OFF)

# --- C++ Standard --------------------------------------------------------

set(CMAKE\_CXX\_STANDARD 17)
set(CMAKE\_CXX\_STANDARD\_REQUIRED ON)

# --- Build Type -----------------------------------------------------------

if(NOT CMAKE\_BUILD\_TYPE)
set(CMAKE\_BUILD\_TYPE "Debug" CACHE STRING "Build type (Debug or Release)" FORCE)
endif()
message(STATUS "Build type: \${CMAKE\_BUILD\_TYPE}")

# --- Compiler Flags -------------------------------------------------------

if(CMAKE\_BUILD\_TYPE STREQUAL "Debug")
add\_compile\_options(-Wall -Wextra -Werror)
if(ENABLE\_COVERAGE)
message(STATUS "Enabling coverage flags")
add\_compile\_options(--coverage)
add\_link\_options(--coverage)
endif()
endif()

# --- Dependencies & Testing ----------------------------------------------

include(CTest)
enable\_testing()

# Fetch Catch2 for unit tests

include(FetchContent)
FetchContent\_Declare(
Catch2
GIT\_REPOSITORY [https://github.com/catchorg/Catch2.git](https://github.com/catchorg/Catch2.git)
GIT\_TAG        v2.13.10
)
FetchContent\_MakeAvailable(Catch2)

# --- Targets --------------------------------------------------------------

# Linear algebra library

add\_library(linalg
src/Vector.cpp
src/Matrix.cpp
src/LinearSystem.cpp
)
target\_include\_directories(linalg PUBLIC include)

# Regression demo executable

add\_executable(RegressionDemo src/RegressionDemo.cpp)
target\_link\_libraries(RegressionDemo PRIVATE linalg)

# --- Tests ----------------------------------------------------------------

# Automatically discover and build all test\_\*.cpp files in tests/

file(GLOB TEST\_SOURCES "\${CMAKE\_CURRENT\_SOURCE\_DIR}/tests/test\_\*.cpp")
foreach(test\_src \${TEST\_SOURCES})
get\_filename\_component(test\_name \${test\_src} NAME\_WE)
add\_executable(\${test\_name} \${test\_src})
target\_link\_libraries(\${test\_name} PRIVATE linalg Catch2::Catch2)
add\_test(NAME \${test\_name} COMMAND \${test\_name})
endforeach()

# --- Installation ---------------------------------------------------------

install(TARGETS linalg RegressionDemo
ARCHIVE DESTINATION lib
RUNTIME DESTINATION bin
)
install(DIRECTORY include/ DESTINATION include)

# --- Coverage Report ------------------------------------------------------

if(ENABLE\_COVERAGE)
find\_program(LCOV\_PATH lcov)
find\_program(GENHTML\_PATH genhtml)
if(LCOV\_PATH AND GENHTML\_PATH)
add\_custom\_target(coverage
COMMAND \${LCOV\_PATH} --capture --directory . --output-file coverage.info
COMMAND \${LCOV\_PATH} --remove coverage.info '/usr/\*' --output-file coverage.cleaned.info
COMMAND \${GENHTML\_PATH} coverage.cleaned.info --output-directory coverage-report
WORKING\_DIRECTORY \${CMAKE\_BINARY\_DIR}
COMMENT "Generating code coverage report"
VERBATIM
)
endif()
endif()

# --- Custom Run Targets ---------------------------------------------------

# Prepare logs directory\add\_custom\_target(prep\_logs

COMMAND \${CMAKE\_COMMAND} -E make\_directory \${CMAKE\_BINARY\_DIR}/logs
COMMENT "Preparing logs directory"
)

# Run all tests and capture output to logs/tests.log

add\_custom\_target(run\_tests
COMMAND \${CMAKE\_CTEST\_COMMAND} --output-on-failure > \${CMAKE\_BINARY\_DIR}/logs/tests.log 2>&1
WORKING\_DIRECTORY \${CMAKE\_BINARY\_DIR}
COMMENT "Running tests and capturing logs"
)
add\_dependencies(run\_tests prep\_logs)

# Run regression demo and capture output to logs/regression.log

add\_custom\_target(run\_demo
COMMAND RegressionDemo --data \${CMAKE\_SOURCE\_DIR}/data/machine.data --train-split 0.8 --seed 42 > \${CMAKE\_BINARY\_DIR}/logs/regression.log 2>&1
DEPENDS RegressionDemo
WORKING\_DIRECTORY \${CMAKE\_BINARY\_DIR}
COMMENT "Running regression demo and capturing logs"
)
add\_dependencies(run\_demo prep\_logs)

# Combined target to execute tests and demo

add\_custom\_target(run\_all
DEPENDS run\_tests run\_demo
COMMENT "Run full project: tests + regression demo"
)
