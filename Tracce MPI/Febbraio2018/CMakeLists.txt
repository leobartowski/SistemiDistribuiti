cmake_minimum_required(VERSION 3.21)
project(Febbraio2018 C)

set(CMAKE_C_STANDARD 99)

find_package(MPI REQUIRED)
include_directories(${MPI_INCLUDE_PATH})

add_executable(Febbraio2018 main.c)
