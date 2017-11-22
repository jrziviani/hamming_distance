cmake_minimum_required (VERSION 2.8)
project (waes_hamming)

set (waes_hamming_VERSION_MAJOR 0)
set (waes_hamming_VERSION_MINOR 1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT UNIX)
    message(FATAL_ERROR "Tested in Linux only")
endif()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(lib)
add_subdirectory(client)
