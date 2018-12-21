//MPI4Type1
//MPI4Type5
//MPI4Type8
//MPI4Type9
//MPI4Type13

#include <iostream>
#include <mpi.h>

using namespace std;

void Type1() {

	int rc = MPI_Init(NULL, NULL);
	if (rc != MPI_SUCCESS) {
		printf("MPI Error \n");
		MPI_Abort(MPI_COMM_WORLD, rc);
	}

	int rank, size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int K = size - 1;

	int** Input = new int*[K];

	if (rank == 0) {
		Input[0] = new int[3]{ 1, 2, 3 };
		Input[1] = new int[3]{ 4, 5, 6 };
		Input[2] = new int[3]{ 7, 8, 9 };
	}
	else {
		Input[0] = new int[3];
		Input[1] = new int[3];
		Input[2] = new int[3];
	}

	MPI_Aint displacements[3];
	displacements[0] = 0;
	displacements[1] = 1;
	displacements[2] = 2;

	MPI_Datatype newtype;

	int count = 3;
	int blocklens[] = { 1,1,1 };

	MPI_Type_indexed(count, blocklens, displacements, MPI_INT, &newtype); //MPI_Type_contiguous
	MPI_Type_commit(&newtype);

	for (int i = 0; i < K; i++) {
		MPI_Bcast(Input[i], 1, newtype, 0, MPI_COMM_WORLD);
		MPI_Barrier(MPI_COMM_WORLD);
	}

	for (int k = 0; k < size; k++) {
		if (k == rank) {
			cout << "Rank: " << rank << endl;
			for (int i = 0; i < K; i++)
			{
				cout << "Troika nomer " << i + 1 << ": " << endl;
				cout << "a: " << Input[i][0] << "; b:" << Input[i][1] << "; c:" << Input[i][2] << "  \n";
			}
			cout << endl;
		}
		MPI_Barrier(MPI_COMM_WORLD);
	}

	MPI_Finalize();
}

void Type5() {
	int rc = MPI_Init(NULL, NULL);
	if (rc != MPI_SUCCESS) {
		printf("MPI Error \n");
		MPI_Abort(MPI_COMM_WORLD, rc);
	}

	int rank, size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	struct Troika {
		int a;
		double b;
		int c;
	};

	int K = size - 1;

	MPI_Datatype newtype;

	int count = 3;
	int blocklens[] = { 1,1,1 };

	MPI_Aint indices[3]; //C type that holds any valid address
	indices[0] = (MPI_Aint)offsetof(struct Troika, a);
	indices[1] = (MPI_Aint)offsetof(struct Troika, b);
	indices[2] = (MPI_Aint)offsetof(struct Troika, c);

	//MPI_Address(indata, &addresses[0]); ~ offsetof
	//MPI_Address(&(indata->a), &addresses[1]);
	//MPI_Address(&(indata->b), &addresses[2]);
	//MPI_Address(&(indata->n), &addresses[3]);
	//displacements[0] = addresses[1] - addresses[0]; = indices
	//displacements[1] = addresses[2] - addresses[0];
	//displacements[2] = addresses[3] - addresses[0];


	MPI_Datatype old_types[] = { MPI_INT,MPI_DOUBLE,MPI_INT };

	MPI_Type_create_struct(count, blocklens, indices, old_types, &newtype);
	MPI_Type_commit(&newtype);

	//int** Input = new int*[K];

	//if (rank == 0) {

	//	Input[0] = new int[3]{ 1, 2, 3 };
	//	Input[1] = new int[3]{ 4, 5, 6 };
	//	Input[2] = new int[3]{ 7, 8, 9 };
	//}
	//else {
	//	Input[0] = new int[3];
	//	Input[1] = new int[3];
	//	Input[2] = new int[3];
	//}

	Troika* Input = new Troika[K];

	if (rank == 0) {
		Input[0] = { 1, 2.2, 3 };
		Input[1] = { 4, 5.5, 6 };
		Input[2] = { 7, 8.8, 9 };
	}

	for (int i = 0; i < K; i++) {
		MPI_Bcast(&Input[i], 1, newtype, 0, MPI_COMM_WORLD);
		MPI_Barrier(MPI_COMM_WORLD);
	}

	for (int k = 0; k < size; k++) {
		if (k == rank) {
			cout << "Rank: " << rank << endl;
			for (int i = 0; i < K; i++)
			{
				cout << "Troika nomer " << i << ": " << endl;
				cout << "a: " << Input[i].a << "; b:" << Input[i].b << "; c:" << Input[i].c << "  \n";
			}
			cout << endl << endl;
		}
		MPI_Barrier(MPI_COMM_WORLD);
	}

	MPI_Finalize();
}

void Type8() {
	int rc = MPI_Init(NULL, NULL);
	if (rc != MPI_SUCCESS) {
		printf("MPI Error \n");
		MPI_Abort(MPI_COMM_WORLD, rc);
	}

	int rank, size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	struct Troika {
		int a,b;
		double c;
	};

	MPI_Datatype newtype;

	int count = 3;
	int blocklens[] = { 1,1,1 };

	MPI_Aint indices[3]; //C type that holds any valid address
	indices[0] = (MPI_Aint)offsetof(struct Troika, a);
	indices[1] = (MPI_Aint)offsetof(struct Troika, b);
	indices[2] = (MPI_Aint)offsetof(struct Troika, c);

	MPI_Datatype old_types[] = { MPI_INT,MPI_INT,MPI_DOUBLE };

	MPI_Type_create_struct(count, blocklens, indices, old_types, &newtype);
	MPI_Type_commit(&newtype);

	Troika* Input = new Troika[rank];

	if (rank != 0) {
		for (int i=0; i<rank; i++)
			Input[i] = { rank, rank, rank + 0.1 };
	}

	int overheadN = sizeof(Troika) * rank + MPI_BSEND_OVERHEAD;

	//int buffN = rank * sizeof(Troika) + MPI_BSEND_OVERHEAD;

	//void* Buffer = malloc(buffN);

	Troika* Buffer = new Troika[overheadN];

	int rSize = 0;
	for (int i = 1; i < size; i++)
		rSize += i;

	Troika *Receive = new Troika[rSize];

	switch (rank) {
	case 0:
		for (int r = 1; r < size; r++) {
			int currentPos = 0;
			for (int i = 1; i < r; i++)
				currentPos += i;
			MPI_Recv(Receive + currentPos, r, newtype, r, r, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}
		break;
	default:
		MPI_Buffer_attach(Buffer, overheadN);
		MPI_Bsend(Input, rank, newtype, 0, rank, MPI_COMM_WORLD);
		MPI_Buffer_detach(Buffer, &overheadN);
		break;
	}
	MPI_Barrier(MPI_COMM_WORLD);

	//int outputRank;
	int currentRank = 1; 
	if (rank == 0)
		for (int i = 0; i < rSize; i++) {
			cout << "i: " << i << endl;
			cout << Receive[i].a << " " << Receive[i].b << " " << Receive[i].c << endl;
		}

	MPI_Finalize();
}

void Type9() {
	int rc = MPI_Init(NULL, NULL);
	if (rc != MPI_SUCCESS) {
		printf("MPI Error \n");
		MPI_Abort(MPI_COMM_WORLD, rc);
	}

	int rank, size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int *InputI = new int[size];
	double *InputD = new double[size];

	if (rank == 0) {
		InputI = new int[size] {1, 2, 3, 4};
		InputD = new double[size] {1.5, 2.5, 3.5, 4.5};
	}

	const int buffN = sizeof(int)*size + sizeof(double)*size;
	char *buff = new char[buffN];

	if (rank == 0) {
		int position = 0;
		MPI_Pack(InputI, size, MPI_INT, buff, buffN, &position, MPI_COMM_WORLD);
		MPI_Pack(InputD, size, MPI_DOUBLE, buff, buffN, &position, MPI_COMM_WORLD);
	}

	MPI_Bcast(buff, buffN, MPI_PACKED, 0, MPI_COMM_WORLD);


	if (rank != 0) {
		int position = 0;
		MPI_Unpack(buff, buffN, &position, InputI, size, MPI_INT, MPI_COMM_WORLD);
		MPI_Unpack(buff, buffN, &position, InputD, size, MPI_DOUBLE, MPI_COMM_WORLD);
	}

	for (int k = 1; k < size; k++) {
		if (k == rank) {
			cout << "Rank: " << rank << endl << endl;
			cout << "Int: " << endl;
			for (int i = 0; i < size; i++)
			{
				cout << InputI[i] << " ";
			}
			cout << endl << endl;

			cout << "Double: " << endl;
			for (int i = 0; i < size; i++)
			{
				cout << InputD[i] << " ";
			}
			cout << endl << endl;
		}
		MPI_Barrier(MPI_COMM_WORLD);
	}

	MPI_Finalize();
}

void Type13() {

	int rc = MPI_Init(NULL, NULL);
	if (rc != MPI_SUCCESS) {
		printf("MPI Error \n");
		MPI_Abort(MPI_COMM_WORLD, rc);
	}

	int rank, size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	double InputD;
	int *InputI = new int[rank];

	if (rank != 0) {
		InputD = rank + 0.5;
		for (int i = 0; i < rank; i++)
			InputI[i] = rank;
	}

	const int buffN = sizeof(int)*rank + sizeof(double);
	char *buff = new char[buffN];

	int rSize = 0;
	for (int i = 1; i < size; i++)
		rSize += sizeof(int)*i + sizeof(double);
	char* Receive = new char[rSize + MPI_BSEND_OVERHEAD];

	if (rank != 0) {
		int position = 0;
		MPI_Pack(&InputD, 1, MPI_DOUBLE, buff, buffN, &position, MPI_COMM_WORLD);
		MPI_Pack(InputI, rank, MPI_INT, buff, buffN, &position, MPI_COMM_WORLD);
	}

	int currentPos = 0;
	switch (rank) {
	case 0:
		for (int r = 1; r < size; r++) {
			MPI_Recv(Receive + currentPos, rSize + MPI_BSEND_OVERHEAD, MPI_PACKED, r, r, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			currentPos += r*sizeof(int) + sizeof(double);
		}
		break;
	default:
		MPI_Send(buff, buffN, MPI_PACKED, 0, rank, MPI_COMM_WORLD);
		break;
	}

	if (rank == 0) {
		int position = 0;
		for (int i = 1; i < size; i++) {
			MPI_Unpack(Receive, buffN, &position, &InputD, 1, MPI_DOUBLE, MPI_COMM_WORLD);
			MPI_Unpack(Receive, buffN, &position, InputI, i, MPI_INT, MPI_COMM_WORLD);
			cout << "Rank:" << i << endl << endl;

			cout << "Double: " << endl;
			cout << InputD << " " << endl << endl;

			cout << "Int: " << endl;
			for (int j = 0; j < i; j++)
			{
				cout << InputI[j] << " ";
			}
			cout << endl << endl;
		}
	}


	MPI_Finalize();
}

void Type14() {
	int rc = MPI_Init(NULL, NULL);
	if (rc != MPI_SUCCESS) {
		printf("MPI Error \n");
		MPI_Abort(MPI_COMM_WORLD, rc);
	}

	int rank, size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int *A = nullptr;
	int *N = new int[size - 1]{ 4, 2 ,1 };

	if (rank == 0) {
		A = new int[3 * (size - 1)] {1, 2, 3, 4, 5, 6, 7, 8, 9};
	}

	MPI_Datatype newtype;
	MPI_Type_create_resized(MPI_INT, 0, 2 * sizeof(int), &newtype);
	MPI_Type_commit(&newtype);

	MPI_Status stat;
	int *Receive = nullptr;
	if (rank != 0)
		Receive = new int[ N[rank - 1] ];
	int count = 0;

	switch (rank) {
	case 0:
		for (int r = 1; r < size; r++)
			MPI_Send(A + (r - 1), N[r - 1], newtype, r, r, MPI_COMM_WORLD);
		break;
	default:
		MPI_Probe(0, rank, MPI_COMM_WORLD, &stat);
		MPI_Get_count(&stat, newtype, &count);
		MPI_Recv(Receive, count, MPI_INT, 0, rank, MPI_COMM_WORLD, &stat);
		break;
	}

	for (int k = 1; k < size; k++) {
		if (k == rank) {
			cout << "Rank: " << rank << endl << endl;
			for (int i = 0; i < count; i++)
			{
				cout << Receive[i] << " ";
			}
			cout << endl << endl;
		}
		MPI_Barrier(MPI_COMM_WORLD);
	}

	MPI_Finalize();
}

int main()
{
    Type1();
	Type5();
	Type8();
	Type9();
	Type13();
    return 0;
}