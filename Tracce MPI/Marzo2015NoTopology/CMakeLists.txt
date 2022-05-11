cmake_minimum_required(VERSION 3.22)
project(Marzo2015NoTopology C)

set(CMAKE_C_STANDARD 99)

find_package(MPI REQUIRED)
include_directories(${MPI_INCLUDE_PATH})

add_executable(Marzo2015NoTopology main.c)
