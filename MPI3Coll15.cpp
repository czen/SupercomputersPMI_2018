#include "stdafx.h"
#include <iostream>
#include <mpi.h>


int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int i;
	int* buf = new int[size * 3];
	int* recvbuf = new int[size * 3];

	for (i = 0; i < size * 3; i++) {
                std::cin >> buf[i];
	}

	MPI_Alltoall(buf, 3, MPI_INT, recvbuf, 3, MPI_INT, MPI_COMM_WORLD);

	for (i = 0; i < size * 3; i++) {
                std::cout << recvbuf[i];
	}


	delete buf;

        MPI_Finalize();
        return 0;
    }
