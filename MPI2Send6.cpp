#include "stdafx.h"
#include <iostream>
#include <mpi.h>


int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	double* dbuf = new double[size];
	int s;
	MPI_Pack_size(size, MPI_DOUBLE, MPI_COMM_WORLD, &s);
	int bufsize = 2 * MPI_BSEND_OVERHEAD + 2 * s;
	char* buf = (char*)malloc(bufsize);
	MPI_Buffer_attach(buf, bufsize);

	if (rank == 0) {
		for (int i = 0; i < size - 1; i++) {
                        std::cin >> dbuf[i];
			MPI_Bsend(&dbuf[i], 1, MPI_DOUBLE, size - 1 - i, 0, MPI_COMM_WORLD);
		}
	}
	else {
		double rec_num;
		MPI_Recv(&rec_num, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                std::cout << rec_num;
	}

	delete dbuf;
        MPI_Finalize();
        return 0;
    }

