cmake_minimum_required(VERSION 3.10)
project(CircularBuffer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
endif()

add_executable(circular_buffer_test
    src/main.cpp
)

target_include_directories(circular_buffer_test PRIVATE include)
