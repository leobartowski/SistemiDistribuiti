cmake_minimum_required(VERSION 3.20)
project(EsercitazioneMPI___Pigreco C)

set(CMAKE_C_STANDARD 99)

find_package(MPI REQUIRED)
include_directories(${MPI_INCLUDE_PATH})

add_executable(EsercitazioneMPI___Pigreco main.c)
