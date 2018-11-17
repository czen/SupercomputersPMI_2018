#include "stdafx.h"
#include <iostream>
#include <mpi.h>


int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	MPI_Comm comm;
	int dims[3] = { 2, 2, size / 4 };
	int periods[3] = { 0, 0, 0 };
	MPI_Cart_create(MPI_COMM_WORLD, 3, dims, periods, 1, &comm);
	if (comm == MPI_COMM_NULL) {
		return;
	}

	MPI_Comm subcomm;
	int cutdims[3] = { 1, 1, 0 };
	MPI_Cart_sub(comm, cutdims, &subcomm);
	if (subcomm == MPI_COMM_NULL) {
		return;
	}

	double A, res = -1;
        std::cin >> A;
	MPI_Allreduce(&A, &res, 1, MPI_DOUBLE, MPI_SUM, subcomm);
        std::cout << res;


        MPI_Finalize();
        return 0;
    }

