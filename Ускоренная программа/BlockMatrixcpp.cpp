#include "pch.h"
#include <stdio.h>
#include <stdlib.h>
#include <cstring>

void transpose(double *A, double *B, int n)
{
	int i, j;
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			B[i*n + j] = A[i + j * n];
		}
	}
}

void mult_A_B(double *A, double *B, double *C, int n)
{
	double *BT = new double[n * n]; 
	transpose(B, BT, n);
	{
		int i, j, k;
		for (i = 0; i < n; i++)
		{
			for (j = 0; j < n; j++)
			{
				double dot = 0;
				for (k = 0; k < n; k++)
				{
					dot += A[i*n + k] * BT[j*n + k];
				}
				C[i*n + j] += dot;
			}
		}

	}
	delete[]BT;
}

void mult_A_B_openmp(double *A, double *B, double *C, int n)
{
	double *BT = new double[n * n];
	transpose(B, BT, n);
#pragma omp parallel
	{
		int i, j, k;
#pragma omp for
		for (i = 0; i < n; i++)
		{
			for (j = 0; j < n; j++)
			{
				double dot = 0;
				for (k = 0; k < n; k++)
				{
					dot += A[i*n + k] * BT[j*n + k];
				}
				C[i*n + j] += dot;
			}
		}

	}
	delete[]BT;
}

void init_matrix_block(matrix_block& block, int blocksize)
{
	block.value = new double[blocksize * blocksize];
	memset(block.value, 0, blocksize*blocksize * sizeof(double));
}

void free_matrix_block(matrix_block& block)
{
	delete[] block.value;
}

void fill_random_matrix_block(matrix_block& block, int blocksize)
{
	int i;
	for (i = 0; i < blocksize*blocksize; i++)
	{
		block.value[i] = 0.01*((rand() - RAND_MAX / 2) % 1000);
	}
}

void mult_add_matrix_block_parallel(matrix_block& left, matrix_block& right,
	matrix_block& result, int blocksize)
{
	mult_A_B_openmp(left.value, right.value, result.value, blocksize);
}

void mult_add_matrix_block(matrix_block& left, matrix_block& right,
	matrix_block& result, int blocksize)
{
	mult_A_B(left.value, right.value, result.value, blocksize);
}


void mult_add_matrix_block_transpose(matrix_block& left, matrix_block& right,
	matrix_block& result, int blocksize)
{
	double *LT = new double[blocksize * blocksize];
	transpose(left.value, LT, blocksize);
	mult_A_B_openmp(LT, right.value, result.value, blocksize);
	delete[] LT;
}

void mult_add_matrix_block_transpose_parallel(matrix_block& left, matrix_block& right,
	matrix_block& result, int blocksize)
{
	double *LT = new double[blocksize * blocksize];
	transpose(left.value, LT, blocksize);
	mult_A_B(LT, right.value, result.value, blocksize);
	delete[] LT;
}

void init_matrix_row(matrix_row& row, int rowsize, int blocksize)
{
	row.blocks = new matrix_block[rowsize];
	int i;
	for (i = 0; i < rowsize; i++)
	{
		init_matrix_block(row.blocks[i], blocksize);
	}
}

void free_matrix_row(matrix_row& row, int rowsize)
{
	int i;
	for (i = 0; i < rowsize; i++)
	{
		free_matrix_block(row.blocks[i]);
	}
	delete[] row.blocks;
}

void init_upper_triangular_matrix(upper_triangular_matrix& matrix, int size, int blocksize)
{
	matrix.size = size;
	matrix.blocksize = blocksize;
	matrix.rows = new matrix_row[size];

	int i;
	for (i = 0; i < size; i++)
	{
		init_matrix_row(matrix.rows[i], size - i, blocksize);
	}
}

void init_block_matrix(block_matrix& matrix, int size, int blocksize)
{
	matrix.size = size;
	matrix.blocksize = blocksize;
	matrix.rows = new matrix_row[size];

	int i;
	for (i = 0; i < size; i++)
	{
		init_matrix_row(matrix.rows[i], size, blocksize);
	}
}

void free_upper_triangular_matrix(upper_triangular_matrix& matrix)
{
	int i;
	for (i = 0; i < matrix.size; i++)
	{
		free_matrix_row(matrix.rows[i], matrix.size - i);
	}
	delete[] matrix.rows;
}

void free_block_matrix(block_matrix& matrix)
{
	int i;
	for (i = 0; i < matrix.size; i++)
	{
		free_matrix_row(matrix.rows[i], matrix.size);
	}
	delete[] matrix.rows;
}

void random_fill_upper_triangular_matrix(upper_triangular_matrix& matrix)
{
	int i, j, k;
	for (i = 0; i < matrix.size; i++)
	{
		for (j = 0; j < matrix.size - i; j++)
		{
			fill_random_matrix_block(matrix.rows[i].blocks[j], matrix.blocksize);
		}
	}

	/* Zero the left triangle */

	for (i = 0; i < matrix.size; i++)
	{
		for (j = 1; j < matrix.blocksize; j++)
		{
			for (k = 0; k < j; k++)
			{
				matrix.rows[i].blocks[0].value[j*matrix.blocksize + k] = 0.0;
			}
		}
	}
}

void mult_symm(upper_triangular_matrix& left,
	upper_triangular_matrix& right, block_matrix& result)
{
	for (int i = 0; i < left.size; i++)
	{
		for (int j = 0; j < left.size; j++)
		{
			for (int k = 0; k <= j; k++)
			{
				if (i > k)
				{
					mult_add_matrix_block_transpose(left.rows[k].blocks[i - k],
						right.rows[k].blocks[j - k],
						result.rows[i].blocks[j], left.blocksize);
				}
				else
				{
					mult_add_matrix_block(left.rows[i].blocks[k - i],
						right.rows[k].blocks[j - k],
						result.rows[i].blocks[j], left.blocksize);
				}
			}
		}
	}
}

void mult_symm_openmp(upper_triangular_matrix& left,
	upper_triangular_matrix& right, block_matrix& result)
{
#pragma omp parallel for shared(left, right, result) num_threads(8)
	for (int i = 0; i < left.size; i++)
	{
		for (int j = 0; j < left.size; j++)
		{
			for (int k = 0; k <= j; k++)
			{
				if (i > k)
				{
					mult_add_matrix_block_transpose(left.rows[k].blocks[i - k],
						right.rows[k].blocks[j - k],
						result.rows[i].blocks[j], left.blocksize);
				}
				else
				{
					mult_add_matrix_block(left.rows[i].blocks[k - i],
						right.rows[k].blocks[j - k],
						result.rows[i].blocks[j], left.blocksize);
				}
			}
		}
	}
}

void mult_symmetric_upper_triangular_matrix_parallel(upper_triangular_matrix& left,
	upper_triangular_matrix& right, block_matrix& result)
{
	for (int j = 0; j < left.size; j++)
	{
		mirror(left.rows[j].blocks[0], left.blocksize);
	}

	mult_symm_openmp(left, right, result);


	for (int j = 0; j < left.size; j++)
	{
		unmirror(left.rows[j].blocks[0], left.blocksize);
	}
}

void mult_symmetric_upper_triangular_matrix(upper_triangular_matrix& left,
	upper_triangular_matrix& right, block_matrix& result)
{
	for (int j = 0; j < left.size; j++)
	{
		mirror(left.rows[j].blocks[0], left.blocksize);
	}

	mult_symm(left, right, result);

	for (int j = 0; j < left.size; j++)
	{
		unmirror(left.rows[j].blocks[0], left.blocksize);
	}
}

void mirror(matrix_block& block, int blocksize)
{
	for (int i = 1; i < blocksize; i++)
	{
		for (int j = 0; j < i; j++)
		{
			block.value[i*blocksize + j] = block.value[j*blocksize + i];
		}
	}
}

void unmirror(matrix_block& block, int blocksize)
{
	for (int i = 1; i < blocksize; i++)
	{
		for (int j = 0; j < i; j++)
		{
			block.value[i*blocksize + j] = 0;
		}
	}
}
