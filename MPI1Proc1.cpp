#include "stdafx.h"
#include <iostream>
#include <mpi.h>


int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	double X;
	if (rank <= 0) {
                std::cin >> X;
                std::cout << -X;
                std::cout << "-X=" <<  -X;
	}
	else {
                std::cout << rank;
                std::cout << "rank=" << rank;
	}

    MPI_Finalize();
    return 0;
}
