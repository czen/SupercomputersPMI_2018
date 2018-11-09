#!/bin/sh

# usage: run_mpi_program <name>
# for file name.cpp
mpicc -o $1 "$1.cpp"
mpirun -np 4 ./$1