#include "SymmetricMatrix.h"

int SymmetricMatrix::getBlockMemoryShift(int blockY, int blockX)
{
	// Data stored by rows
	int block_number = blockY * blocks_in_line - (((2.0 + (double(blockY) - 1.0)) / 2.0)*double(blockY)) + blockX;
	return block_number * block_capacity;
}

int SymmetricMatrix::getElementMemoryShift(int row, int column)
{
	int element_number = (row % block_width) * block_width + (column % block_width);

	int blockX = column / block_width;
	int blockY = row / block_width;

	int memory_shift = getBlockMemoryShift(blockY, blockX);
	return element_number + memory_shift;
}

void SymmetricMatrix::clean()
{
	for (int i = 0; i < data_size; i++)
		data[i] = 0;
}

SymmetricMatrix::SymmetricMatrix(int matrix_width, int block_width)
{
	this->block_width = block_width;
	this->matrix_width = matrix_width;

	blocks_in_line = matrix_width / block_width;

	block_count = blocks_in_line * blocks_in_line;
	block_capacity = block_width * block_width;

	int symmetric_block_count = 0;
	for (int x = 0; x < matrix_width / block_width; x++)
		for (int y = 0; y < matrix_width / block_width; y++)
			if (x < y)
				symmetric_block_count++;

	data_size = (block_count - symmetric_block_count) * block_capacity;
	data = new int[data_size];

	tmp = new int[block_capacity];

	clean();
}

SymmetricMatrix::~SymmetricMatrix()
{
	delete[] data;
	delete[] tmp;
}

int SymmetricMatrix::operator()(int row, int column)
{
	if (column < row)
	{
		std::swap(row, column);
	}
	return data[getElementMemoryShift(column, row)];
}

void SymmetricMatrix::setValue(int row, int column, int new_value)
{
	data[getElementMemoryShift(row, column)] = new_value;
}

int* SymmetricMatrix::getBlock(int blockY, int blockX)
{
	if (blockX > blockY)
	{
		std::swap(blockX, blockY);

		int* block = data + getBlockMemoryShift(blockY, blockX);

		int index_in_tmp_block = 0;

		for (int i = 0; i < block_width; i++)
		{
			for (int j = i; j < block_capacity; j += block_width)
			{
				tmp[index_in_tmp_block] = block[j];
				index_in_tmp_block++;
			}
		}
		return tmp;
	}
	else
	{
		int* block = data + getBlockMemoryShift(blockY, blockX);
		return block;
	}
}


void SymmetricMatrix::readMatrixFromFile(std::string filePath) 
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

void SymmetricMatrix::printMatrix()
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

void SymmetricMatrix::generateMatrix(int min_val, int max_val)
{
	std::mt19937 gen(time(0));
	std::uniform_int_distribution<> uid(min_val, max_val);

	for (int i = 0; i < matrix_width; i++)
		for (int j = 0; j < matrix_width; ++j)
		{
			int rnd_num = uid(gen);
			this->setValue(i, j, rnd_num);
			this->setValue(j, i, rnd_num);
		}
}