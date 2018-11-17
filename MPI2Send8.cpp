#include "stdafx.h"
#include <iostream>
#include <mpi.h>


int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	//int ibuf;
	int s;
	MPI_Pack_size(size + 1, MPI_INT, MPI_COMM_WORLD, &s);
	int bufsize = MPI_BSEND_OVERHEAD + s;
	char* buf = (char*)malloc(bufsize);

	MPI_Buffer_attach(buf, bufsize);

	if (rank > 0) {
		int N;
                std::cin >> N;
		if (N != 0) {
			MPI_Bsend(&N, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
		}
	}
	else {
		int ibuf;
		MPI_Status stat;
		MPI_Recv(&ibuf, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &stat);
                std::cout << ibuf << stat.MPI_SOURCE;
	}


        MPI_Finalize();
        return 0;
    }
