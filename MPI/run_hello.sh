#!/bin/sh

mpicc -o hello hello.c
mpirun -np 4 ./hello