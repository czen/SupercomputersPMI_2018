#include "stdafx.h"
#include <iostream>
#include <mpi.h>


int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int s;
	MPI_Pack_size(4, MPI_INT, MPI_COMM_WORLD, &s);
	int bufsize = size * MPI_BSEND_OVERHEAD + size * s;
	char* buf = (char*)malloc(bufsize);
	MPI_Buffer_attach(buf, bufsize);

	int toprev, tonext;
	int fromprev, fromnext;
        std::cin >> toprev >> tonext;
	if (rank != size - 1) {
		fromnext = 0;
		MPI_Ssend(&tonext, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
		MPI_Recv(&fromprev, 1, MPI_INT, (rank == 0) ? size - 1 : rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                std::cout << fromprev << fromnext;
	}
	else if (rank == size - 1) {
		fromnext = 0;
                MPI_Recv(&fromprev, 1, MPI_INT, size - 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Ssend(&tonext, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
		MPI_Ssend(&toprev, 1, MPI_INT, size - 2, 1, MPI_COMM_WORLD);
                std::cout << fromprev << fromnext;
	}


        MPI_Finalize();
        return 0;
    }
