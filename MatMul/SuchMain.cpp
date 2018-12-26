#include "omp.h"
#include <iostream>
#include <algorithm>
#include <cstdlib>
using namespace std;

#define THREADS_NUM 8
//
#define BSIZE 64; // должен извлекаться корень, программа за этим не следит, будет блок sqrt(BSIZE) x sqrt(BSIZE)
#define MN 2048;

void nullify(int *C, int n) {
	for (int i = 0; i < n*n; i++) {
		C[i] = 0;
	}
}

void MatrixMultBlock1(int* A, int* B, int* C, int n) {
	int bSize = BSIZE;

	// unparalleled
	nullify(C, n);
	int sqrtBSize = (int)sqrt(bSize);
	int blocksInRow = (int)(n / sqrtBSize);
	double t1 = omp_get_wtime();
	for (int blockNumber = 0; blockNumber < n*n / bSize; blockNumber++) {
		for (int realBlockNumber = 0; realBlockNumber < blockNumber / blocksInRow + 1; realBlockNumber++) {
			for (int i = 0; i < sqrtBSize; i++) {
				for (int j = 0; j < sqrtBSize; j++) {
					for (int k = 0; k < sqrtBSize; k++) {
						C[bSize*blockNumber + sqrtBSize * i + j] +=
							A[bSize*(blockNumber / blocksInRow)*blocksInRow + bSize * realBlockNumber + sqrtBSize * i + k] *
							B[bSize*realBlockNumber*blocksInRow + bSize * (blockNumber % blocksInRow) + sqrtBSize * k + j];
					}
				}
			}
		}
	}
	double t2 = omp_get_wtime();
	double nptime = (t2 - t1) * 1000;
	cout << "Non-parallel time: " << nptime << endl;

	//paralleled 2
	nullify(C, n);
	t1 = omp_get_wtime();
#pragma omp parallel num_threads(THREADS_NUM)
	{
#pragma omp master
		{
			cout << "num_threads: " << omp_get_num_threads() << endl;
		}

#pragma omp for
		for (int blockNumber = 0; blockNumber < n*n / bSize; blockNumber++) {
			for (int realBlockNumber = 0; realBlockNumber < blockNumber / blocksInRow + 1; realBlockNumber++) {
				for (int i = 0; i < sqrtBSize; i++) {
					for (int j = 0; j < sqrtBSize; j++) {
						for (int k = 0; k < sqrtBSize; k++) {
							C[bSize*blockNumber + sqrtBSize * i + j] +=
								A[bSize*(blockNumber / blocksInRow)*blocksInRow + bSize * realBlockNumber + sqrtBSize * i + k] *
								B[bSize*realBlockNumber*blocksInRow + bSize * (blockNumber % blocksInRow) + sqrtBSize * k + j];
						}
					}
				}
			}
		}
	}
	t2 = omp_get_wtime();
	cout << "Total parallel time of 2 algotirhm: " << (t2 - t1) * 1000 << endl;
}

void MatrixMultBlock2(int* A, int* B, int* C, int n) {
	int bSize = BSIZE;

	int sqrtBSize = (int)sqrt(bSize);
	int blocksInRow = (int)(n / sqrtBSize);
	double t1;
	double t2;

	//paralleled 1
	nullify(C, n);
	int partThread = blocksInRow / THREADS_NUM;
	if (partThread == 0) {
		partThread = 1;
		cout << "Too many threads in 1 algorithm" << endl;
	}
	t1 = omp_get_wtime();

#pragma omp parallel num_threads(THREADS_NUM)
	{
		for (int blockNumber = 0; blockNumber < n*n / bSize; blockNumber++) {
			for (int realBlockNumber = partThread * omp_get_thread_num(); (realBlockNumber < blockNumber / blocksInRow + 1) &&
				(realBlockNumber < partThread*(1 + omp_get_thread_num())); realBlockNumber++) {
				for (int i = 0; i < sqrtBSize; i++) {
					for (int j = 0; j < sqrtBSize; j++) {
						for (int k = 0; k < sqrtBSize; k++) {
							C[bSize*blockNumber + sqrtBSize * i + j] +=
								A[bSize*(blockNumber / blocksInRow)*blocksInRow + bSize * realBlockNumber + sqrtBSize * i + k] *
								B[bSize*realBlockNumber*blocksInRow + bSize * (blockNumber % blocksInRow) + sqrtBSize * k + j];
						}
					}
				}
			}
		}
	}
	t2 = omp_get_wtime();
	cout << "Total parallel time of 1 algotirhm: " << (t2 - t1) * 1000 << endl;
}

void CreateMatrix(int*A, int*B, int n) {
	for (int i = 0; i < n; i++) {
		for (int j = n * i; j < n * i + n; j++) {
			A[j] = rand() % 100;
			B[j] = rand() % 100;
		}
	}
}

void MatrixMult(int* A, int* B, int* C, int n) {
	cout << "Elements in matrix: " << n << 'x' << n << ", " << n * n << endl;
	nullify(C, n);
	// считаем, что матрицы заданы не блочно, все равно только время меряем
	double t1 = omp_get_wtime();
	for (int j = 0; j < n; j++)
		for (int i = j; i < n; i++)
			for (int k = 0; k < n; k++)
				C[i*n + j] += A[i*n + k] * B[j + k * n];
	double t2 = omp_get_wtime();
	double nptime = (t2 - t1) * 1000;
	cout << "Non-parallel non-blocked time: " << nptime << endl;
}

int main()
{
	int n = MN;
	int* A = new int[n*n];
	int* B = new int[n*n];
	int* C = new int[n*n];
	CreateMatrix(A, B, n);
	MatrixMult(A, B, C, n);
	MatrixMultBlock1(A, B, C, n);
	MatrixMultBlock2(A, B, C, n);
	cout << "Cin button" << endl;
	cin >> n;
	return 0;
}