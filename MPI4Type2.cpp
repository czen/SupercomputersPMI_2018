#include "stdafx.h"
#include <iostream>
#include <mpi.h>


int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	struct {
		int p1, p2, p3;
	} triple;

	MPI_Datatype triple_type;
	MPI_Type_contiguous(3, MPI_INT, &triple_type);
	MPI_Type_commit(&triple_type);

	int i;
	if (rank <= 0) {
		for (i = 1; i < size; i++) {
                        std::cin >> triple.p1;
                        std::cin >> triple.p2;
                        std::cin >> triple.p3;
			MPI_Send(&triple, 1, triple_type, i, 0, MPI_COMM_WORLD);
		}
	}
	else {
		MPI_Recv(&triple, 1, triple_type, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                std::cout << triple.p1 << triple.p2 << triple.p3;
	}
        MPI_Finalize();
        return 0;
    }

