cmake_minimum_required(VERSION 3.20)

project(markov_algos VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Catch2 CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

add_executable(markov_algos 
    src/main.cpp 
)

target_include_directories(markov_algos PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)


target_link_libraries(markov_algos PRIVATE 
    Catch2::Catch2
    Catch2::Catch2WithMain
    fmt::fmt
)

if(MSVC)
    # windows-specific flags
    target_compile_options(markov_algos PRIVATE /W4 /MP /O2 /permissive-)
else()
    # linux/WSL flags 
    target_compile_options(markov_algos PRIVATE -Wall -Wextra -O3 -march=native)
endif()
