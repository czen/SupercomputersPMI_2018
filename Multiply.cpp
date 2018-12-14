#include "stdafx.h"
#include <iostream>
//функция умножения матриц из нашего вида
void multParallel(trmat& left, trmat& right, block_matrix& result)
{

	for (int k = 0; k< left.size; k++)
		for (int i = 1; i < left.blocksize; i++)
			for (int j = 0; j < i; j++)
				left.colomn[k].blocks[0].value[i*left.blocksize + j] = left.colomn[k].blocks[0].value[j*left.blocksize + i];
	//вызов ф-ии умножения двух блоков
	multParallelBlock(left, right, result);

	for (int k = 0; k < left.size; k++)
		for (int i = 1; i < left.blocksize; i++)
			for (int j = 0; j < i; j++)
				left.colomn[k].blocks[0].value[i*left.blocksize + j] = 0;
}

void multParallelBlock(trmat& left, trmat& right, block_matrix& result)
{
//4 потока параллельно умножают блоки
#pragma omp parallel for shared(left, right, result) num_threads(4)
	for (int i1 = 0; i1 < left.size; i1++)
	{
		for (int j = 0, j1 = left.size - 1; j < left.size; j++, j1--)
		{
			for (int k = 0, k1 = j; k <= j; k++, k1--)
			{
				//оформление првильной индексации
				if (i1 > k)
				{
					int *LT = new int[left.blocksize*left.blocksize];
					for (int i = 0; i < left.blocksize; i++)
						for (int j = 0; j < left.blocksize; j++)
							LT[i*left.blocksize + j] = left.colomn[k].blocks[i1 - k].value[i + j*left.blocksize];
					multTwoBlockParallel(LT, right.colomn[k].blocks[j - k].value, result.colomn[i1].blocks[j].value, left.blocksize);
				}
				else
					multTwoBlockParallel(left.colomn[i1].blocks[k - i1].value, right.colomn[k].blocks[j - k].value, result.colomn[i1].blocks[j].value, left.blocksize);

			}
		}
	}
}


void multTwoBlockParallel(int *A, int *B, int *C, int n)
{
	int *BT = new int[n*n];
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			BT[i*n + j] = B[i + j*n];
#pragma omp parallel num_threads(4), shared(A, B,n,C)
	{
		int i, j, k;
#pragma omp for
		for (i = 0; i < n; i++)
			for (j = 0; j < n; j++)
			{
				int dot = 0;
				for (k = 0; k < n; k++)
					dot += A[i*n + k] * BT[j*n + k];
				C[i*n + j] += dot;
			}
	}
	delete[] BT;
}
//обычное умножение
void multNonParallel(trmat& left, trmat& right, block_matrix& result)
{
	for (int k = 0; k< left.size; k++)
		for (int i = 1; i < left.blocksize; i++)
			for (int j = 0; j < i; j++)
				left.colomn[k].blocks[0].value[i*left.blocksize + j] = left.colomn[k].blocks[0].value[j*left.blocksize + i];
	multNonParallelBlock(left, right, result);
	for (int k = 0; k < left.size; k++)
		for (int i = 1; i < left.blocksize; i++)
			for (int j = 0; j < i; j++)
				left.colomn[k].blocks[0].value[i*left.blocksize + j] = 0;
}
void multNonParallelBlock(trmat& left, trmat& right, block_matrix& result)
{
	for (int i1 = 0; i1 < left.size; i1++)
	{
		for (int j = 0, j1 = left.size - 1; j < left.size; j++, j1--)
			for (int k = 0, k1 = j; k <= j; k++, k1--)
			{
				//оформление правильной индексации
				if (i1 > k)
				{
					int *LT = new int[left.blocksize*left.blocksize];
					for (int i = 0; i < left.blocksize; i++)
						for (int j = 0; j < left.blocksize; j++)
							LT[i*left.blocksize + j] = left.colomn[k].blocks[i1 - k].value[i + j*left.blocksize];
					multTwoBlockParallel(LT, right.colomn[k].blocks[j - k].value, result.colomn[i1].blocks[j].value, left.blocksize);
				}
				else
					multTwoBlockParallel(left.colomn[i1].blocks[k - i1].value, right.colomn[k].blocks[j - k].value, result.colomn[i1].blocks[j].value, left.blocksize);
			}
	}
}

//перемножение в 2 блока
void multTwoBlockNonParallel(int *A, int *B, int *C, int n)
{
	int *BT = new int[n*n];
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			BT[i*n + j] = B[i + j*n];
	int i, j, k;
	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++) {
			int dot = 0;
			for (k = 0; k < n; k++)
				dot += A[i*n + k] * BT[j*n + k];
			C[i*n + j] += dot;
		}
	delete[] BT;
}