cmake_minimum_required(VERSION 3.10)
project(ParallelProcessor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
endif()

find_package(Threads REQUIRED)

add_executable(parallel_processor_test
    src/main.cpp
)

target_include_directories(parallel_processor_test PRIVATE include)
target_link_libraries(parallel_processor_test PRIVATE Threads::Threads)
