#pragma once
#include <iostream>
#include <ctime>
#include <random>

class SymmetrMatrix
{
private:
	int *data;//Чистые данные
	int block_quantity;//Кол-во блоков
	int	block_width;//Ширина блока в элементах
	int	block_capacity;//Кол-во элементов в блоке
	int	matrix_width;//Ширина матрицы в элементах
	int	matrix_capacity;//Кол-во элементов в матрице
	int	width_matrix_in_blocks;//Ширина матрицы в блоках

	//Получение индекса первого элемента конкретного блока
	int getFirstElementBlock(int row, int column);
	//Получение индекса элемента уже внутри блока относительно глобальной нумерации в матрице
	int getElementBlock(int row, int column);
	//Зануление всех элементов матрицы
	void zerofication();

public:
	//Конструктор симметричной матрицы
	SymmetrMatrix(int matrix_width, int block_width);
    //Деструктор симметричной матрицы
	~SymmetrMatrix();

	int getValue(int row, int column);//Получение элемента из матрицы
	void setValue(int row, int column, int value);//Установка значения конкретного элемента матрицы
	int *getBlock(int blockY, int blockX);//Получение блока
	void readMatrixFromFile(std::string filePath);//Чтение матрицы из файла
	void printMatrix();//Вывод матрицы на печать
	void generateMatrix(int min_val, int max_val);

	int*tmp;
};

int SymmetrMatrix::getFirstElementBlock(int blockY, int blockX)
{
	// По строчкам
	int block_number = blockY * width_matrix_in_blocks - ((2.0 + (double)blockY - 1) / 2.0)*(double)blockY + blockX;
	return block_number * block_capacity;
}

int SymmetrMatrix::getElementBlock(int row, int column)
{
	int element_number = (row%block_width) * block_width + (column%block_width);

	int blockX = column / block_width;
	int blockY = row / block_width;

	int memory_shift = getFirstElementBlock(blockY, blockX);
	return element_number + memory_shift;
}

void SymmetrMatrix::zerofication()
{
	for (int i = 0; i < matrix_capacity; i++)
		data[i] = 0;
}

SymmetrMatrix::SymmetrMatrix(int matrix_width, int block_width)
{
	this->block_width = block_width;
	this->matrix_width = matrix_width;

	width_matrix_in_blocks = matrix_width / block_width;

	block_quantity = width_matrix_in_blocks * width_matrix_in_blocks;
	block_capacity = block_width * block_width;

	int symmetric_block_quantity = 0;

	for (int x = 0; x < matrix_width / block_width; x++)
		for (int y = 0; y < matrix_width / block_width; y++)
			if (x < y) symmetric_block_quantity++;

	matrix_capacity = (block_quantity - symmetric_block_quantity) * block_capacity;
	data = new int[matrix_capacity];

	tmp = new int[block_capacity];

	zerofication();
}

SymmetrMatrix::~SymmetrMatrix()
{
	delete[] data;
	delete[] tmp;
}

int SymmetrMatrix::getValue(int row, int column)
{
	if (column < row)
	{
		std::swap(row, column);
	}
	return data[getElementBlock(column, row)];
}

void SymmetrMatrix::setValue(int row, int column, int new_value)
{
	data[getElementBlock(row, column)] = new_value;
}

int* SymmetrMatrix::getBlock(int blockY, int blockX)
{
	if (blockX > blockY)
	{
		std::swap(blockX, blockY);

		int* block = data + getFirstElementBlock(blockY, blockX);

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
		int* block = data + getFirstElementBlock(blockY, blockX);
		return block;
	}
}


void SymmetrMatrix::readMatrixFromFile(std::string filePath) {
	zerofication();
	std::fstream in(filePath);
	std::string line;
	int i = 0;

	while (std::getline(in, line)) {
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

	//if (in.is_open()) {
		in.close();
	//}
}

void SymmetrMatrix::printMatrix() {

	for (int i = 0;i < matrix_width;++i) {
		for (int j = 0;j < matrix_width;++j)
			std::cout << getValue(i, j) << " ";
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void SymmetrMatrix::generateMatrix(int min_val, int max_val)
{
	std::mt19937 gen(time(0));
	std::uniform_int_distribution<> uid(min_val, max_val);

	for (int i = 0; i < matrix_width; i++)
		for (int j = i; j < matrix_width; ++j)
		{
			int rnd_num = uid(gen);
			//this->setValue(j, i, rnd_num);
			this->setValue(i, j, rnd_num);
			this->setValue(j, i, rnd_num);
		}

}


