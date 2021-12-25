cmake_minimum_required(VERSION 3.14)
project(pimlico CXX)

# Link CMake gcov module
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMakeModules)
include(coverage)
append_coverage_compiler_flags()
set(COVERAGE_EXCLUDES
        "${CMAKE_SOURCE_DIR}/test*"
        "/usr*"
        "/Library*")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3")
set(CMAKE_BUILD_PARALLEL_LEVEL "4")

# Pimlico library
file(GLOB_RECURSE LIBRARY_SOURCE_FILES source/*.cpp)
add_library(pimlico_library STATIC ${LIBRARY_SOURCE_FILES})
target_include_directories(pimlico_library PRIVATE "source/")

# Tests
file(GLOB_RECURSE TEST_SOURCE_FILES test/*.cpp)
add_executable(test ${TEST_SOURCE_FILES})
target_link_libraries(test PRIVATE pimlico_library)
target_include_directories(test PRIVATE "source/")
target_include_directories(test PRIVATE "test/include")
set_target_properties(test
        PROPERTIES RUNTIME_OUTPUT_DIRECTORY
        ${CMAKE_SOURCE_DIR}/test/binaries)

APPEND_COVERAGE_COMPILER_FLAGS()
SETUP_TARGET_FOR_COVERAGE_LCOV(
        NAME coverage
        EXECUTABLE test/binaries/test || true
        DEPENDENCIES
            pimlico_library
            test)
