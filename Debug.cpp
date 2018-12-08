#include "pch.h"
#include <iostream>

#define EPSILON 0.00000001

/* Just for visual debugging */
void print_matrix_block(matrix_block& block, int blocksize)
{
	int i, j;
	for (i = 0; i < blocksize; i++)
	{
		for (j = 0; j < blocksize; j++)
		{
			std::cout << block.value[i*blocksize + j];
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void print_matrix_row(matrix_row& row, int rowsize, int blocksize, int empty)
{
	int j, l, k;
	for (l = 0; l < blocksize; l++)
	{
		for (j = 0; j < rowsize; j++)
		{
			for (k = 0; k < blocksize; k++)
			{
				std::cout << row.blocks[j].value[l*blocksize + k];
			}
			std::cout << "|";
		}
		std::cout << std::endl;
	}
}

void print_upper_triangular_matrix(upper_triangular_matrix& matrix)
{
	block_matrix mm;
	convert_upper_triangular_to_block_matrix(matrix, mm);
	print_block_matrix(mm);
	free_block_matrix(mm);
}

void print_symmetric_matrix(upper_triangular_matrix& matrix)
{
	block_matrix mm;
	make_symmetric(matrix, mm);
	print_block_matrix(mm);
	free_block_matrix(mm);
}

void print_block_matrix(block_matrix& matrix)
{
	int i;
	for (i = 0; i < matrix.size; i++)
	{
		print_matrix_row(matrix.rows[i], matrix.size, matrix.blocksize, 0);
		std::cout << std::endl;
	}
}

int convert_upper_triangular_to_block(upper_triangular_matrix& matrix,
	matrix_block& newm)
{
	init_matrix_block(newm, matrix.blocksize*matrix.size);
	int i, j, k, l;
	int p = 0;

	for (i = 0; i < matrix.size; i++)
	{
		for (l = 0; l < matrix.blocksize; l++)
		{
			for (j = 0; j < i; j++)
			{
				for (k = 0; k < matrix.blocksize; k++)
				{
					newm.value[p++] = 0;
				}
			}

			for (j = 0; j < matrix.size - i; j++)
			{
				for (k = 0; k < matrix.blocksize; k++)
				{
					newm.value[p++] =
						matrix.rows[i].blocks[j].value[l*matrix.blocksize + k];
				}
			}
		}
	}
	return matrix.blocksize*matrix.size;
}

void convert_upper_triangular_to_block_matrix(upper_triangular_matrix& matrix,
	block_matrix& newm)
{
	init_block_matrix(newm, matrix.size, matrix.blocksize);
	int i, j;
	for (i = 0; i < matrix.size; i++)
	{
		for (j = 0; j < matrix.size - i; j++)
		{
			memcpy(newm.rows[i].blocks[i + j].value, matrix.rows[i].blocks[j].value,
				matrix.blocksize*matrix.blocksize * sizeof(double));
		}
	}
}

void convert_block_matrix_to_block(block_matrix& matrix, matrix_block& block)
{
	init_matrix_block(block, matrix.size*matrix.blocksize);

	int memblock = matrix.blocksize * sizeof(double);

	for (int i = 0; i < matrix.size*matrix.blocksize; i++)
	{
		for (int j = 0; j < matrix.size; j++)
		{
			memcpy(block.value + matrix.blocksize*(i*matrix.size + j),
				matrix.rows[i / matrix.blocksize].blocks[j].value +
				(i % matrix.blocksize)*matrix.blocksize, memblock);
		}
	}
}

int is_empty_block(matrix_block& block, int blocksize)
{
	int i = 0;
	while (i < blocksize*blocksize && EPSILON >= fabs(block.value[i])) { i++; }
	return (i == blocksize * blocksize);
}

int is_equal(block_matrix& matrix, matrix_block& block)
{
	matrix_block blockm, diff;
	init_matrix_block(diff, matrix.blocksize*matrix.size);
	convert_block_matrix_to_block(matrix, blockm);
	difference(block, blockm, diff, matrix.blocksize*matrix.size);
	return is_empty_block(diff, matrix.blocksize*matrix.size);
}

void difference(matrix_block& left, matrix_block& right, matrix_block& result,
	int blocksize)
{
	for (int i = 0; i < blocksize*blocksize; i++)
	{
		result.value[i] = left.value[i] - right.value[i];
	}
}

void block_transpose(matrix_block& block, matrix_block& result, int blocksize)
{
	transpose(block.value, result.value, blocksize);
}

void make_symmetric(upper_triangular_matrix& matrix, block_matrix& newm)
{
	init_block_matrix(newm, matrix.size, matrix.blocksize);
	int memblocksize = matrix.blocksize*matrix.blocksize * sizeof(double);
	for (int i = 0; i < matrix.size - 1; i++)
	{
		for (int j = 1; j < matrix.size - i; j++)
		{
			memcpy(newm.rows[i].blocks[j + i].value,
				matrix.rows[i].blocks[j].value, memblocksize);
			block_transpose(matrix.rows[i].blocks[j],
				newm.rows[j + i].blocks[i], matrix.blocksize);
		}
	}

	for (int i = 0; i < matrix.size; i++)
	{
		memcpy(newm.rows[i].blocks[i].value, matrix.rows[i].blocks[0].value,
			memblocksize);
		mirror(newm.rows[i].blocks[i], newm.blocksize);
	}
}