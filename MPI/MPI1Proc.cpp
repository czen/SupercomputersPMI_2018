//MPI1Proc1
//MPI1Proc10

#include <iostream>
#include <mpi.h>

using namespace std;

void Proc1(){
	int rc = MPI_Init(NULL, NULL);
	if (rc != MPI_SUCCESS) {
		printf("MPI Error \n");
		MPI_Abort(MPI_COMM_WORLD, rc);
	}

	int rank, size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	double n = rank + 0.5;
	cout << "rank: " << rank << ", -n: " << -n << endl;

	MPI_Finalize();
}

void Proc10() { 
	int rc = MPI_Init(NULL, NULL);
	if (rc != MPI_SUCCESS) {
		printf("MPI Error \n");
		MPI_Abort(MPI_COMM_WORLD, rc);
	}

	int rank, size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int *N = new int[size] {3, 4, 2, 5};
	//int *N = new int[size] {3, 4, 2, 5, 1};

	double* Input0 = new double[N[0]]{ 0.1, 0.2, 0.3 };
	//int* Input0 = new int[N[0]]{ 1, 2, 4 };
	double* Input1 = new double[N[1]]{ 1.1, 1.2, 1.3, 1.4 };
	int* Input2 = new int[N[2]]{ 2, 4 };
	double* Input3 = new double[N[3]]{ 3.1, 3.2, 3.3, 3.4, 3.5 };
	//int* Input4 = new int[N[3]]{ 4 };

	void* Input = nullptr;

	switch (rank) {
	case 0:Input = Input0; break;
	case 1:Input = Input1; break;
	case 2:Input = Input2; break;
	case 3:Input = Input3; break;
	//case 4:Input = Input4; break;
	}
	int i;
	double d;
	double max = INT_MIN;
	double min = INT_MAX;
	for (int j = 0; j < N[rank]; j++) {
		if (rank == 0) {
			if (size % 2 == 0) {
				d = ((double *)Input)[j];
				if (d < min)
					min = d;
			}
			else {
				i = ((int *)Input)[j];
				if (i < min)
					min = i;
			}
		}
		else
			if (rank % 2 == 0) {
				i = ((int *)Input)[j];
				if (i < min)
					min = i;
			}
			else {
				d = ((double *)Input)[j];
				if (d > max)
					max = d;
			}
	}

	if (rank % 2 == 0)
		cout << "Rank: " << rank << ", min: " << min << endl;
	else
		cout << "Rank: " << rank << ", max: " << max << endl;

	MPI_Finalize();
}


int main()
{
    Proc1();
    Proc10();
    return 0;
}