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
	if (rank == 0) {
                std::cin >> N;
	}
	MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
	
	MPI_Comm comm;
	int dims[2] = { N, size / N };
	int periods[2] = { 0, 1 };
	int coords[2];
	MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 1, &comm);
	if (comm == MPI_COMM_NULL) {
		return;
	}
	MPI_Comm_rank(comm, &rank);
	MPI_Cart_coords(comm, rank, 2, coords);
        std::cout << coords[0] << coords[1];
	

        MPI_Finalize();
        return 0;
    }

