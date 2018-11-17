#include "stdafx.h"
#include <iostream>
#include <mpi.h>


int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	double* buf;
	int bufsize;
	double outval;
	int N;
	if (rank == 0) {
                std::cin >> bufsize;
		buf = new double[1 + bufsize];
		for (int i = 1; i <= bufsize; i++) {
                        std::cin >> buf[i];
			buf[0] = 0;
		}
		N = 1;
	} else {
                std::cin >> N;
	}
	MPI_Bcast(&bufsize, 1, MPI_INT, 0, MPI_COMM_WORLD);
	if (rank != 0) {
		buf = new double[bufsize];
	}
	MPI_Comm comm;
	MPI_Comm_split(MPI_COMM_WORLD, N, rank, &comm);
	if (N == 0) {
		return;
	}

	int comm_size;
	MPI_Comm_size(comm, &comm_size);
	int* counts = new int[comm_size];
	int* stride = new int[comm_size];
	for (int i = 0; i < comm_size; i++) {
		counts[i] = 1;
		stride[i] = i;
	}

	MPI_Scatterv(buf, counts, stride, MPI_DOUBLE, &outval, 1, MPI_DOUBLE, 0, comm);

	if (rank == 0) {
		delete buf;
	}
	else {
                std::cout << outval;
	}
        MPI_Finalize();
        return 0;
    }

