cmake_minimum_required(VERSION 3.14)
project(pimlico CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_PARALLEL_LEVEL "4")

# Library
file(GLOB_RECURSE SOURCE_FILES source/*.cpp)
add_library(pimlico_library STATIC ${SOURCE_FILES})

# # Buffer test
# add_executable(test_buffer tests/buffer/buffer.cpp)
# target_link_libraries(test_buffer pimlico_library)
# target_include_directories(test_buffer PRIVATE "source/")
# target_include_directories(test_buffer PRIVATE "tests/include")
# set_target_properties(test_buffer PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/tests/buffer/)

# # Grammar test
# add_executable(test_terms tests/grammar/term/test.cpp)
# target_link_libraries(test_terms pimlico_library)
# target_compile_options(test_terms PRIVATE "-g3")
# target_include_directories(test_terms PRIVATE "source/")
# set_target_properties(test_terms PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/tests/grammar/term/)
