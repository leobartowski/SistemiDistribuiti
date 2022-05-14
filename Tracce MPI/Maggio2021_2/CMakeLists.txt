cmake_minimum_required(VERSION 3.22)
project(Maggio2021_2 C)

set(CMAKE_C_STANDARD 99)

find_package(MPI REQUIRED)
include_directories(${MPI_INCLUDE_PATH})

add_executable(Maggio2021_2 main.c)
