#pragma once
class ResultMatrix
{
private:
	int *data;//������ ������

	int block_quantity;//���-�� ������
	int	block_width;//������ ����� � ���������
	int	block_capacity;//���-�� ��������� � �����
	int	matrix_width;//������ ������� � ���������
	int	matrix_capacity;//���-�� ��������� � �������
	int	width_matrix_in_blocks;//������ ������� � ������

	//��������� ������� ������� �������� ����������� �����
	int getFirstElementBlock(int row, int column);
	//��������� ������� �������� ��� ������ ����� ������������ ���������� ��������� � �������
	int getElementBlock(int row, int column);

public:
	
	ResultMatrix(int matrix_width, int block_width);//����������� �������������� �������
	~ResultMatrix();//���������� �������������� �������

	int getValue(int row, int column);//��������� �������� �� �������
	int *getBlock(int blockY, int blockX);//��������� �����
	
	int	getWidthMatrixInBlocks();//��������� ������ ������� � ������
	int getBlockCount();//��������� ���-�� ������
	int getBlockWidth();//��������� ������ ������ �����
	void zerofication();//��������� ���� ��������� �������
	void printMatrix();
};


int ResultMatrix::getValue(int row, int column)
{
	return data[getElementBlock(row, column)];
}

int* ResultMatrix::getBlock(int blockY, int blockX)
{
	int* block = data + getFirstElementBlock(blockY, blockX);
	return block;
}

int ResultMatrix::getBlockCount()
{
	return block_quantity;
}

int ResultMatrix::getBlockWidth()
{
	return block_width;
}

int	ResultMatrix::getWidthMatrixInBlocks()
{
	return width_matrix_in_blocks;
}

void ResultMatrix::zerofication()
{
	for (int i = 0; i < matrix_capacity; i++)
		data[i] = 0;
}
 
int ResultMatrix::getFirstElementBlock(int blockY, int blockX)
{
	int block_number = blockX * width_matrix_in_blocks + blockY;
	return block_number * block_capacity;
}

int ResultMatrix::getElementBlock(int row, int column)
{
	// �������� ������ ����� �������� �� �������� 
	int element_number = (row % block_width) * block_width + (column % block_width);

	int blockY = row / block_width;
	int blockX = column / block_width;

	int memory_shift = getFirstElementBlock(blockY, blockX);
	return memory_shift + element_number;
}

ResultMatrix::ResultMatrix(int matrix_width, int block_width)
{
	this->block_width = block_width;
	this->matrix_width = matrix_width;

	width_matrix_in_blocks = matrix_width / block_width;

	block_quantity = (width_matrix_in_blocks) * (width_matrix_in_blocks);
	block_capacity = block_width * block_width;

	matrix_capacity = block_quantity * block_capacity;
	data = new int[matrix_capacity];
}

ResultMatrix::~ResultMatrix() {
	delete[] data;
}

void ResultMatrix::printMatrix() {
	{
		for (int i = 0;i < matrix_width;++i) {
			for (int j = 0;j < matrix_width;++j)
				std::cout << getValue(i, j) << " ";
			std::cout << std::endl;
		}
	}
	std::cout << std::endl;
}