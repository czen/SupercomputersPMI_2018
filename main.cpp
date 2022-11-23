#define MATRIX_SIZE 1440

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <omp.h>
#include <cmath>
#include <assert.h>
#include <chrono>

#include "SymmetrMatrix.h"
#include "VTMatrix.h"
#include "ResultMatrix.h"

auto blocks = { 6, 8, 9, 10, 12, 15, 16, 18, 20, 24, 30,
32, 36, 40, 45, 48, 60, 72, 80, 90, 96,  120,  144,  160,  180,
240,  288,  360,  480,  720, 1440 };

std::string matrixFile(int blockLen, std::string fname)
{
	return  std::to_string(blockLen)+ fname + ".txt";
}
//Перемножение 2-ух блоков
void multiplyBlocks(int *A, int *B, int *C, int block_size)
{
	for (int i = 0; i < block_size; i++)
		for (int j = 0; j < block_size; j++)
			for (int k = 0; k < block_size; k++)
				C[i * block_size + j] += A[i * block_size + k] * B[k * block_size + j];
}
//Параллельное перемножение 2-ух блоков
void multiplyBlocksParallel(int *A, int *B, int *C, int block_size)
{
#pragma omp parallel for 
	for (int i = 0; i < block_size; i++)
		for (int j = 0; j < block_size; j++)
			for (int k = 0; k < block_size; k++)
				C[i * block_size + j] += A[i * block_size + k] * B[k * block_size + j];
}
//Последовательное умножение 
void serialMultiplication(VTMatrix *A, SymmetrMatrix *B, ResultMatrix *C)
{
	int blocks_on_line = C->getWidthMatrixInBlocks();

	for (int i = 0; i < blocks_on_line; i++)
		for (int j = 0; j < blocks_on_line; j++)
			for (int k = 0; k < blocks_on_line; k++)
				multiplyBlocks(A->getBlock(i, k), B->getBlock(k, j), C->getBlock(i, j), C->getBlockWidth());
}

// Параллельное умножение внутри блоков
void InBlockParallelMultiplication(VTMatrix *A, SymmetrMatrix *B, ResultMatrix *C)
{
	int block_quantity = C->getWidthMatrixInBlocks();

	for (int i = 0; i < block_quantity; i++)
		for (int j = 0; j < block_quantity; j++)
			for (int k = 0; k < block_quantity; k++)
				multiplyBlocksParallel(A->getBlock(i, k), B->getBlock(k, j), C->getBlock(i, j), C->getBlockWidth());
}

// Параллельное умножение нескольких пар блоков
void PairParallelMultiplication(VTMatrix *A, SymmetrMatrix *B, ResultMatrix *C)
{
	int block_quantity = C->getWidthMatrixInBlocks();

#pragma omp parallel for 
	for (int i = 0; i < block_quantity; i++)
		for (int j = 0; j < block_quantity; j++)
			for (int k = 0; k < block_quantity; k++)
				multiplyBlocks(A->getBlock(i, k), B->getBlock(k, j), C->getBlock(i, j), C->getBlockWidth());
}


int main()
{
	double start_time[3], end_time[3];
	setlocale(LC_ALL, "");
	//int blockLen = 5;
	for (auto blockLen : blocks)
	{
	std::cout << "Длина блока: " << blockLen << std::endl;

	VTMatrix *A = new VTMatrix(MATRIX_SIZE, blockLen);
	A->generateMatrix(1, 9);
	//A->printMatrix();
	
	SymmetrMatrix *B = new SymmetrMatrix(MATRIX_SIZE, blockLen);
	B->generateMatrix(1, 9);
	//B->printMatrix();

	ResultMatrix *C = new ResultMatrix(MATRIX_SIZE, blockLen);
	C->zerofication();
	auto start = std::chrono::system_clock::now();
	serialMultiplication(A, B, C);
	auto end = std::chrono::system_clock::now();
	//C->printMatrix();
	std::chrono::duration<double> elapsed = end - start;

	std::cout << "Последовательный: " << elapsed.count() << std::endl;
	C->zerofication();

	start_time[0] = omp_get_wtime();
	InBlockParallelMultiplication(A, B, C);
	end_time[0] = omp_get_wtime();
	std::cout << "2 блока: " << end_time[0] - start_time[0] << std::endl;
	C->zerofication();

	start_time[1] = omp_get_wtime();
	PairParallelMultiplication(A, B, C);
	end_time[1] = omp_get_wtime();
	std::cout << "2 разных блока: " << end_time[1] - start_time[1] << std::endl;
	C->zerofication();
	
	delete A;
	delete B;
	delete C;
	std::cout << "===============================" << std::endl;
	}
	system("pause");
	return 0;
}