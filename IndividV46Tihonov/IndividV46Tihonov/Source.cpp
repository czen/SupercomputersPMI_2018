#include <time.h>
#include <omp.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <fstream>
#include <vector>
#include <ctime>
using namespace std;
int ThreadNum = 4; // для корректной работы должен находиться корень
int MatrixSize = 1440;
int VBlockSize = MatrixSize;// в ходе работы меняется

int CountBlocks = MatrixSize / VBlockSize;

vector<int> getVblockSize(int MatrixSize, string file_block_size) {
	vector<int> vec;
	for (int i = 1; i <= MatrixSize; i++) {
		if ((i % 2 == 0) && (MatrixSize%i == 0)) vec.push_back(i);
	}
	ofstream file(file_block_size);
	file << vec.size() << " ";
	for (int i : vec) {
		file << i << " ";
	}
	file.close();
	return vec;
}
 
// Function for simple initialization of matrix elements
void Dummy_Data_Initialization(double* pAMatrix, double* pBMatrix, int Size) {
	int i, j; // Loop variables
	for (i = 0; i < Size; i++)
		for (j = 0; j < Size; j++) {
			pAMatrix[i*Size + j] = 1;
			pBMatrix[i*Size + j] = 1;
		}
}
// Function for random initialization of matrix elements
void Random_Data_Initialization(double* pAMatrix, double* pBMatrix,
	int Size) {
	int i, j; // Loop variables
	srand(unsigned(clock()));
	for (i = 0; i < Size; i++)
		for (j = 0; j < Size; j++) {
			pAMatrix[i*Size + j] = rand() / double(1000);
			pBMatrix[i*Size + j] = rand() / double(1000);
		}
}
// Function for memory allocation and initialization of matrix elements
void Process_Initialization(double* &pAMatrix, double* &pBMatrix,
	double* &pCMatrix, int &Size) {
	Size = MatrixSize;
	// Memory allocation
	pAMatrix = new double[Size*Size];
	pBMatrix = new double[Size*Size];
	pCMatrix = new double[Size*Size];
	// Initialization of matrix elements
	Dummy_Data_Initialization(pAMatrix, pBMatrix, Size);
	for (int i = 0; i < Size*Size; i++) {
		pCMatrix[i] = 0;
	}
}
// Function for formatted matrix output
void Print_Matrix(double* pMatrix, int RowCount, int ColCount) {
	int i, j; // Loop variables
	for (i = 0; i < RowCount; i++) {
		for (j = 0; j < ColCount; j++)
			printf("%7.4f ", pMatrix[i*RowCount + j]);
		printf("\n");
	}
}
// Function for matrix multiplication
void Serial_Result_Calculation(double* pAMatrix, double* pBMatrix,
	double* pCMatrix, int Size) {
	int i, j, k; // Loop variables
	for (i = 0; i < Size; i++) {
		for (j = 0; j < Size; j++)
			for (k = 0; k < Size; k++)
				pCMatrix[i*Size + j] += pAMatrix[i*Size + k] * pBMatrix[k*Size + j];
	}
}
// Function for computational process termination
void Process_Termination(double* pAMatrix, double* pBMatrix,
	double* pCMatrix) {
	delete[] pAMatrix;
	delete[] pBMatrix;
	delete[] pCMatrix;
}

void Parallel_Result_Calculation(double* pAMatrix, double* pBMatrix,
	double* pCMatrix, int Size) {
	int GridSize = int(sqrt((double)ThreadNum));
	int BlockSize = Size / GridSize;
	
	omp_set_num_threads(ThreadNum);
#pragma omp parallel 
	{
		int ThreadID = omp_get_thread_num();
		int RowIndex = ThreadID / GridSize;
		int ColIndex = ThreadID % GridSize;
		for (int iter = 0; iter < GridSize; iter++) {
			for (int i = RowIndex * BlockSize; i < (RowIndex + 1)*BlockSize; i++)
				for (int j = ColIndex * BlockSize; j < (ColIndex + 1)*BlockSize; j++)
					for (int k = iter * BlockSize; k < (iter + 1)*BlockSize; k++)
						pCMatrix[i*Size + j] += pAMatrix[i*Size + k] * pBMatrix[k*Size + j];
		}
	} // pragma omp parallel
}

// Serial block matrix mutiplication
void Optimal_Result_Calculation(double* pAMatrix, double* pBMatrix, double*
	pCMatrix, int Size) {
	int BlockSize = VBlockSize;
	int GridSize = int(Size / double(BlockSize));
	for (int n = 0; n < GridSize; n++)
		for (int m = 0; m < GridSize; m++)
			for (int iter = 0; iter < GridSize; iter++)
				for (int i = n * BlockSize; i < (n + 1)*BlockSize; i++)
					for (int j = m * BlockSize; j < (m + 1)*BlockSize; j++)
						for (int k = iter * BlockSize; k < (iter + 1)*BlockSize; k++)
							pCMatrix[i*Size + j] +=
							pAMatrix[i*Size + k] * pBMatrix[k*Size + j];
}

void Parallel_Optimal_Result_Calculation(double* pAMatrix, double* pBMatrix,
	double* pCMatrix, int Size) {
	int BlockSize = VBlockSize;
	int GridSize = int(Size / double(BlockSize));
	for (int n = 0; n < GridSize; n++)
#pragma omp parallel for
		for (int m = 0; m < GridSize; m++)
			for (int iter = 0; iter < GridSize; iter++)
				for (int i = n * BlockSize; i < (n + 1)*BlockSize; i++)
					for (int j = m * BlockSize; j < (m + 1)*BlockSize; j++)
						for (int k = iter * BlockSize; k < (iter + 1)*BlockSize; k++)
							pCMatrix[i*Size + j] +=	pAMatrix[i*Size + k] * pBMatrix[k*Size + j];
}


void main() {
	double* pAMatrix; // The first argument of matrix multiplication
	double* pBMatrix; // The second argument of matrix multiplication
	double* pCMatrix; // The result matrix

	int Size; // Size of matricies
	
	// Memory allocation and initialization of matrix elements
	Process_Initialization(pAMatrix, pBMatrix, pCMatrix, Size);
	printf(" Memory allocation and initialization нof matrix elements:ACK\n");
	
	/*experiment the first. Run-time dependence on block size for different algorithms
	Размер матрицы статичен (должен быть четным)
	Размеры блоков меняются (размер матрицы % размер блока=0);
	*/
	{
		
	//Файлы результатов измерений
		ofstream result_serial("result_serial.txt");
		ofstream result_serial_block("result_serial_block.txt");
		ofstream result_parallel_block("result_parallel_block.txt");
		ofstream result_parallel_optimal_block("result_parallel_optimal.txt");

		double start_time = clock();
		Serial_Result_Calculation(pAMatrix, pBMatrix, pCMatrix, Size);
		double end_time = clock();
		result_serial << end_time - start_time;
		printf(" Serial_Result_Calculation:ACK\n");

		start_time = clock();
		Parallel_Result_Calculation(pAMatrix, pBMatrix, pCMatrix, Size);
		end_time = clock();
		result_parallel_block << end_time - start_time;
		printf("Parallel_Result_Calculation:ACK\n");

		vector<int> vec = getVblockSize(MatrixSize, "file_block_size.txt");
		
		for (int i : vec) {
			VBlockSize = i;

			start_time = clock();
			Optimal_Result_Calculation(pAMatrix, pBMatrix, pCMatrix, Size);
			end_time = clock();
			result_serial_block << end_time - start_time << " ";

			printf("Optimal_Result_Calculation for block %i:ACK\n",i);

			start_time = clock();
			Parallel_Optimal_Result_Calculation(pAMatrix, pBMatrix, pCMatrix, Size);
			end_time = clock();
			result_parallel_optimal_block << end_time - start_time << " ";
			printf("Parallel_Optimal_Result_Calculation for block %i:ACK\n",i);
		}
		result_serial.close();
		result_serial_block.close();
		result_parallel_block.close();
		result_parallel_optimal_block.close();
	
	
	}
	Process_Termination(pAMatrix, pBMatrix, pCMatrix);
	  	  
	system("pause");
}