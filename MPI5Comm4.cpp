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
	int color = rank % 3 == 0 ? 0 : MPI_UNDEFINED;
	MPI_Comm_split(MPI_COMM_WORLD, color, rank, &comm);
	if (comm == MPI_COMM_NULL) {
		return;
	}

	MPI_Comm_size(comm, &size);
	std::vector<int> res(3 * size), data(ptin_iterator<int>(3), ptin_iterator<int>());
	MPI_Gather(&data[0], 3, MPI_INT, &res[0], 3, MPI_INT, 0, comm);
	if (rank == 0) {
		copy(res.begin(), res.end(), ptout_iterator<int>());
	}
        MPI_Finalize();
        return 0;
    }

