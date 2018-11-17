#include "stdafx.h"
#include <iostream>
#include <mpi.h>


int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	double A;
	int N;
        std::cin >> A >> N;
	MPI_Status stat;
	if (rank != N) {
		MPI_Send(&A, 1, MPI_DOUBLE, N, 0, MPI_COMM_WORLD);
		MPI_Recv(&A, 1, MPI_DOUBLE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &stat);
	}
	else {
		stat.MPI_SOURCE = rank;
	}
        std::cout << A << stat.MPI_SOURCE;


        MPI_Finalize();
        return 0;
    }
