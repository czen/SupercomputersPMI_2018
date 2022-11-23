#pragma once
#include <iostream>

class VTMatrix
{
private:
	int *data;//������ ������
	int *zero_block;//������� ����

	int block_quantity;//���-�� ������
	int	block_width;//������ ����� � ���������
	int	block_capacity;//���-�� ��������� � �����
	int	matrix_width;//������ ������� � ���������
	int	matrix_capacity;//���-�� ��������� � �������
	int	width_matrix_in_blocks;//������ ������� � ������

	//��������� ������� ������� �������� ����������� �����
	int getFirstElementBlock(int blockY, int blockX);
	//��������� ������� �������� ��� ������ ����� ������������ ���������� ��������� � �������
	int getElementBlock(int row, int column);
	//��������� ���� ��������� �������
	void zerofication();

public:
	//����������� ������-����������� �������
	VTMatrix(int matrix_width, int block_width);
	//���������� ������-����������� �������
	~VTMatrix();

	int getValue(int row, int column);//��������� �������� �� �������
	void setValue(int row, int column, int value);//��������� �������� ����������� �������� �������
	int *getBlock(int blockY, int blockX);//��������� �����
	void readMatrixFromFile(std::string filePath);//������ ������� �� �����
	void printMatrix();//����� ������� �� ������
	void generateMatrix(int min_val, int max_val);
};

int VTMatrix::getFirstElementBlock(int blockY, int blockX)
{
	// �� �������
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

	// ������� ������� ������ ��� ���������������� ��������� ������
	int zero_block_quantity = 0;
	for (int x = 0; x < matrix_width / block_width; x++)
		for (int y = 0; y < matrix_width / block_width; y++)
			if (x < y)
				zero_block_quantity++;

	// ��������� ������ ������������ ��� ����� ������� ������ 
	matrix_capacity = (block_quantity - zero_block_quantity) * block_capacity;
	data = new int[matrix_capacity];

	//��������� ������ ��� 1 �������� �����
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