cmake_minimum_required(VERSION 3.10)
project(LogFileManager VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/W4 /WX)
endif()

add_executable(log_file_manager_test
    src/LogFileManager.cpp
    src/main.cpp
)

target_include_directories(log_file_manager_test PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Create logs directory in build folder after build
add_custom_command(TARGET log_file_manager_test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/logs"
    COMMENT "Creating logs directory"
)
