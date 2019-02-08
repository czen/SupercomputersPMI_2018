#define MATRIX_SIZE 1500

#include <omp.h>

#include "SymmetricMatrix.h"
#include "TriangularMatrix.h"
#include "BlockMatrix.h"

auto blocks = {5, 10, 15, 20, 25, 30, 50, 100, 75, 125, 150, 250, 300, 500, 750};

void multiplyBlocks(int *A, int *B, int *C, int block_size)
{
	for (int i = 0; i < block_size; i++)
		for (int j = 0; j < block_size; j++)
			for (int k = 0; k < block_size; k++)
				C[i * block_size + j] += A[i * block_size + k] * B[k * block_size + j];
}

void multiplyBlocksParallel(int *A, int *B, int *C, int block_size)
{
#pragma omp parallel for
	for (int i = 0; i < block_size; i++)
		for (int j = 0; j < block_size; j++)
			for (int k = 0; k < block_size; k++)
				C[i * block_size + j] += A[i * block_size + k] * B[k * block_size + j];
}

void serialMultiplication(TriangularMatrix *A, SymmetricMatrix *B, BlockMatrix *C)
{
	int blocks_on_line = C->getBlocksOnLine();

	for (int i = 0; i < blocks_on_line; i++)
		for (int j = 0; j < blocks_on_line; j++)
			for (int k = 0; k < blocks_on_line; k++)
				multiplyBlocks(A->getBlock(i, k), B->getBlock(k, j), C->getBlock(i, j), C->getBlockWidth());
}

void InBlockParallelMultiplication(TriangularMatrix *A, SymmetricMatrix *B, BlockMatrix *C)
{
	int blocks_on_line = C->getBlocksOnLine();

	for (int i = 0; i < blocks_on_line; i++)
		for (int j = 0; j < blocks_on_line; j++)
			for (int k = 0; k < blocks_on_line; k++)
				multiplyBlocksParallel(A->getBlock(i, k), B->getBlock(k, j), C->getBlock(i, j), C->getBlockWidth());
}

void PairParallelMultiplication(TriangularMatrix *A, SymmetricMatrix *B, BlockMatrix *C)
{
	int blocks_on_line = C->getBlocksOnLine();

#pragma omp parallel for
	for (int i = 0; i < blocks_on_line; i++)
		for (int j = 0; j < blocks_on_line; j++)
			for (int k = 0; k < blocks_on_line; k++)
				multiplyBlocks(A->getBlock(i, k), B->getBlock(k, j), C->getBlock(i, j), C->getBlockWidth());
}


int main() 
{	
	double start_time[3], end_time[3];

	for (int block_width : blocks) 
	{
		std::cout << "Block length: " << block_width << std::endl;

		TriangularMatrix *A = new TriangularMatrix(MATRIX_SIZE, block_width);	
		A->generateMatrix(-1000, 1000);

		SymmetricMatrix *B = new SymmetricMatrix(MATRIX_SIZE, block_width);
		B->generateMatrix(-1000, 1000);
		
		BlockMatrix *C = new BlockMatrix(MATRIX_SIZE, block_width);
		C->clean();

		start_time[0] = omp_get_wtime();
		serialMultiplication(A, B, C);
		end_time[0] = omp_get_wtime();

		std::cout << "Serial multiplicaton: " << end_time[0] - start_time[0] << std::endl;

		C->clean();

		start_time[1] = omp_get_wtime();
		InBlockParallelMultiplication(A, B, C);
		end_time[1] = omp_get_wtime();
		std::cout << "In block parallel multiplication: " << end_time[1] - start_time[1] << std::endl;
		C->clean();

		start_time[2] = omp_get_wtime();
		PairParallelMultiplication(A, B, C);
		end_time[2] = omp_get_wtime();
		std::cout << "Pair parallel multiplication: " << end_time[2] - start_time[2] << std::endl;
		C->clean();

		delete A;
		delete B;
		delete C;
		std::cout << "===============================" << std::endl;
	}
	system("pause");
	return 0;
	
}