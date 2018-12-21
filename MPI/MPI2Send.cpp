//MPI2Send4
//MPI2Send6
//MPI2Send8
//MPI2Send14
//MPI2Send15

#include <iostream>
#include <mpi.h>

using namespace std;

void Send4() { 
	int rc = MPI_Init(NULL, NULL);
	if (rc != MPI_SUCCESS) {
		printf("MPI Error \n");
		MPI_Abort(MPI_COMM_WORLD, rc);
	}

	int rank, size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	const int N[4] = { 0, 3, 5, 1 };

	int* Input1 = new int[3]{ 4 , 6 , 1 };

	int* Input2 = new int[5]{ 1, 1, 1, 1, 1 };

	int* Input3 = new int[1]{ 4 };

	
	int overheadN = 10 + MPI_BSEND_OVERHEAD;

	int* Receive = new int[overheadN];

	int* Buffer = new int[overheadN];

	int buffN = overheadN * sizeof(int);

	int count;

	MPI_Status stat;

	switch (rank) {
	case 1:
		MPI_Buffer_attach(Buffer, buffN);
		MPI_Bsend(Input1, N[rank], MPI_INT, 0, rank, MPI_COMM_WORLD);
		MPI_Buffer_detach(&Buffer, &buffN);
		break;
	case 2:
		MPI_Buffer_attach(Buffer, buffN);
		MPI_Bsend(Input2, N[rank], MPI_INT, 0, rank, MPI_COMM_WORLD);
		MPI_Buffer_detach(Buffer, &buffN);
		break;
	case 3:
		MPI_Buffer_attach(Buffer, buffN);
		MPI_Bsend(Input3, N[rank], MPI_INT, 0, rank, MPI_COMM_WORLD);
		MPI_Buffer_detach(Buffer, &buffN);
		break;
	case 0:
		for (int r = 1; r < size; r++) {
			MPI_Recv(Receive, INT_MAX, MPI_INT, r, r, MPI_COMM_WORLD, &stat);
			MPI_Get_count(&stat, MPI_INT, &count);
			cout << "RANK: " << r << endl;
			for (int i = 0; i < count; i++)
				cout << Receive[i] << " ";
			cout << endl;
		}
		break;
	}

	MPI_Finalize();
}

void Send6() { 
	int rc = MPI_Init(NULL, NULL);
	if (rc != MPI_SUCCESS) {
		printf("MPI Error \n");
		MPI_Abort(MPI_COMM_WORLD, rc);
	}

	int rank, size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	const int N = 3 ;

	double* Input = new double[N]{ 4.1 , 6.2 , 1.3 };

	const int overheadN = N + MPI_BSEND_OVERHEAD;

	double Receive;

	double* Buffer = new double[overheadN];

	int buffN = overheadN * sizeof(double);

	switch (rank) {
	case 0:
		MPI_Buffer_attach(Buffer, buffN);
		for (int r = size - 1; r > 0; r--) {
			MPI_Bsend(&Input[size - r - 1], 1, MPI_DOUBLE, r, r, MPI_COMM_WORLD);
		}
		MPI_Buffer_detach(Buffer, &buffN);
		break;
	case 1:;
	case 2:;
	case 3:
		MPI_Recv(&Receive, 1, MPI_DOUBLE, 0, rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		cout << "RANK: " << rank << ", NUMBER: " << Receive << endl;
		break;
	}

	MPI_Finalize();
}

void Send8() {
	int rc = MPI_Init(NULL, NULL);
	if (rc != MPI_SUCCESS) {
		printf("MPI Error \n");
		MPI_Abort(MPI_COMM_WORLD, rc);
	}

	int rank, size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	const int N = 3;

	int* Input = new int[N] { 0, 0, 1255};

	int Receive;

	MPI_Status stat;

	if (rank == 0)
	{
		MPI_Recv(&Receive, 1, MPI_INT, MPI_ANY_SOURCE, 55, MPI_COMM_WORLD, &stat);
		cout << "RANK: " << stat.MPI_SOURCE << ", NUMBER: " << Receive << endl;
	}
	else {
		if (Input[rank - 1] != 0)
			MPI_Send(&Input[rank - 1], 1, MPI_INT, 0, 55, MPI_COMM_WORLD);
	}
	
	MPI_Finalize();
}

void Send14(){

	const int N = 10;
	int* Input = new int[N] { 4, 1, 0, 2, 1, 3, 2, 4, 3, 0};
	int rc = MPI_Init(NULL, NULL);
	if (rc != MPI_SUCCESS) {
		printf("MPI Error \n");
		MPI_Abort(MPI_COMM_WORLD, rc);
	}

	int rank, size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int Receive[2];

	int previous = rank == 0 ? size - 1 : rank - 1, next = rank == size - 1 ? 0 : rank + 1;

	if (size % 2 == 0) {
		if (rank % 2 == 0) {
			MPI_Ssend(&Input[2 * rank + 1], 1, MPI_INT, next, rank, MPI_COMM_WORLD);
			MPI_Recv(&Receive[1], 1, MPI_INT, next, next, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

			MPI_Ssend(&Input[2 * rank], 1, MPI_INT, previous, rank, MPI_COMM_WORLD);
			MPI_Recv(&Receive[0], 1, MPI_INT, previous, previous, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}
		else {
			MPI_Recv(&Receive[0], 1, MPI_INT, previous, previous, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			MPI_Ssend(&Input[2 * rank], 1, MPI_INT, previous, rank, MPI_COMM_WORLD);

			MPI_Recv(&Receive[1], 1, MPI_INT, next, next, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			MPI_Ssend(&Input[2 * rank + 1], 1, MPI_INT, next, rank, MPI_COMM_WORLD);
		}
	}
	else {
		if (rank % 2 == 0) {
			if (next != size - 1 && next != 0) {
				MPI_Ssend(&Input[2 * rank + 1], 1, MPI_INT, next, rank, MPI_COMM_WORLD);
				MPI_Recv(&Receive[1], 1, MPI_INT, next, next, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			}

			if (previous != size - 2 && previous != size - 1) {
				MPI_Ssend(&Input[2 * rank], 1, MPI_INT, previous, rank, MPI_COMM_WORLD);
				MPI_Recv(&Receive[0], 1, MPI_INT, previous, previous, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			}
		}
		else {
			if (previous != size - 2 && previous != size - 1) {
				MPI_Recv(&Receive[0], 1, MPI_INT, previous, previous, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				MPI_Ssend(&Input[2 * rank], 1, MPI_INT, previous, rank, MPI_COMM_WORLD);
			}

			if (next != size - 1 && next != 0) {
				MPI_Recv(&Receive[1], 1, MPI_INT, next, next, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				MPI_Ssend(&Input[2 * rank + 1], 1, MPI_INT, next, rank, MPI_COMM_WORLD);
			}
		}
		
		if (rank == size - 2)
			MPI_Ssend(&Input[2 * rank + 1], 1, MPI_INT, next, rank, MPI_COMM_WORLD);
		if (rank == size - 1) {
			MPI_Recv(&Receive[0], 1, MPI_INT, previous, previous, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			MPI_Ssend(&Input[2 * rank], 1, MPI_INT, previous, rank, MPI_COMM_WORLD);
		}
		if (rank == size - 2)
			MPI_Recv(&Receive[1], 1, MPI_INT, next, next, MPI_COMM_WORLD, MPI_STATUS_IGNORE);


		if (rank == 0)
			MPI_Ssend(&Input[2 * rank], 1, MPI_INT, previous, rank, MPI_COMM_WORLD);
		if (rank == size - 1) {
			MPI_Recv(&Receive[1], 1, MPI_INT, next, next, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			MPI_Ssend(&Input[2 * rank + 1], 1, MPI_INT, next, rank, MPI_COMM_WORLD);
		}
		if (rank == 0)
			MPI_Recv(&Receive[0], 1, MPI_INT, previous, previous, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}

	cout << "rank: " << rank << ", previous: " << Receive[0] << ", next: " << Receive[1] << endl;

	MPI_Finalize();
}


void Send15(){

	const int sizeN = 4;

	const int* N = new int[sizeN] { 2, 3, 1, 0};  // куда

	double* A = new double[sizeN] { 4.7, 5.2, 9.3, 0.22}; // что

	int rc = MPI_Init(NULL, NULL);
	if (rc != MPI_SUCCESS) {
		printf("MPI Error \n");
		MPI_Abort(MPI_COMM_WORLD, rc);
	}

	int rank, size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	double Receive;

	MPI_Status stat;

	MPI_Send(&A[rank], 1, MPI_DOUBLE, N[rank], N[rank], MPI_COMM_WORLD);
	MPI_Recv(&Receive, 1, MPI_DOUBLE, MPI_ANY_SOURCE, rank, MPI_COMM_WORLD, &stat);

	cout << "Rank: " << rank << ", number: " << Receive << ", sourceRank: " << stat.MPI_SOURCE << endl;

	MPI_Finalize();
}

int main()
{
    Send4();
    Send6();
    Send8();
    Send14();
    Send15();
    return 0;
}