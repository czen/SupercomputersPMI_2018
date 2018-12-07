#include <time.h>
#include <iostream>
#include <omp.h>
#include "blockmatrix.h"
using namespace std;

struct matrix_block{ int* value;};
struct matrix_row{matrix_block* blocks;};
struct trmat
{
	//запись матрицы в строке с учётом блоков
	matrix_row* row;
	int size;
	int blocksize;
};
typedef struct trmat block_matrix;
typedef struct trmat symmetric_matrix;
//параллельное умножение   
void multParallel(trmat& left,
	trmat& right, block_matrix& result);
void multParallelBlock(trmat& left,
	trmat& right, block_matrix& result);
void multTwoBlockParallel(int *A, int *B, int *C, int n);
//непараллельное умножение
void multNonParallel(trmat& left,
	trmat& right, block_matrix& result);
void multNonParallelBlock(trmat& left,
	trmat& right, block_matrix& result);
void multTwoBlockNonParallel(int *A, int *B, int *C, int n);

int main() {
	setlocale(LC_ALL, "Rus");
	int matrix_size = 1024;
	cout << "Matrix size = " << matrix_size << endl;
    int block_size = 8;
	while (block_size<1024){
	cout << "Block's size = " << block_size<< endl;
	int size = matrix_size / block_size;
	trmat a;
	a.size = size;
	a.blocksize = block_size;
	a.row = new matrix_row[size];
	for (int i = 0; i < size; i++)
	{
		a.row[i].blocks = new matrix_block[size - i];
		for (int j = 0; j < size - i; j++)
		{
			int s = block_size*block_size;
			a.row[i].blocks[j].value = new int[s];
			for (int k = 0; k < s; ++k) a.row[i].blocks[j].value[k] = 0;
		}


	}
	int i, j, k;
	for (i = 0; i < a.size; i++)
		for (j = 0; j < a.size - i; j++)
			for (k = 0; k < a.blocksize*a.blocksize; k++) {
				a.row[i].blocks[j].value[k] = rand() % 10 + 1;
			}
	for (i = 0; i < a.size; i++)
		for (j = 1; j < a.blocksize; j++)
			for (k = 0; k < j; k++)
				a.row[i].blocks[0].value[j*a.blocksize + k] = 0;
	trmat b;
	b.size = size;
	b.blocksize = block_size;
	b.row = new matrix_row[size];
	for (int i = 0; i < size; i++)
	{
		b.row[i].blocks = new matrix_block[size - i];
		for (int j = 0; j < size - i; j++)
		{
			int s = block_size*block_size;
			b.row[i].blocks[j].value = new int[s];
			for (int k = 0; k < s; ++k) b.row[i].blocks[j].value[k] = 0;
		}


	}
	for (i = 0; i < b.size; i++)
		for (j = 0; j < b.size - i; j++)
			for (k = 0; k < b.blocksize*b.blocksize; k++) {
				b.row[i].blocks[j].value[k] = rand() % 10 + 1;
			}
	for (i = 0; i < b.size; i++)
		for (j = 1; j < b.blocksize; j++)
			for (k = 0; k < j; k++)
				b.row[i].blocks[0].value[j*b.blocksize + k] = 0;
	block_matrix c;
	c.size = size;
	c.blocksize = block_size;
	c.row = new matrix_row[size];
	for (i = 0; i < size; i++)
	{
		c.row[i].blocks = new matrix_block[size];
		for (int j = 0; j < size; j++)
		{
			int s = block_size*block_size;
			c.row[i].blocks[j].value = new int[s];
			for (int k = 0; k < s; ++k) c.row[i].blocks[j].value[k] = 0;
		}

	}
	block_matrix c1;
	c1.size = size;
	c1.blocksize = block_size;
	c1.row = new matrix_row[size];
	for (i = 0; i < size; i++)
	{
		c1.row[i].blocks = new matrix_block[size];
		for (int j = 0; j < size; j++)
		{
			int s = block_size*block_size;
			c1.row[i].blocks[j].value = new int[s];
			for (int k = 0; k < s; ++k) c1.row[i].blocks[j].value[k] = 0;
		}

	}
	double start_time, end_time;
	start_time = omp_get_wtime();
	multParallel(a, b, c);
	end_time = omp_get_wtime();
	cout << "Parallel time = " << end_time - start_time << endl;
	multNonParallel(a, b, c1);
	end_time = omp_get_wtime();
	cout << "NonParallel time = " << end_time - start_time << endl;
	block_size *= 2;
    }
	system("pause");
	return 0;
}

void multParallel(trmat& left,trmat& right, block_matrix& result)
{
	//перенумируем элементы матрицы для удобства
	for (int k = 0; k< left.size; k++)
	 for (int i = 1; i < left.blocksize; i++)
		  for (int j = 0; j < i; j++)
			left.row[k].blocks[0].value[i*left.blocksize + j] = left.row[k].blocks[0].value[j*left.blocksize + i];
	//	параллельное умножение
	multParallelBlock(left, right, result);
	//перенумируем обратно
	for (int k = 0; k < left.size; k++)
	 for (int i = 1; i < left.blocksize; i++)
		for (int j = 0; j < i; j++)
			left.row[k].blocks[0].value[i*left.blocksize + j] = 0;
}

void multParallelBlock(trmat& left,trmat& right, block_matrix& result)
{

#pragma omp parallel for shared(left, right, result) num_threads(4)
	for (int i1 = 0; i1 < left.size; i1++)
	{

		for (int j = 0, j1 = left.size - 1; j < left.size; j++, j1--)
		{
			for (int k = 0, k1 = j; k <= j; k++, k1--)
			{
				if (i1 > k)
				{
					int *LT = new int[left.blocksize*left.blocksize];
					for (int i = 0; i < left.blocksize; i++)
					  for (int j = 0; j < left.blocksize; j++)
							LT[i*left.blocksize + j] = left.row[k].blocks[i1 - k].value[i + j*left.blocksize];
				    multTwoBlockParallel(LT, right.row[k].blocks[j - k].value, result.row[i1].blocks[j].value, left.blocksize);
				}
				else
					multTwoBlockParallel(left.row[i1].blocks[k - i1].value, right.row[k].blocks[j - k].value, result.row[i1].blocks[j].value, left.blocksize);
				
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

void multNonParallel(trmat& left,trmat& right, block_matrix& result)
{
	//перенумируем элементы матрицы для удобства
	for (int k = 0; k< left.size; k++)
		for (int i = 1; i < left.blocksize; i++)
			for (int j = 0; j < i; j++)
				left.row[k].blocks[0].value[i*left.blocksize + j] = left.row[k].blocks[0].value[j*left.blocksize + i];
	//	непараллельное умножение
	multNonParallelBlock(left, right, result);
	//перенумируем обратно
	for (int k = 0; k < left.size; k++)
		for (int i = 1; i < left.blocksize; i++)
			for (int j = 0; j < i; j++)
				left.row[k].blocks[0].value[i*left.blocksize + j] = 0;
}

void multNonParallelBlock(trmat& left,trmat& right, block_matrix& result)
{

for (int i1 = 0; i1 < left.size; i1++)
	{

		for (int j = 0, j1 = left.size - 1; j < left.size; j++, j1--)
		 for (int k = 0, k1 = j; k <= j; k++, k1--)
			{
				if (i1 > k)
				{
					int *LT = new int[left.blocksize*left.blocksize];
					for (int i = 0; i < left.blocksize; i++)
						for (int j = 0; j < left.blocksize; j++)
							LT[i*left.blocksize + j] = left.row[k].blocks[i1 - k].value[i + j*left.blocksize];
					multTwoBlockParallel(LT, right.row[k].blocks[j - k].value, result.row[i1].blocks[j].value, left.blocksize);
				}
				else
					multTwoBlockParallel(left.row[i1].blocks[k - i1].value, right.row[k].blocks[j - k].value, result.row[i1].blocks[j].value, left.blocksize);

			}
	}
}

void multTwoBlockNonParallel(int *A, int *B, int *C, int n)
{
	int *BT = new int[n*n];
	for (int i = 0; i < n; i++)
	for (int j = 0; j < n; j++)
		 BT[i*n + j] = B[i + j*n];
int i, j, k;
for (i = 0; i < n; i++)
		for (j = 0; j < n; j++){
				int dot = 0;
				for (k = 0; k < n; k++)
				 dot += A[i*n + k] * BT[j*n + k];
                C[i*n + j] += dot;
			}
	delete[] BT;
}