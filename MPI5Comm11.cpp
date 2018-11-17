#include "stdafx.h"
#include <iostream>
#include <mpi.h>


int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	double x, res;
	int N;
        std::cin >> N >> x;
	MPI_Comm comm;
	MPI_Comm_split(MPI_COMM_WORLD, N, rank, &comm);

	MPI_Allreduce(&x, &res, 1, MPI_DOUBLE, (N == 2) ? MPI_MAX : MPI_MIN, comm);
        std::cout << res;

    MPI_Finalize();
    return 0;
}
