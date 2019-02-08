#include "BlockMatrix.h"

// Returns memory shift to specified block
// Blocks are stored by columns
int BlockMatrix::getBlockMemoryShift(int blockY, int blockX)
{
	int block_number = blockX * blocks_in_line + blockY;
	return block_number * block_capacity;
}

int BlockMatrix::getElementMemoryShift(int row, int column)
{
	// Elements inside block are stored by columns
	int element_number = (row % block_width) * block_width + (column % block_width);

	int blockY = row / block_width;
	int blockX = column / block_width;

	int memory_shift = getBlockMemoryShift(blockY, blockX);
	return memory_shift + element_number;
}

BlockMatrix::BlockMatrix(int matrix_width, int block_width)
{
	this->block_width = block_width;
	this->matrix_width = matrix_width;

	blocks_in_line = matrix_width / block_width;

	block_count = (blocks_in_line) * (blocks_in_line);
	block_capacity = block_width * block_width;

	data_size = block_count * block_capacity;
	data = new int[data_size];
}

BlockMatrix::~BlockMatrix() {
	delete[] data;
}

int BlockMatrix::operator()(int row, int column)
{
	return data[getElementMemoryShift(row, column)];
}

int* BlockMatrix::getBlock(int blockY, int blockX)
{
	int* block = data + getBlockMemoryShift(blockY, blockX);
	return block;
}

int BlockMatrix::getBlockCount()
{
	return block_count;
}

int BlockMatrix::getBlocksOnLine()
{
	return blocks_in_line;
}

int BlockMatrix::getBlockWidth()
{
	return block_width;
}

void BlockMatrix::clean()
{
	for (int i = 0; i < data_size; i++)
		data[i] = 0;
}

void BlockMatrix::printMatrix()
{
	for (int i = 0; i < matrix_width; i++)
	{
		for (int j = 0; j < matrix_width; j++)
		{
			std::cout << this->operator()(i, j) << " ";
		}
		std::cout << std::endl;
	}
}