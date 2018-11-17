#include "stdafx.h"
#include <iostream>
#include <mpi.h>


int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	MPI_Comm comm, subcomm;
	int dims[2] = { 2, size / 2 };
	int repeat[2] = { 0, 0 };
	int subdim[2] = { 0, 1 };
	MPI_Cart_create(MPI_COMM_WORLD, 2, dims, repeat, 1, &comm);
	if (comm == MPI_COMM_NULL) {
		return;
	}

	MPI_Cart_sub(comm, subdim, &subcomm);
	if (subcomm == MPI_COMM_NULL) {
		return;
	}

	double A;
	int dim, period, coord;
	MPI_Cart_get(subcomm, 1, &dim, &period, &coord);
	if (coord == 0) {
                std::cin >> A;
	}
	MPI_Bcast(&A, 1, MPI_DOUBLE, 0, subcomm);
        std::cout << A;

        MPI_Finalize();
        return 0;
    }

