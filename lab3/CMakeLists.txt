cmake_minimum_required(VERSION 3.12)
project(MPI_Lab CXX)

find_package(MPI REQUIRED)

add_executable(mpi_program src/main.cpp)

target_link_libraries(mpi_program PUBLIC MPI::MPI_CXX)

if(WIN32)
    target_include_directories(mpi_program PUBLIC "C:/Program Files (x86)/Microsoft MPI/Include")
    target_link_directories(mpi_program PUBLIC "C:/Program Files (x86)/Microsoft MPI/Lib/x64")
endif()