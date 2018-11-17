#include "stdafx.h"
#include <iostream>
#include <mpi.h>


int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int N;
        std::cin >> N;
	double* arr = new double[N];
	for (int i = 0; i < N; i++) {
                std::cin >> arr[i];
	}

	double res = 0;
	if (rank == 0) {
		res = 1;
	}
	for (int i = 0; i < N; i++) {
		res = (rank == 0 ? (res*arr[i]) : (res + arr[i]));
	}
	if (rank % 2 != 0) {
		res /= N;
	}
        std::cout << res;



	delete arr;

        MPI_Finalize();
        return 0;
    }
