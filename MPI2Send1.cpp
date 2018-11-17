#include "stdafx.h"
#include <iostream>
#include <mpi.h>

#define MAX_NUM 5
int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	int ibuf[MAX_NUM];
	int s;
	MPI_Pack_size(MAX_NUM, MPI_INT, MPI_COMM_WORLD, &s);
	int bufsize = size * MPI_BSEND_OVERHEAD + size * s;
	char* buf = (char*)malloc(bufsize);
	MPI_Buffer_attach(buf, bufsize);

	if (rank > 0) {
		int N;
                std::cin >> N;
		for (int i = 0; i < N; i++) {
                        std::cin >> ibuf[i];
		}
		
		MPI_Bsend(ibuf, N, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}
	else {
		int rec_nums[MAX_NUM];
		int nums;
		MPI_Status stat;
		for (int i = 1; i <= size - 1; i++) {
			MPI_Recv(rec_nums, MAX_NUM, MPI_INT, i, 0, MPI_COMM_WORLD, &stat);
			MPI_Get_count(&stat, MPI_INT, &nums);
			for (int j = 0; j < nums; j++) {
                                std::cout << rec_nums[j];
			}
		}
	}
        MPI_Finalize();
        return 0;
    }
