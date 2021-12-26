cmake_minimum_required(VERSION 3.14)
project(pimlico CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3 -fprofile-arcs -ftest-coverage")
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

# Make sure gcov, lcov, genhtml and c++filt are present
find_program(GCOV_PATH gcov)
find_program(LCOV_PATH lcov)
find_program(GENHTML_PATH genhtml)
find_program(CPPFILT_PATH c++filt)
if(NOT GCOV_PATH OR NOT LCOV_PATH OR NOT GENHTML_PATH OR NOT CPPFILT_PATH)
    message(FATAL_ERROR "gcov, lcov, genhtml, or c++filt not found")
endif()

# Setting up commands which will be run to generate coverage data.
# Cleanup lcov
set(RESET
    lcov
        --directory .
        --base-directory ${CMAKE_SOURCE_DIR}
    --zerocounters > /dev/null)

# Create baseline to make sure untouched files show up in the report
set(GET_BASELINE
    lcov
        --capture
        --initial
        --directory .
        --base-directory ${CMAKE_SOURCE_DIR}
        --output-file coverage/coverage.base > /dev/null)

# Run tests
set(RUN_TEST
    ${CMAKE_SOURCE_DIR}/test/binaries/test > /dev/null || true
)

# Capturing lcov counters and generating report
set(CAPTURE_TEST
    lcov
        --directory .
        --base-directory ${CMAKE_SOURCE_DIR}
        --capture
        --output-file coverage/coverage.capture > /dev/null)

# add baseline counters
set(COMBINE_BASELINE
    lcov
        --add-tracefile coverage/coverage.base
        --add-tracefile coverage/coverage.capture
        --output-file coverage/coverage.total > /dev/null)

# filter collected data to final coverage report
set(FILTER_RESULTS
    lcov
        --remove coverage/coverage.total
        "${CMAKE_SOURCE_DIR}/test*" "/usr*" "/Library*"
        --output-file coverage/coverage.info > /dev/null)

# Generate HTML output
set(GENERATE_HTML
    genhtml
        --demangle-cpp
        -o coverage/coverage coverage/coverage.info > /dev/null)

# Create coverage target
add_custom_target(coverage
    COMMAND mkdir -p coverage
    COMMAND ${RESET}
    COMMAND ${GET_BASELINE}
    COMMAND ${RUN_TEST}
    COMMAND ${CAPTURE_TEST}
    COMMAND ${COMBINE_BASELINE}
    COMMAND ${FILTER_RESULTS}
    COMMAND ${GENERATE_HTML}

    BYPRODUCTS coverage/
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    DEPENDS pimlico_library test
)
