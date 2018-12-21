#include "mpi.h"
#include <iostream>
#include "stddef.h"
#include <windows.h>
#include <algorithm>
#include <fstream>
using namespace std;

// В каждом из процессов, входящих в коммуникатор MPI_COMM_WORLD, прочесть одно вещественное число X и вывести его противоположное значение ?X. 
//Для ввода и вывода данных использовать поток ввода-вывода pt, определенный в задачнике. Кроме того, отобразить значение ?X в разделе отладки, 
//используя функцию Show, также определенную в задачнике.
void MPI1Proc1() { // готово
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


//В каждом подчиненном процессе дано целое число N (0 < N < 5) и набор из N целых чисел. Переслать данные наборы в главный процесс, 
//используя по одному вызову функции MPI_Bsend для каждого передающего процесса, и вывести наборы в главном процессе в порядке возрастания 
//рангов переславших их процессов. Для определения размера пересланного набора использовать функцию MPI_Get_count. 
void MPI2Send4() { //готово
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

//В главном процессе дан набор вещественных чисел; количество чисел равно количеству подчиненных процессов.С помощью функции MPI_Bsend 
//переслать по одному числу в каждый из подчиненных процессов, перебирая процессы в обратном порядке(первое число в последний процесс, 
//второе — в предпоследний процесс, и т.д.), и вывести в подчиненных процессах полученные числа.
void MPI2Send6() { //готово
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
//В каждом процессе даны два целых числа.С помощью функций MPI_Ssend и MPI_Recv переслать первое число в предыдущий процесс, 
//а второе — в последующий процесс(для процесса 0 считать предыдущим последний процесс, а для последнего процесса считать последующим процесс 0).
//В каждом процессе вывести числа, полученные от предыдущего и последующего процесса(в указанном порядке).
//Указание. Функция MPI_Ssend обеспечивает синхронный режим пересылки данных, при котором операция отправки сообщения будет завершена только после 
//начала приема этого сообщения процессом - получателем.В случае пересылки данных в синхронном режиме возникает опасность взаимных блокировок(deadlocks) 
//из - за неправильного порядка вызова функций отправки и получения сообщений.
void MPI2Send13() { // готово

	//const int N = 8;
	const int N = 10;
	//int* Input = new int[N] { 3, 1, 0, 2, 1, 3, 2, 0};
	int* Input = new int[N] { 4, 1, 0, 2, 1, 3, 2, 4, 3, 0};
	//int* Input = new int[N] { 9,2,1,4,3,6,5,8,7,0};
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

//В каждом процессе даны два числа : вещественное A и целое N, причем набор чисел N содержит все значения от 0 до K ? 1, где K — количество процессов.
//Используя функции MPI_Send и MPI_Recv(с параметром MPI_ANY_SOURCE), выполнить в каждом процессе пересылку числа A в процесс N и вывести полученное число, 
//а также ранг процесса, из которого число было получено.
void MPI2Send15() { // готово

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
//В главном процессе дан набор из 5 чисел. Используя функцию MPI_Bcast, 
//переслать этот набор во все подчиненные процессы и вывести в них полученные числа в том же порядке. 
void MPI3Coll2() { // готово

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



//В главном процессе дан набор из K + 2 чисел, где K — количество процессов. Используя функцию MPI_Scatterv, переслать в каждый процесс три числа из 
//данного набора; при этом в процесс ранга R должны быть пересланы числа с номерами от R + 1 до R + 3 (в процесс 0 — первые три числа, в процесс 1 — числа 
//со второго по четвертое, и т. д.). В каждом процессе вывести полученные числа.
void MPI3Coll10() { // готово
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


//В каждом процессе дан набор из 3K целых чисел, где K — количество процессов. Используя функцию MPI_Alltoall, переслать в каждый процесс три 
//очередных числа из каждого набора (в процесс 0 — первые три числа, в процесс 1 — следующие три числа, и т. д.). В каждом процессе вывести числа 
//в порядке возрастания рангов переславших их процессов (включая числа, полученные из этого же процесса).
void MPI3Coll15() { // готово
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






//В каждом подчиненном процессе дан набор из одного вещественного и R целых чисел, где значение R равно рангу процесса 
//(в процессе 1 дано одно целое число, в процессе 2 — два целых числа, и т. д.). Используя функции упаковки и одну функцию 
//передачи и приема, переслать все данные в главный процесс и вывести эти данные в порядке возрастания рангов переславших их процессов.
void MPI4Type13() { // готово

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


//В главном процессе даны два набора целых чисел: A размера 3K и N размера K, где K — количество подчиненных процессов. Считая, 
//что элементы наборов нумеруются от 1, переслать и вывести в каждом подчиненном процессе ранга R (R = 1, 2, …, K) NR элементов из набора A, 
//начиная с элемента AR и перебирая их через один (например, если N2 равно 3, то в процесс ранга 2 надо переслать элементы A2, A4, A6). 
//Использовать для пересылки каждого набора элементов по одному вызову функций MPI_Send, MPI_Probe и MPI_Recv, причем функция MPI_Recv 
//должна возвращать массив, содержащий только те элементы, которые требуется вывести. Для этого в главном процессе определить новый тип, 
//содержащий единственный целочисленный элемент и дополнительный конечный пустой промежуток, равный размеру элемента целого типа. 
//Использовать в функции MPI_Send исходный массив A с требуемым смещением, указав в качестве второго параметра количество элементов, 
//равное NR, а в качестве третьего параметра — новый тип. В функции MPI_Recv использовать целочисленный массив размера NR и тип MPI_INT. 
//Для определения количества NR переданных элементов использовать в подчиненных процессах функцию MPI_Get_count.
//Указание.Для задания завершающего пустого промежутка при определении нового типа в MPI - 2 следует использовать функцию 
//MPI_Type_create_resized(в данном случае эту функцию надо применить к типу MPI_INT).В MPI - 1 надо использовать метку нулевого размера 
//типа MPI_UB совместно с функцией MPI_Type_struct(в стандарте MPI - 2 тип MPI_UB объявлен устаревшим).
void MPI4Type14() { //готово
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


//В каждом процессе нечетного ранга (1, 3, …) даны два вещественных числа. С помощью функций MPI_Comm_group, MPI_Group_excl и 
//MPI_Comm_create создать новый коммуникатор, включающий процессы нечетного ранга. Используя одну коллективную операцию пересылки 
//данных для созданного коммуникатора, переслать исходные числа во все процессы нечетного ранга и вывести эти числа в порядке возрастания 
//рангов переславших их процессов (включая числа, полученные из этого же процесса).
void MPI5Comm2() { // готово
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







//В главном процессе дано целое число N (> 1), причем известно, что количество процессов K делится на N. Переслать число N во все процессы, 
//после чего, используя функцию MPI_Cart_create, определить для всех процессов декартову топологию в виде двумерной решетки — матрицы размера 
//N ? K/N (порядок нумерации процессов оставить прежним). Используя функцию MPI_Cart_coords, вывести для каждого процесса его координаты в 
//созданной топологии.
void MPI5Comm13() { // готово
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



int main()
{	
	/*MPI1Proc1[1]
	MPI1Proc8[1]
	MPI2Send4[1]
	MPI2Send6[1]
	MPI2Send10[1]
	MPI2Send13[1]
	MPI2Send15[1]
	MPI3Coll2[1]
	MPI3Coll7[1]
	MPI3Coll10[1]
	MPI3Coll13[1]
	MPI3Coll15[1]
	MPI3Coll16[2]
	MPI4Type3[1]
	MPI4Type4[1]
	MPI4Type7[1]
	MPI4Type11[1]
	MPI4Type13[1]
	MPI4Type14[2]
	MPI5Comm2[1]
	MPI5Comm5[1]
	MPI5Comm6[1]
	MPI5Comm9[2]
	MPI5Comm11[2]
	MPI5Comm13[1]
	MPI5Comm16[1]
	MPI5Comm22[1]*/
return 0;
}
