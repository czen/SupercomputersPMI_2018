#include <time.h>
#include <iostream>
#include <omp.h>
using namespace std;

struct matrix_block_line
{
	//Блочные Строки
	int* value;
};
struct block_matrix_row
{
	matrix_block_line* blocks;
	int row_length;
};
struct block_matrix
{
	block_matrix_row* row;
	int col_length;
	int blocksize;
};  
void multParallel(block_matrix& left,
	block_matrix& right, block_matrix& result);
void multTwoBlockParallel(int *A, int *B, int *C, int n);
void multNonParallel(block_matrix& left,
	block_matrix& right, block_matrix& result);
void multTwoBlockNonParallel(int *A, int *B, int *C, int n);

int main() {
	setlocale(LC_ALL, "Rus");

	int matrix_size = 1024;
	cout << "Matrix size = " << matrix_size << "x" << matrix_size << endl;
	int block_size = 2;

	while (block_size < matrix_size) {
		cout << "Block's size = " << block_size << "x" << block_size << endl;
		int size = matrix_size / block_size;

		block_matrix a;
		a.col_length = size;
		a.blocksize = block_size;
		a.row = new block_matrix_row[size];
		for (int i = 0; i < size; i++)
		{
			a.row[i].blocks = new matrix_block_line[i + 1];
			a.row[i].row_length = i + 1;
			for (int j = 0; j < i + 1; j++)
			{
				a.row[i].blocks[j].value = new int[block_size * block_size];
				for (int k = 0; k < block_size * block_size; ++k)
					a.row[i].blocks[j].value[k] = rand() % 10 + 1;
			}
		}
		for (int i = 0; i < a.col_length; i++)
			for (int j = 0; j < i + 1; j++)
				for (int k = 0; k < a.blocksize; k++)
					for (int l = 0; l < a.blocksize; l++)
						if (k<l)
							a.row[i].blocks[i + 1 - 1].value[k*a.blocksize + l] = 0;

		block_matrix b;
		b.col_length = size;
		b.blocksize = block_size;
		b.row = new block_matrix_row[size];
		for (int i = 0; i < size; i++)
		{
			b.row[i].blocks = new matrix_block_line[size - i];
			b.row[i].row_length = size - i;
			for (int j = 0; j < size - i; j++)
			{
				b.row[i].blocks[j].value = new int[block_size * block_size];
				for (int k = 0; k < block_size * block_size; ++k)
					b.row[i].blocks[j].value[k] = rand() % 10 + 1;
			}
		}
		for (int i = 0; i < b.col_length; i++)
			for (int j = 0; j < size - i; j++)
				for (int k = 0; k < b.blocksize; k++)
					for (int l = 0; l < b.blocksize; l++)
						if (k>l)
							b.row[i].blocks[0].value[k*b.blocksize + l] = 0;

		block_matrix c;
		c.col_length = size;
		c.blocksize = block_size;
		c.row = new block_matrix_row[size];
		for (int i = 0; i < size; i++)
		{
			c.row[i].blocks = new matrix_block_line[size];
			c.row[i].row_length = size;
			for (int j = 0; j < size; j++)
			{
				c.row[i].blocks[j].value = new int[block_size * block_size];
				for (int k = 0; k < block_size * block_size; ++k)
					c.row[i].blocks[j].value[k] = 0;
			}

		}
		block_matrix c1;
		c1.col_length = size;
		c1.blocksize = block_size;
		c1.row = new block_matrix_row[size];
		for (int i = 0; i < size; i++)
		{
			c1.row[i].blocks = new matrix_block_line[size];
			c1.row[i].row_length = size;
			for (int j = 0; j < size; j++)
			{
				c1.row[i].blocks[j].value = new int[block_size * block_size];
				for (int k = 0; k < block_size * block_size; ++k)
					c1.row[i].blocks[j].value[k] = 0;
			}

		}

		double start_time, end_time;

		start_time = omp_get_wtime();
		multParallel(a, b, c);
		end_time = omp_get_wtime();
		cout << "Parallel time = " << end_time - start_time << endl;

		start_time = omp_get_wtime();
		multNonParallel(a, b, c1);
		end_time = omp_get_wtime();
		cout << "NonParallel time = " << end_time - start_time << endl;

		/*for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < i + 1; j++)
			{
				for (int k = 0; k < block_size * block_size; ++k)
					std::cout << a.row[i].blocks[j].value[k] << " ";
				std::cout << "|";
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size - i; j++)
			{
				for (int k = 0; k < block_size * block_size; ++k)
					std::cout << b.row[i].blocks[j].value[k] << " ";
				std::cout << "|";
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				for (int k = 0; k < block_size * block_size; ++k)
					std::cout << c.row[i].blocks[j].value[k] << " ";
				std::cout << "|";
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				for (int k = 0; k < block_size * block_size; ++k)
					std::cout << c1.row[i].blocks[j].value[k] << " ";
				std::cout << "|";
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;*/

		block_size *= 2;
	}
	system("pause");
	return 0;
}

void multParallel(block_matrix& left, block_matrix& right, block_matrix& result)
{
	for (int k = 0; k< right.col_length; k++)
		for (int i = 1; i < right.blocksize; i++)
			for (int j = 0; j < i; j++)
				right.row[k].blocks[0].value[i*right.blocksize + j] = right.row[k].blocks[0].value[j*right.blocksize + i];

#pragma omp parallel for shared(left, right, result) num_threads(4)
	for (int ik = 0; ik < result.col_length; ik++)
		for (int jk = 0; jk < result.col_length; jk++)
			for (int k = 0; k < result.col_length; k++)
			{
				if (k < left.row[ik].row_length)
					if (jk - result.row[k].row_length + right.row[k].row_length >= 0)
					{
						int nn = jk - result.row[k].row_length + right.row[k].row_length;
						multTwoBlockNonParallel(left.row[ik].blocks[k].value,
							right.row[k].blocks[nn].value,
							result.row[ik].blocks[jk].value,
							right.blocksize);
					}
					else
					{
						int *RT = new int[right.blocksize*right.blocksize];
						for (int i = 0; i < right.blocksize; i++)
							for (int j = 0; j < right.blocksize; j++)
							{
								int kk = k - result.row[k].row_length + right.row[k].row_length;
								int nn = jk - result.row[k].row_length + right.row[k].row_length + k;
								RT[i*right.blocksize + j] = right.row[nn].blocks[kk].value[i + j * right.blocksize];
							}
						multTwoBlockNonParallel(left.row[ik].blocks[k].value,
							RT,
							result.row[ik].blocks[jk].value,
							right.blocksize);
						delete[] RT;
					}
			}

	for (int k = 0; k < right.col_length; k++)
		for (int i = 1; i < right.blocksize; i++)
			for (int j = 0; j < i; j++)
				right.row[k].blocks[0].value[i*right.blocksize + j] = 0;
}


void multTwoBlockParallel(int *A, int *B, int *C, int n)
{
#pragma simd
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			for (int k = 0; k < n; k++)
				C[i*n + j] += A[i*n + k] * B[j*n + k];
}

void multNonParallel(block_matrix& left, block_matrix& right, block_matrix& result)
{
	for (int k = 0; k< right.col_length; k++)
		for (int i = 1; i < right.blocksize; i++)
			for (int j = 0; j < i; j++)
				right.row[k].blocks[0].value[i*right.blocksize + j] = right.row[k].blocks[0].value[j*right.blocksize + i];

	for (int ik = 0; ik < result.col_length; ik++)
		for (int jk = 0; jk < result.col_length; jk++)
			for (int k = 0; k < result.col_length; k++)
			{
				if (k < left.row[ik].row_length)
					if (jk - result.row[k].row_length + right.row[k].row_length >= 0)
					{
						int nn = jk - result.row[k].row_length + right.row[k].row_length;
						multTwoBlockNonParallel(left.row[ik].blocks[k].value,
							right.row[k].blocks[nn].value,
							result.row[ik].blocks[jk].value,
							right.blocksize);
					}
					else
					{
						int *RT = new int[right.blocksize*right.blocksize];
						for (int i = 0; i < right.blocksize; i++)
							for (int j = 0; j < right.blocksize; j++)
							{
								int kk = k - result.row[k].row_length + right.row[k].row_length;
								int nn = jk - result.row[k].row_length + right.row[k].row_length + k;
								RT[i*right.blocksize + j] = right.row[nn].blocks[kk].value[i + j * right.blocksize];
							}
						multTwoBlockNonParallel(left.row[ik].blocks[k].value,
							RT,
							result.row[ik].blocks[jk].value,
							right.blocksize);
						delete[] RT;
					}
			}

	for (int k = 0; k < right.col_length; k++)
		for (int i = 1; i < right.blocksize; i++)
			for (int j = 0; j < i; j++)
				right.row[k].blocks[0].value[i*right.blocksize + j] = 0;
}

void multTwoBlockNonParallel(int *A, int *B, int *C, int n)
{
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			for (int k = 0; k < n; k++)
				C[i*n + j] += A[i*n + k] * B[j*n + k];
}