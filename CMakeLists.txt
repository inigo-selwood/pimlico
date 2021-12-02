cmake_minimum_required(VERSION 3.14)
project(pimlico CXX)

set(CMAKE_CXX_STANDARD 11)

file(GLOB_RECURSE SOURCE_FILES source/*.cpp)
add_library(pimlico_library STATIC ${SOURCE_FILES})

add_executable(test_terms tests/grammar/term/test.cpp)
target_compile_options(test_terms PRIVATE "-g3")
set_target_properties(test_terms PROPERTIES CMAKE_BUILD_PARALLEL_LEVEL "4")
target_include_directories(test_terms PRIVATE "source/")
target_link_libraries(test_terms pimlico_library)
set_target_properties(test_terms PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/tests/grammar/term/)
