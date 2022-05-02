cmake_minimum_required(VERSION 3.22)
project(Febbraio2019 C)

set(CMAKE_C_STANDARD 99)

find_package(MPI REQUIRED)
include_directories(${MPI_INCLUDE_PATH})

add_executable(Febbraio2019 main.c)
