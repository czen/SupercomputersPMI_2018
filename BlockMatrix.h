#pragma once
#include <iostream>
#include <fstream>

class BlockMatrix 
{
private:
	int *data;

	int block_count, 
		block_width, 
		block_capacity, 
		matrix_width, 
		data_size, 
		blocks_in_line;

	int getBlockMemoryShift(int row, int column);
	int getElementMemoryShift(int row, int column);

public:
	BlockMatrix(int matrix_width, int block_width);
	~BlockMatrix();

	int operator()(int row, int column);
	
	int* getBlock(int blockY, int blockX);

	int getBlockCount();
	int getBlockWidth();
	int getBlocksOnLine();
	void clean();
	void printMatrix();
};


