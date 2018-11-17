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
	int *buf = new int[(size + 1)*(size + 2)/2 - 1];
	int *sendbuf = new int[rank + 2];
	int *recvlen = new int[size];
	int *stride = new int[size];

	for (i = 0; i < rank + 2; i++) {
                std::cin >> sendbuf[i];
	}

	recvlen[0] = 2;
	stride[0] = 0;
	for (i = 1; i < size; i++) {
		recvlen[i] = i + 2;
		stride[i] = stride[i - 1] + recvlen[i - 1];
	}

	MPI_Allgatherv(sendbuf, rank + 2, MPI_INT, buf, recvlen, stride, MPI_INT, MPI_COMM_WORLD);
	for (i = 0; i < (size + 1)*(size + 2) / 2 - 1; i++) {
                std::cout << buf[i];
	}

	delete buf;
	delete sendbuf;
	delete recvlen;
	delete stride;

        MPI_Finalize();
        return 0;
    }
