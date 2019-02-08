#pragma once
#include <iostream>
#include <sstream>
#include <fstream>
#include "SymmetricMatrix.h"

class TriangularMatrix : public SymmetricMatrix 
{
protected:
	int getBlockMemoryShift(int blockY, int blockX);
	int getElementMemoryShift(int row, int column);
	
public:
	TriangularMatrix(int matrix_width, int block_width);
	~TriangularMatrix();

	int operator()(int row, int column);

	void setValue(int row, int column, int value);
	int *getBlock(int blockY, int blockX);
	void readMatrixFromFile(std::string filePath);
	void printMatrix();

	void generateMatrix(int min_val, int max_val);

	void clean();
};

