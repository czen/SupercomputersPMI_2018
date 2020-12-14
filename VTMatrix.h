#pragma once
#include <iostream>

class VTMatrix
{
private:
	int *data;//Чистые данные
	int *zero_block;//Нулевой блок

	int block_quantity;//Кол-во блоков
	int	block_width;//Ширина блока в элементах
	int	block_capacity;//Кол-во элементов в блоке
	int	matrix_width;//Ширина матрицы в элементах
	int	matrix_capacity;//Кол-во элементов в матрице
	int	width_matrix_in_blocks;//Ширина матрицы в блоках

	//Получение индекса первого элемента конкретного блока
	int getFirstElementBlock(int blockY, int blockX);
	//Получение индекса элемента уже внутри блока относительно глобальной нумерации в матрице
	int getElementBlock(int row, int column);
	//Зануление всех элементов матрицы
	void zerofication();

public:
	//Конструктор верхне-треугольной матрицы
	VTMatrix(int matrix_width, int block_width);
	//Деструктор верхне-треугольной матрицы
	~VTMatrix();

	int getValue(int row, int column);//Получение элемента из матрицы
	void setValue(int row, int column, int value);//Установка значения конкретного элемента матрицы
	int *getBlock(int blockY, int blockX);//Получение блока
	void readMatrixFromFile(std::string filePath);//Чтение матрицы из файла
	void printMatrix();//Вывод матрицы на печать
	void generateMatrix(int min_val, int max_val);
};

int VTMatrix::getFirstElementBlock(int blockY, int blockX)
{
	// По строкам
	int block_number = blockY * width_matrix_in_blocks - ((2.0 + (double)blockY - 1) / 2.0)*(double)blockY + blockX;
	return block_number * block_capacity;
}

int VTMatrix::getElementBlock(int row, int column)
{
	int element_number = (row % block_width) * block_width + (column % block_width);

	int blockY = row / block_width;
	int blockX = column / block_width;

	int memory_shift = getFirstElementBlock(blockY, blockX);
	return element_number + memory_shift;
}

void VTMatrix::zerofication()
{
	for (int i = 0; i < matrix_capacity; i++)
		data[i] = 0;
	for (int i = 0; i < block_capacity; i++)
		zero_block[i] = 0;
}

VTMatrix::VTMatrix(int matrix_width, int block_width)
{
	this->block_width = block_width;
	this->matrix_width = matrix_width;

	width_matrix_in_blocks = matrix_width / block_width;

	block_quantity = width_matrix_in_blocks * width_matrix_in_blocks;
	block_capacity = block_width * block_width;

	// Подсчет нулевых блоков для оптимизированной аллокации памяти
	int zero_block_quantity = 0;
	for (int x = 0; x < matrix_width / block_width; x++)
		for (int y = 0; y < matrix_width / block_width; y++)
			if (x < y)
				zero_block_quantity++;

	// Выделение памяти производится без учета нулевых блоков 
	matrix_capacity = (block_quantity - zero_block_quantity) * block_capacity;
	data = new int[matrix_capacity];

	//Выделение памяти для 1 нулевого блока
	zero_block = new int[block_capacity];
	zerofication();
}

VTMatrix::~VTMatrix()
{
	delete[] data;
	delete[] zero_block;
}

int VTMatrix::getValue(int row, int column)
{
	if (column < row)
		return 0;
	return data[getElementBlock(row, column)];
}

void VTMatrix::setValue(int row, int column, int new_value)
{
	if (column < row)
		return;
	data[getElementBlock(row, column)] = new_value;
}

int* VTMatrix::getBlock(int blockY, int blockX)
{
	int* block;

	if (blockX < blockY)
		block = zero_block;
	else
		block = data + getFirstElementBlock(blockY, blockX);
	return block;
}

void VTMatrix::readMatrixFromFile(std::string filePath)
{
	zerofication();
	std::fstream in(filePath);
	std::string line;
	int i = 0;

	while (std::getline(in, line)) {
		int j = 0;
		std::stringstream linestream(line);

		int value;
		while (linestream >> value) {
			setValue(i, j, value);
			++j;
		}
		++i;
	}

	//if (in.is_open()) {
		in.close();
	//}
}

void VTMatrix::printMatrix() {
	{
		for (int i = 0;i < matrix_width;++i) {
			for (int j = 0;j < matrix_width;++j)
				std::cout << getValue(i, j) << " ";
			std::cout << std::endl;
		}
	}
	std::cout << std::endl;
}

void VTMatrix::generateMatrix(int min_val, int max_val)
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