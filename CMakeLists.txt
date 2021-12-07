cmake_minimum_required(VERSION 3.14)
project(pimlico CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_PARALLEL_LEVEL "4")

# Pimlico library
file(GLOB_RECURSE LIBRARY_SOURCE_FILES source/*.cpp)
add_library(pimlico_library STATIC ${LIBRARY_SOURCE_FILES})
target_include_directories(pimlico_library PRIVATE "source/")


# Buffer tests
file(GLOB_RECURSE BUFFER_TERM_SOURCE_FILES tests/buffer/*.cpp)
add_executable(test_buffer ${BUFFER_TERM_SOURCE_FILES})
target_link_libraries(test_buffer PRIVATE pimlico_library)
target_include_directories(test_buffer PRIVATE "source/")
target_include_directories(test_buffer PRIVATE "tests/include")
set_target_properties(test_buffer
        PROPERTIES RUNTIME_OUTPUT_DIRECTORY
    ${CMAKE_SOURCE_DIR}/tests/binaries)

# Term tests
file(GLOB_RECURSE TEST_TERM_SOURCE_FILES tests/grammar/term/*.cpp)
add_executable(test_term ${TEST_TERM_SOURCE_FILES})
target_link_libraries(test_term PRIVATE pimlico_library)
target_include_directories(test_term PRIVATE "source/")
target_include_directories(test_term PRIVATE "tests/include")
set_target_properties(test_term
        PROPERTIES RUNTIME_OUTPUT_DIRECTORY
        ${CMAKE_SOURCE_DIR}/tests/binaries)
