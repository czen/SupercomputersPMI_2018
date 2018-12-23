#pragma once
class SymmetricMatrix {
private:
  double *data;
  double *tmpBlock;
  int blockLinearCount, blockLen, blockCapacity, matrixSize;

  int getBlockLeftUpperCornerLinearNumber(int row, int column);
  int getLinearNumberByPosition(int row, int column);
  void cleanByZero();

public:
  SymmetricMatrix(int matrixSize, int blockLen);
  ~SymmetricMatrix();

  double getValue(int row, int column);
  void setValue(int row, int column, double value);
  double *getBlock(int blockY, int blockX);
  void readMatrixFromFile(std::string filePath);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int SymmetricMatrix::getBlockLeftUpperCornerLinearNumber(int blockY, int blockX) {
	int sum = (2 * blockLinearCount - (blockX - 1));
	int outerNumber = blockX * (sum / 2.0) + (blockY - blockX);
	return outerNumber * blockCapacity;
}

int SymmetricMatrix::getLinearNumberByPosition(int row, int column) {
	int innerNumber = (row % blockLen) * blockLen + (column % blockLen);
	int blockX = column / blockLen;
	int blockY = row / blockLen;
	int outerNumber = getBlockLeftUpperCornerLinearNumber(blockY, blockX);
	return innerNumber + outerNumber;
}

void SymmetricMatrix::cleanByZero() {
	int dataSize = blockLinearCount * (blockLinearCount + 1) / 2 * blockLen * blockLen;
	for (auto i = 0; i < dataSize; i++) data[i] = 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SymmetricMatrix::SymmetricMatrix(int matrixSize, int blockLen) : blockLen(blockLen), matrixSize(matrixSize) {
	blockLinearCount = matrixSize / blockLen;
	blockCapacity = blockLen * blockLen;

	data = new double[blockLinearCount * (blockLinearCount + 1) / 2 * blockLen *blockLen];
	tmpBlock = new double[blockCapacity];
}

SymmetricMatrix::~SymmetricMatrix() {
	delete[] data;
	delete[] tmpBlock;
}

double SymmetricMatrix::getValue(int row, int column) {
	if (column > row) {
		std::swap(column, row);
	}
	return data[getLinearNumberByPosition(row, column)];
}

void SymmetricMatrix::setValue(int row, int column, double newValue) {
	data[getLinearNumberByPosition(row, column)] = newValue;
}

double* SymmetricMatrix::getBlock(int blockY, int blockX) {
	if (blockX > blockY) {
		std::swap(blockX, blockY);
	}

	double * block = data + getBlockLeftUpperCornerLinearNumber(blockY, blockX);
	return block;
}

void SymmetricMatrix::readMatrixFromFile(std::string filePath) {
	cleanByZero();
	std::fstream in(filePath);
	std::string line;
	int i = 0;

	while (std::getline(in, line)) {
		int j = 0;
		std::stringstream linestream(line);

		double value;
		while (linestream >> value) {
			setValue(i, j, value);
			++j;
		}
		++i;
	}

	if (in.is_open()) {
		in.close();
	}
}