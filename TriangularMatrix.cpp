#include "TriangularMatrix.h"

int TriangularMatrix::getBlockMemoryShift(int blockY, int blockX)
{
	// Data stored by columns
	int block_number = double(blockX) * double((4.0 + double((blockX - 1))) / 2.0) + (blockY - blockX);
	return block_number * block_capacity;
}

int TriangularMatrix::getElementMemoryShift(int row, int column)
{
	int element_number = (row % block_width) * block_width + (column % block_width);

	int blockY = row / block_width;
	int blockX = column / block_width;

	int memory_shift = getBlockMemoryShift(blockY, blockX);
	return element_number + memory_shift;
}

void TriangularMatrix::clean()
{
	for (int i = 0; i < data_size; i++)
		data[i] = 0;
	for (int i = 0; i < block_capacity; i++)
		tmp[i] = 0;
}

TriangularMatrix::TriangularMatrix(int matrix_width, int block_width) : SymmetricMatrix(matrix_width, block_width)
{
}

TriangularMatrix::~TriangularMatrix()
{
}

int TriangularMatrix::operator()(int row, int column)
{
	if (column < row)
		return 0;
	return data[getElementMemoryShift(row, column)];
}

void TriangularMatrix::setValue(int row, int column, int new_value)
{
	if (column < row)
		return;
	data[getElementMemoryShift(row, column)] = new_value;
}

int* TriangularMatrix::getBlock(int blockY, int blockX)
{
	int* block;

	if (blockX < blockY)
		block = tmp;
	else
		block = data + getBlockMemoryShift(blockY, blockX);
	return block;
}

void TriangularMatrix::readMatrixFromFile(std::string filePath)
{
	clean();
	std::fstream in(filePath);
	std::string line;
	int i = 0;

	while (std::getline(in, line)) 
	{
		int j = 0;
		std::stringstream linestream(line);

		int value;
		while (linestream >> value) 
		{
			setValue(i, j, value);
			++j;
		}
		++i;
	}

	in.close();
}

void TriangularMatrix::printMatrix()
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

void TriangularMatrix::generateMatrix(int min_val, int max_val)
{
	std::mt19937 gen(time(0));
	std::uniform_int_distribution<> uid(min_val, max_val);

	for (int i = 0; i < matrix_width; i++)
	{
		for (int j = 0; j < matrix_width; j++)
		{
			if (i > j)
				this->setValue(i, j, 0);
			else
			{
				int rnd_num = uid(gen);
				this->setValue(i, j, rnd_num);
			}
		}
	}
}