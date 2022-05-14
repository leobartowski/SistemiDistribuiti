cmake_minimum_required(VERSION 3.22)
project(Giugno2016v2 C)

set(CMAKE_C_STANDARD 99)

find_package(MPI REQUIRED)
include_directories(${MPI_INCLUDE_PATH})

add_executable(Giugno2016v2 main.c)
