//MPI3Coll2
//MPI3Coll4
//MPI3Coll10
//MPI3Coll12
//MPI3Coll15
//MPI3Coll18

#include <iostream>
#include <mpi.h>

using namespace std;

void Coll2() {

	int rc = MPI_Init(NULL, NULL);
	if (rc != MPI_SUCCESS) {
		printf("MPI Error \n");
		MPI_Abort(MPI_COMM_WORLD, rc);
	}

	int rank, size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int* Input = nullptr;

	if (rank == 0)
		Input = new int[5]{ 1,2,3,4,5 };
	else
		Input = new int[5];
	
	for (int i = 0; i < 5; i++) {
		MPI_Bcast(&Input[i], 1, MPI_INT, 0 /*root*/, MPI_COMM_WORLD);
		MPI_Barrier(MPI_COMM_WORLD);
	}

	cout << "Rank: " << rank << "     Array: \n";
	for (int i = 0; i < 5; i++)
	{
		cout << Input[i] << "  ";
	}
	cout << "\n\n";

	MPI_Finalize();
}

void Coll4() { 
	int rc = MPI_Init(NULL, NULL);
	if (rc != MPI_SUCCESS) {
		printf("MPI Error \n");
		MPI_Abort(MPI_COMM_WORLD, rc);
	}

	int rank, size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int* Input0 = new int[5]{ 0, 0, 0, 0, 0 };

	int* Input1 = new int[5]{ 1, 1, 1, 1, 1};

	int* Input2 = new int[5]{ 2, 2, 2, 2, 2 };

	int* Input3 = new int[5]{ 3, 3, 3, 3, 3 };

	int* Input = nullptr;

	switch (rank) {
	case 0:Input = Input0; break;
	case 1:Input = Input1; break;
	case 2:Input = Input2; break;
	case 3:Input = Input3; break;
	}

	int* Buffer = new int[5 * size];

	MPI_Gather(Input, 5, MPI_INT, Buffer, 5, MPI_INT, 0 , MPI_COMM_WORLD);
	
	if (rank == 0) {
		for (int i = 0; i < size * 5; i++) {
			cout << Buffer[i] << " ";
		}
		cout << endl;
	}

	MPI_Finalize();
}

void Coll10() { 
	int rc = MPI_Init(NULL, NULL);
	if (rc != MPI_SUCCESS) {
		printf("MPI Error \n");
		MPI_Abort(MPI_COMM_WORLD, rc);
	}

	int rank, size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);


	int* Input = new int[size + 2];
	if (rank == 0)
		Input = new int[size+2]{ 1, 2, 3, 4, 5, 6 };

	int* sendcounts = new int[size];
	int* displacements = new int[size];
	for (int i = 0; i < size; i++) {
		sendcounts[i] = 3;
		displacements[i] = i;
	}

	int* Buffer = new int[3];

	MPI_Scatterv(Input, sendcounts, displacements, MPI_INT, Buffer, 3, MPI_INT, 0, MPI_COMM_WORLD);

	for (int k = 0; k < size; k++) {
		if (k == rank) {
			cout << "Rank: " << rank << endl;
			for (int i = 0; i < 3; i++) {
				cout << Buffer[i] << " ";
			}
			cout << endl << endl;
		}
		MPI_Barrier(MPI_COMM_WORLD);
	}

	MPI_Finalize();
}

void Coll12() {
	int rc = MPI_Init(NULL, NULL);
	if (rc != MPI_SUCCESS) {
		printf("MPI Error \n");
		MPI_Abort(MPI_COMM_WORLD, rc);
	}

	int rank, size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int* Input0 = new int[4]{ 0, 0, 0, 0 };

	int* Input1 = new int[4]{ 1, 1, 1, 1 };

	int* Input2 = new int[4]{ 2, 2, 2, 2 };

	int* Input3 = new int[4]{ 3, 3, 3, 3 };

	int* Input = nullptr;

	switch (rank) {
	case 0:Input = Input0; break;
	case 1:Input = Input1; break;
	case 2:Input = Input2; break;
	case 3:Input = Input3; break;
	}

	int* Buffer = new int[4 * size];

	MPI_Allgather(Input, 4, MPI_INT, Buffer, 4, MPI_INT, MPI_COMM_WORLD);

	for (int k = 0; k < size; k++) {
		if (k == rank) {
			cout << "Rank: " << rank << endl;
			for (int i = 0; i < 4 * size; i++) {
				cout << Buffer[i] << " ";
			}
			cout << endl << endl;
		}
		MPI_Barrier(MPI_COMM_WORLD);
	}

	MPI_Finalize();
}



void Coll15() { 
	int rc = MPI_Init(NULL, NULL);
	if (rc != MPI_SUCCESS) {
		printf("MPI Error \n");
		MPI_Abort(MPI_COMM_WORLD, rc);
	}

	int rank, size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int* Input0 = new int[3 * size]{ 0, 0, 4, 0, 0, 5, 0, 0, 6, 0, 0, 7 };

	int* Input1 = new int[3 * size]{ 1, 1, 4, 1, 1, 5, 1, 1, 6, 1, 1, 7 };

	int* Input2 = new int[3 * size]{ 2, 2, 4, 2, 2, 5, 2, 2, 6, 2, 2, 7 };

	int* Input3 = new int[3 * size]{ 3, 3, 4, 3, 3, 5, 3, 3, 6, 3, 3, 7 };

	int* Input = nullptr;

	switch (rank) {
	case 0:Input = Input0; break;
	case 1:Input = Input1; break;
	case 2:Input = Input2; break;
	case 3:Input = Input3; break;
	}

	int* Buffer = new int[3 * size];

	MPI_Alltoall(Input, 3, MPI_INT, Buffer, 3, MPI_INT, MPI_COMM_WORLD);

	for (int k = 0; k < size; k++) {
		if (k == rank) {
			cout << "Rank: " << rank << endl;
			for (int i = 0; i < 3 * size; i++) {
				cout << Buffer[i] << " ";
			}
			cout << endl << endl;
		}
		MPI_Barrier(MPI_COMM_WORLD);
	}

	MPI_Finalize();
}

void Coll18() {
	int rc = MPI_Init(NULL, NULL);
	if (rc != MPI_SUCCESS) {
		printf("MPI Error \n");
		MPI_Abort(MPI_COMM_WORLD, rc);
	}

	int rank, size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int* Input0 = new int[size + 1]{ 1, 2, 3, 4, 5 };

	int* Input1 = new int[size + 1]{ 6, 7, 8, 9, 10 };

	int* Input2 = new int[size + 1]{ 11, 12, 13, 14, 15 };

	int* Input3 = new int[size + 1]{ 16, 17, 18, 19, 20 };

	int* Input = nullptr;

	switch (rank) {
	case 0:Input = Input0; break;
	case 1:Input = Input1; break;
	case 2:Input = Input2; break;
	case 3:Input = Input3; break;
	}

	int* Buffer = new int[2 * size];

	int* sendcounts = new int[size];
	int* displacements = new int[size];
	int* rdisplacements = new int[size];
	for (int i = 0; i < size; i++) {
		sendcounts[i] = 2;
		displacements[i] = size - 1 - i;
		rdisplacements[i] = 2 * i;
	}

	MPI_Alltoallv(Input, sendcounts, displacements, MPI_INT, Buffer, sendcounts, rdisplacements, MPI_INT, MPI_COMM_WORLD);

	for (int k = 0; k < size; k++) {
		if (k == rank) {
			cout << "Rank: " << rank << endl;
			for (int i = 0; i < 2 * size; i++) {
				cout << Buffer[i] << " ";
			}
			cout << endl << endl;
		}
		MPI_Barrier(MPI_COMM_WORLD);
	}

	MPI_Finalize();
}


int main()
{
    Coll2();
    Coll4();
    Coll10();
    Coll12();
    Coll15();
    Coll18();
    return 0;
}