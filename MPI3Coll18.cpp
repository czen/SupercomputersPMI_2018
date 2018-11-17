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
	double* buf = new double[size + 1];
	double* recvbuf = new double[size * 2];

	int* sendcounts = new int[size];
	int* stride = new int[size];
	int* recvstride = new int[size];

	for (i = 0; i < size; i++) {
                std::cin >> buf[i];
		sendcounts[i] = 2;
		stride[i] = size - 1 - i;
		recvstride[i] = i*2;
	}
        std::cin >> buf[size];

	MPI_Alltoallv(buf, sendcounts, stride, MPI_DOUBLE,
		recvbuf, sendcounts, recvstride, MPI_DOUBLE, MPI_COMM_WORLD);

	for (i = 0; i < 2 * size; i++) {
                std::cout << recvbuf[i];
	}

	delete buf;
	delete recvbuf;
	delete stride;
	delete recvstride;
	delete sendcounts;
        MPI_Finalize();
        return 0;
    }

