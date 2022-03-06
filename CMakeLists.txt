cmake_minimum_required(VERSION 3.14)
project(pimlico CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3 -fprofile-arcs -ftest-coverage")
set(CMAKE_BUILD_PARALLEL_LEVEL "4")

file(GLOB COVERAGE_FILES CMakeFiles/*.cpp.gcda)
file(REMOVE "${COVERAGE_FILES}")

# Coverage reset target
add_custom_target(coverage_reset)
add_custom_command(TARGET coverage_reset
        COMMENT "Deleting existing .gcda files"
        COMMAND find CMakeFiles -name "*.cpp.gcda" -type f -delete
        DEPENDS pimlico_library)

# Pimlico library
file(GLOB_RECURSE LIBRARY_SOURCE_FILES source/*.cpp)
add_library(pimlico_library
        STATIC ${LIBRARY_SOURCE_FILES})
add_dependencies(pimlico_library coverage_reset)
target_include_directories(pimlico_library PRIVATE "source/")

# Library Tests
file(GLOB_RECURSE TEST_SOURCE_FILES test/*.cpp)
add_executable(test ${TEST_SOURCE_FILES})
target_link_libraries(test PRIVATE pimlico_library)
target_include_directories(test PRIVATE "source/")
target_include_directories(test PRIVATE "test/include")
set_target_properties(test
        PROPERTIES RUNTIME_OUTPUT_DIRECTORY test/binaries)
