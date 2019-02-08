#pragma once
#include <iostream>
#include <sstream>
#include <fstream>
#include <random>
#include <ctime>

class SymmetricMatrix 
{
protected:
	int *data;
	int *tmp;

	int block_count, 
		block_width, 
		block_capacity, 
		matrix_width, 
		data_size, 
		blocks_in_line;

	int getBlockMemoryShift(int row, int column);
	int getElementMemoryShift(int row, int column);	

public:
	SymmetricMatrix(int matrix_width, int block_width);
	~SymmetricMatrix();

	int operator()(int row, int column);
	void setValue(int row, int column, int value);
	int *getBlock(int blockY, int blockX);

	void readMatrixFromFile(std::string filePath);
	void printMatrix();

	void generateMatrix(int min_val, int max_val);

	void clean();
};


