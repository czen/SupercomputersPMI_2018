//MPI5Comm2
//MPI5Comm5
//MPI5Comm7
//MPI5Comm8
//MPI5Comm12
//MPI5Comm13
//MPI5Comm18
//MPI5Comm19

#include <iostream>
#include <mpi.h>

using namespace std;

void Comm2() {
	int rc = MPI_Init(NULL, NULL);
	if (rc != MPI_SUCCESS) {
		printf("MPI Error \n");
		MPI_Abort(MPI_COMM_WORLD, rc);
	}

	int rank, size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	double a=0, b=0;
	if (rank % 2 == 1)
	{
		a = rank + 0.5; b = rank + 0.6;
	}

	MPI_Group newgroup, basegroup;
	MPI_Comm newcomm;

	int chetsize = size / 2 + size % 2;
	int nechetsize = size / 2;
	int *ranks = new int[chetsize];

	double *buffer = new double[nechetsize * 2];

	for (int i = 0; i < chetsize; i++)
		ranks[i] = (2 * i);

	MPI_Comm_group(MPI_COMM_WORLD, &basegroup);
	MPI_Group_excl(basegroup, chetsize, ranks, &newgroup);
	MPI_Comm_create(MPI_COMM_WORLD, newgroup, &newcomm);

	if (rank % 2 == 1) {
		buffer[rank - 1] = a;
		buffer[rank] = b;

		int newrank, newsize;
		MPI_Comm_size(newcomm, &newsize);
		MPI_Comm_rank(newcomm, &newrank);

		for (int i = 0; i < newsize; i++)
			MPI_Bcast(&buffer[2 * i], 2, MPI_DOUBLE, i, newcomm);
	}

	for (int k = 1; k < size; k += 2) {
		if (k == rank) {
			for (int i = 0; i < nechetsize * 2; i++)
			{
				cout << buffer[i] << " ";
			}
			cout << endl << endl;
		}
		MPI_Barrier(MPI_COMM_WORLD);
	}
	MPI_Finalize();
}




void Comm5() {
    
    int rc = MPI_Init(NULL, NULL);
	if (rc != MPI_SUCCESS) {
		printf("MPI Error \n");
		MPI_Abort(MPI_COMM_WORLD, rc);
	}

    int rank, size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Comm comm;

    MPI_Comm_split(MPI_COMM_WORLD, rank % 2, rank, &comm);

    double buf = rank;
    double res;
    if (rank % 2)
        MPI_Reduce(&buf, &res, 1, MPI_DOUBLE, MPI_MAX, 0, comm);
    else
        MPI_Reduce(&buf, &res, 1, MPI_DOUBLE, MPI_MIN, 1, comm);

    if (rank == 0)
        cout << res << endl;
    if (rank == 1)
        cout << res << endl;

    MPI_Comm_free(&comm);
    MPI_Finalize();
}

void Comm7() {
	int rc = MPI_Init(NULL, NULL);
	if (rc != MPI_SUCCESS) {
		printf("MPI Error \n");
		MPI_Abort(MPI_COMM_WORLD, rc);
	}

	int rank, size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int color = MPI_UNDEFINED;

	MPI_Comm newcomm;

	int N = 0;

	rank % 2 == 0 ? N = 1 : NULL;

	double A = NULL;

	double* Receive = nullptr;

	double *buffer = nullptr;

	if (N == 1)
	{
		color = 4;
		buffer = new double[3];
		A = rank + 0.5;
	}

	MPI_Comm_split(MPI_COMM_WORLD, color, rank, &newcomm);

	if (color != MPI_UNDEFINED) {
		int newrank, newsize;
		MPI_Comm_size(newcomm, &newsize);
		MPI_Comm_rank(newcomm, &newrank);

		MPI_Gather(&A, 1, MPI_DOUBLE, buffer, 1, MPI_DOUBLE, 0, newcomm);

		if (0 == newrank) {
			for (int i = 0; i < newsize; i++)
			{
				cout << buffer[i] << " ";
			}
			cout << endl << endl;
		}
	}

	MPI_Finalize();
}

void Comm8() {
	int rc = MPI_Init(NULL, NULL);
	if (rc != MPI_SUCCESS) {
		printf("MPI Error \n");
		MPI_Abort(MPI_COMM_WORLD, rc);
	}

	int rank, size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int color = MPI_UNDEFINED;

	MPI_Comm newcomm;

	int N = 0;

	rank % 2 == 0 ? N = 1 : NULL;

	double A = NULL;

	double* Receive = nullptr;

	double *buffer = nullptr;

	if (N == 1)
	{
		color = 4;
		buffer = new double[3];
		A = rank + 0.5;
	}

	MPI_Comm_split(MPI_COMM_WORLD, color, rank, &newcomm);

	if (color != MPI_UNDEFINED) {
		int newrank, newsize;
		MPI_Comm_size(newcomm, &newsize);
		MPI_Comm_rank(newcomm, &newrank);

		MPI_Gather(&A, 1, MPI_DOUBLE, buffer, 1, MPI_DOUBLE, newsize - 1, newcomm);

		if (newsize - 1 == newrank) {
			for (int i = 0; i < newsize; i++)
			{
				cout << buffer[i] << " ";
			}
			cout << endl << endl;
		}
	}

	MPI_Finalize();
}


void Comm12() {
	int rc = MPI_Init(NULL, NULL);
	if (rc != MPI_SUCCESS) {
		printf("MPI Error \n");
		MPI_Abort(MPI_COMM_WORLD, rc);
	}

	int rank, size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int color;
	color = rank % 2 == 0 ? 2 : 1;

	int N;
	N = color == 2 ? 2 : 1;

	MPI_Comm newcomm;

	double A = rank + 0.5;

	double* Receive = nullptr;

	double *buffer = nullptr;

	buffer = new double[3];

	MPI_Comm_split(MPI_COMM_WORLD, color, rank, &newcomm);

	int newrank, newsize;
	MPI_Comm_size(newcomm, &newsize);
	MPI_Comm_rank(newcomm, &newrank);

	if (N == 1) 
		MPI_Allreduce(MPI_IN_PLACE, &A, 1, MPI_DOUBLE, MPI_MIN, newcomm);
	else
		MPI_Allreduce(MPI_IN_PLACE, &A, 1, MPI_DOUBLE, MPI_MAX, newcomm);

	for (int k = 0; k < size; k++) {
		if (k == rank) {
			char * minmax = (char *)(N == 1 ? "min: " : "max: ");
			cout << "Rank: " << rank << ", " << minmax << A << endl << endl;
		}
		MPI_Barrier(MPI_COMM_WORLD);
	}

	MPI_Finalize();
}

void Comm13() {
	int rc = MPI_Init(NULL, NULL);
	if (rc != MPI_SUCCESS) {
		printf("MPI Error \n");
		MPI_Abort(MPI_COMM_WORLD, rc);
	}

	int rank, size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int N;
	if (rank == 0)
		N = 2;

	MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);

	MPI_Comm topology;

	const int ndims = 2;
	int dims[ndims] = { N , size / N };
	int periods[ndims] = { true , true };
	int reorder = false;

	MPI_Cart_create(MPI_COMM_WORLD, ndims, dims, periods, reorder, &topology);

	int coords[ndims];
	MPI_Cart_coords(topology, rank, ndims, coords);

	for (int k = 0; k < size; k++) {
		if (k == rank) {
			cout << "Rank: " << rank << ", coords: " << coords[0] << ", " << coords[1] << endl << endl;
		}
		MPI_Barrier(MPI_COMM_WORLD);
	}

	MPI_Finalize();
}

void Comm18() {
	int rc = MPI_Init(NULL, NULL);
	if (rc != MPI_SUCCESS) {
		printf("MPI Error \n");
		MPI_Abort(MPI_COMM_WORLD, rc);
	}

	int rank, size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int N = size / 3;
	int *numbers = nullptr;
	if (rank < 3)
		numbers = new int[N], fill_n(numbers, N, rank);
	else
		numbers = new int[N];

	MPI_Comm topology, new_topology;

	const int ndims = 2;
	int dims[ndims] = { N , 3 };
	int periods[ndims] = { true , true };
	int reorder = false;
	MPI_Cart_create(MPI_COMM_WORLD, ndims, dims, periods, reorder, &topology);

	int remain_dims[ndims] = { true , false };
	MPI_Cart_sub(topology, remain_dims, &new_topology);

	int* sendcounts = new int[N];
	int* displacements = new int[N];
	for (int i = 0; i < N; i++) {
		sendcounts[i] = 1;
		displacements[i] = i;
	}
	int Receive;

	MPI_Scatterv(numbers, sendcounts, displacements, MPI_INT, &Receive, 1, MPI_INT, 0, new_topology);
	for (int k = 0; k < size; k++) {
		if (k == rank) {
			cout << "Rank: " << rank << ", number: " << Receive << endl << endl;
		}
		MPI_Barrier(MPI_COMM_WORLD);
	}

	MPI_Finalize();
}

void Comm19() {
	int rc = MPI_Init(NULL, NULL);
	if (rc != MPI_SUCCESS) {
		printf("MPI Error \n");
		MPI_Abort(MPI_COMM_WORLD, rc);
	}

	int rank, size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int number = rank;

	MPI_Comm topology, new_topology;

	const int ndims = 3;
	int dims[ndims] = { 2 , 2 , size / 4 };
	int periods[ndims] = { true , true , true };
	int reorder = false;
	MPI_Cart_create(MPI_COMM_WORLD, ndims, dims, periods, reorder, &topology);

	int remain_dims[ndims] = { false , false , true };
	MPI_Cart_sub(topology, remain_dims, &new_topology);

	int newrank, newsize;
	MPI_Comm_size(new_topology, &newsize);
	MPI_Comm_rank(new_topology, &newrank);

	int* Receive = new int[newsize];
	MPI_Gather(&number, 1, MPI_INT, Receive, 1, MPI_INT, 0, new_topology);

	for (int k = 0; k < size; k++) {
		if (k == rank) {
			if (0 == newrank) {
				cout << "oldRank: " << rank << ", newRank: " << newrank << ", numbers: " << endl;
				for (int i = 0; i < newsize; i++)
					cout << Receive[i] << " ";
				cout << endl << endl;
			}
		}
		MPI_Barrier(MPI_COMM_WORLD);
	}
	MPI_Finalize();
}

int main()
{
    Comm2();
	Comm5();
	Comm7();
	Comm8();
	Comm12();
	Comm13();
	Comm18();
    Comm19();
    return 0;
}