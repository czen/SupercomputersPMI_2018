#include "omp.h"
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iterator>
#include <cassert>
using namespace std;

#define THREADS_NUM 8
#define BSIZE 4;
#define MN 2048;

void copyThat(int *src, int* dest, int n)
{
	for (int i = 0; i < n*n; i++)
		dest[i] = src[i];
}

void nullify(int *C, int n) {
	for (int i = 0; i < n*n; i++) {
		C[i] = 0;
	}
}

void CreateMatrix(int*A, int*B, int n) {
	for (int j = 0; j < n; j++) {
		for (int i = j; i < n; i++) {
			A[i*n + j] = rand() % 100;
			B[i*n + j] = rand() % 100;
		}
	}
	for (int i = 0; i < n; i++) {
		for (int j = i + 1; j < n; j++) {
			A[i*n + j] = 0;
			B[i*n + j] = 0;
		}
	}
}

int* EncodeMatrix(int*A, int n) {
	int bSize = BSIZE;
	int sqrtBSize = (int)sqrt(bSize);
	int blocksInRow = (int)(n / sqrtBSize);
	int* NewArr = new int[n*n];
	for (int blockNumber = 0; blockNumber < n*n / bSize; blockNumber++) {
		for (int i = 0; i < sqrtBSize; i++) {
			for (int j = 0; j < sqrtBSize; j++) {
				NewArr[bSize*blockNumber + sqrtBSize * i + j] = A[blockNumber / blocksInRow * sqrtBSize * n + blockNumber % blocksInRow * sqrtBSize + n * i + j];
			}
		}
	}
	delete[]A;
	return NewArr;
}

int* DecodeMatrix(int*A, int n) {
	int bSize = BSIZE;
	int sqrtBSize = (int)sqrt(bSize);
	int blocksInRow = (int)(n / sqrtBSize);
	int* NewArr = new int[n*n];
	for (int blockNumber = 0; blockNumber < n*n / bSize; blockNumber++) {
		for (int i = 0; i < sqrtBSize; i++) {
			for (int j = 0; j < sqrtBSize; j++) {
				NewArr[blockNumber / blocksInRow * sqrtBSize * n + blockNumber % blocksInRow * sqrtBSize + n * i + j] = A[bSize*blockNumber + sqrtBSize * i + j];
			}
		}
	}
	delete[]A;
	return NewArr;
}

int* MatrixMult(int* A, int* B, int n) {
	cout << "Elements in matrix: " << n << 'x' << n << ", " << n * n << endl;
	int* NewArr = new int[n*n];
	nullify(NewArr, n);
	double t1 = omp_get_wtime();
	for (int j = 0; j < n; j++)
		for (int i = j; i < n; i++)
			for (int k = 0; k < n; k++)
				NewArr[i*n + j] += A[i*n + k] * B[j + k * n];
	double t2 = omp_get_wtime();
	double nptime = (t2 - t1) * 1000;
	cout << "Non-parallel non-blocked time: " << nptime << endl;
	return NewArr;
}

int* MatrixMultBlock(int* A, int* B, int n) {
	int bSize = BSIZE;
	int* NewArr = new int[n*n];
	nullify(NewArr, n);
	int sqrtBSize = (int)sqrt(bSize);
	int blocksInRow = (int)(n / sqrtBSize);
	double t1 = omp_get_wtime();
	for (int blockNumber = 0; blockNumber < n*n / bSize; blockNumber++) {
		for (int realBlockNumber = blockNumber % blocksInRow; realBlockNumber < blockNumber / blocksInRow + 1; realBlockNumber++) {
			for (int i = 0; i < sqrtBSize; i++) {
				for (int j = 0; j < sqrtBSize; j++) {
					for (int k = 0; k < sqrtBSize; k++) {
						NewArr[bSize*blockNumber + sqrtBSize * i + j] +=
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
	return NewArr;
}

int* MatrixMultBlockPar1(int* A, int* B, int n) {
	int bSize = BSIZE;

	int sqrtBSize = (int)sqrt(bSize);
	int blocksInRow = (int)(n / sqrtBSize);
	int numberOfBlocks = n * n / bSize;
	double t1;
	double t2;

	//paralleled 1
	int* NewArr = new int[n*n];
	nullify(NewArr, n);
	int partThread = numberOfBlocks / THREADS_NUM;
	if (partThread == 0) {
		partThread = 1;
		cout << "Too many threads in 1 algorithm" << endl;
	}
	t1 = omp_get_wtime();
#pragma omp parallel num_threads(THREADS_NUM)
	{
#pragma omp master
		{
			cout << "num_threads: " << omp_get_num_threads() << endl;
		}
		if (omp_get_thread_num() / numberOfBlocks == 0) {
			for (int blockNumber = partThread * omp_get_thread_num(); blockNumber < partThread*(1 + omp_get_thread_num()); blockNumber++) {
				for (int realBlockNumber = blockNumber % blocksInRow; realBlockNumber < blockNumber / blocksInRow + 1; realBlockNumber++) {
					for (int i = 0; i < sqrtBSize; i++) {
						for (int j = 0; j < sqrtBSize; j++) {
							for (int k = 0; k < sqrtBSize; k++) {
								NewArr[bSize*blockNumber + sqrtBSize * i + j] +=
									A[bSize*(blockNumber / blocksInRow)*blocksInRow + bSize * realBlockNumber + sqrtBSize * i + k] *
									B[bSize*realBlockNumber*blocksInRow + bSize * (blockNumber % blocksInRow) + sqrtBSize * k + j];
							}
						}
					}
				}
			}
		}
	}
	t2 = omp_get_wtime();
	cout << "Total parallel time of 1 algotirhm: " << (t2 - t1) * 1000 << endl;
	return NewArr;
}

int* MatrixMultBlockPar2(int* A, int* B, int n) {
	int bSize = BSIZE;

	int sqrtBSize = (int)sqrt(bSize);
	int blocksInRow = (int)(n / sqrtBSize);
	double t1;
	double t2;

	//paralleled 2
	int* NewArr = new int[n*n];
	nullify(NewArr, n);
	t1 = omp_get_wtime();
#pragma omp parallel num_threads(THREADS_NUM)
	{
#pragma omp for
		for (int blockNumber = 0; blockNumber < n*n / bSize; blockNumber++) {
			for (int realBlockNumber = blockNumber % blocksInRow; realBlockNumber < blockNumber / blocksInRow + 1; realBlockNumber++) {
				for (int i = 0; i < sqrtBSize; i++) {
					for (int j = 0; j < sqrtBSize; j++) {
						for (int k = 0; k < sqrtBSize; k++) {
							NewArr[bSize*blockNumber + sqrtBSize * i + j] +=
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
	return NewArr;
}

int main()
{
	int n = MN;
	int* A = new int[n*n];
	int* B = new int[n*n];
	int* C = new int[n*n];
	CreateMatrix(A, B, n);

	int *CC = MatrixMult(A, B, n);

	A = EncodeMatrix(A, n);
	B = EncodeMatrix(B, n);

	int* M1 = MatrixMultBlock(A, B, n);
	int* M2 = MatrixMultBlockPar1(A, B, n);
	int* M3 = MatrixMultBlockPar2(A, B, n);
	for (int i = 0; i < n*n; i++)
		assert(M1[i] == M2[i] && M2[i] == M3[i]);

	C = DecodeMatrix(M3, n);

	for (int i = 0; i < n*n; i++) {
		assert(CC[i] == C[i]);
	}

	system("pause");
	return 0;
}