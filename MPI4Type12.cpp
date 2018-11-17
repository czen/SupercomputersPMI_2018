#include "stdafx.h"
#include <iostream>
#include <mpi.h>


int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int packsize, packsizedouble, packsizeint;
	MPI_Pack_size(size - 1, MPI_DOUBLE, MPI_COMM_WORLD, &packsizedouble);
	MPI_Pack_size(size*(size - 1) / 2, MPI_INT, MPI_COMM_WORLD, &packsizeint);
	packsize = packsizedouble + packsizeint;

	double d;
	int intx, i;
	char* packbuf = new char[packsize];

	if (rank > 0) {
		int position = 0;
                std::cin >> d;
		MPI_Pack(&d, 1, MPI_DOUBLE, packbuf, packsize, &position, MPI_COMM_WORLD);
		for (i = 0; i < rank; i++) {
                        std::cin >> intx;
			MPI_Pack(&intx, 1, MPI_INT, packbuf, packsize, &position, MPI_COMM_WORLD);
		}
		MPI_Send(packbuf, packsize, MPI_PACKED, 0, 0, MPI_COMM_WORLD);
	}
	if (rank <= 0) {
		for (i = 1; i < size; i++) {
			int position = 0;
			MPI_Recv(packbuf, packsize, MPI_PACKED, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			MPI_Unpack(packbuf, packsize, &position, &d, 1, MPI_DOUBLE, MPI_COMM_WORLD);
                        std::cout << d;
			for (int j = 0; j < i; j++) {
				MPI_Unpack(packbuf, packsize, &position, &intx, 1, MPI_INT, MPI_COMM_WORLD);
                                std::cout << intx;
			}
		}
	}

	delete packbuf;

        MPI_Finalize();
        return 0;
    }

