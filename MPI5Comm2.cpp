#include "stdafx.h"
#include <iostream>
#include <mpi.h>


int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	MPI_Comm odd_comm, world_comm;
	MPI_Comm_dup(MPI_COMM_WORLD, &world_comm);

	int even_group_size = size / 2 + (size % 2);
	int* evenranks = new int[even_group_size];
	for (int i = 0; i < even_group_size; i++) {
		evenranks[i] = i * 2;
	}

	MPI_Group odd_group, world_group;
	MPI_Comm_group(world_comm, &world_group);
	MPI_Group_excl(world_group, even_group_size, evenranks, &odd_group);

	MPI_Comm_create(world_comm, odd_group, &odd_comm);
	if (odd_comm == MPI_COMM_NULL) {
		return;
	}
	MPI_Comm_size(odd_comm, &size);
	Show(size);

	double nums[2];
	int* recvcounts = new int[size];
	int* stride = new int[size];
	int* sendstride = new int[size];
	double* recvbuf = new double[2 * size];
	for (int i = 0; i < size; i++) {
		recvcounts[i] = 2;
		stride[i] = i * 2;
		sendstride[i] = 0;
	}
        std::cin >> nums[0] >> nums[1];
	MPI_Alltoallv(nums, recvcounts, sendstride, MPI_DOUBLE, recvbuf, recvcounts, stride, MPI_DOUBLE, odd_comm);
	for (int i = 0; i < size * 2; i++) {
                std::cout << recvbuf[i];
	}

        MPI_Finalize();
        return 0;
    }

