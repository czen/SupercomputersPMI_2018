#include "stdafx.h"
#include <iostream>
#include <mpi.h>


int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int N;
	double x;
        std::cin >> N;
	if (N != 0) {
                std::cin >> x;
	}

	MPI_Comm comm;
	MPI_Comm_split(MPI_COMM_WORLD, N, rank, &comm);
	if (N == 0) {
		return;
	}

	int comm_size;
	MPI_Comm_size(comm, &comm_size);
	double* buf = new double[comm_size];
	MPI_Allgather(&x, 1, MPI_DOUBLE, buf, comm_size, MPI_DOUBLE, comm);

	for (int i = 0; i < comm_size; i++) {
                std::cout << buf[i];
	}

	delete buf;

        MPI_Finalize();
        return 0;
    }

