#include "stdafx.h"
#include <iostream>
#include <mpi.h>


int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	struct triple_struct {
		int p1, p2;
		double p3;
	} triple;

	int i;
	triple_struct* buf = new triple_struct[size - 1];
	char* packbuf;
	int position = 0;
	int packsizeint, packsizedouble, packsize;
	MPI_Pack_size(size - 1, MPI_INT, MPI_COMM_WORLD, &packsizeint);
	MPI_Pack_size(size - 1, MPI_DOUBLE, MPI_COMM_WORLD, &packsizedouble);

	packsize = packsizeint * 2 + packsizedouble;
	packbuf = (char*)malloc(packsize);

	if (rank <= 0) {
		for (i = 0; i < size - 1; i++) {
                        std::cin >> buf[i].p1 >> buf[i].p2 >> buf[i].p3;
			MPI_Pack(&buf[i].p1, 1, MPI_INT, packbuf, packsize, &position, MPI_COMM_WORLD);
			MPI_Pack(&buf[i].p2, 1, MPI_INT, packbuf, packsize, &position, MPI_COMM_WORLD);
			MPI_Pack(&buf[i].p3, 1, MPI_DOUBLE, packbuf, packsize, &position, MPI_COMM_WORLD);
		}
		MPI_Bcast(&position, 1, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Bcast(packbuf, position, MPI_PACKED, 0, MPI_COMM_WORLD);
	} else {
		MPI_Bcast(&position, 1, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Bcast(packbuf, position, MPI_PACKED, 0, MPI_COMM_WORLD);
		int newpos = 0;
		for (i = 0; i < size - 1; i++) {
			MPI_Unpack(packbuf, position, &newpos, &buf[i].p1, 1, MPI_INT, MPI_COMM_WORLD);
			MPI_Unpack(packbuf, position, &newpos, &buf[i].p2, 1, MPI_INT, MPI_COMM_WORLD);
			MPI_Unpack(packbuf, position, &newpos, &buf[i].p3, 1, MPI_DOUBLE, MPI_COMM_WORLD);
                        std::cout << buf[i].p1 << buf[i].p2 << buf[i].p3;
		}
	}

	delete buf;

        MPI_Finalize();
        return 0;
    }

