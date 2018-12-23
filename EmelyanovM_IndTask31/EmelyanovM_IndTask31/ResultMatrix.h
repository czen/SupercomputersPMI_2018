#pragma once
class ResultMatrix {
private:
  double *data;
  int blockLinearCount, blockLen, blockCapacity, matrixSize;

  int getBlockLeftUpperCornerLinearNumber(int row, int column);
  int getLinearNumberByPosition(int row, int column);
public:
  ResultMatrix(int matrixSize, int blockLen);
  ResultMatrix(const ResultMatrix &other) = delete;
  ~ResultMatrix();

  double getValue(int row, int column);
  void setValue(int row, int column, double value);
  double *getBlock(int blockY, int blockX);
  int getLinearBlockCount();
  int getBlockSize();
  void cleanByZero();
  bool compareResult(std::string filePath);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int ResultMatrix::getBlockLeftUpperCornerLinearNumber(int blockY, int blockX) {
	int outerNumber = blockX * blockLinearCount + blockY;
	return outerNumber * blockCapacity;
}

int ResultMatrix::getLinearNumberByPosition(int row, int column) {
	int innerNumber = (row % blockLen) * blockLen + (column % blockLen);
	int blockY = row / blockLen;
	int blockX = column / blockLen;
	int outerNumber = getBlockLeftUpperCornerLinearNumber(blockY, blockX);
	return innerNumber + outerNumber;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ResultMatrix::ResultMatrix(int matrixSize, int blockLen): blockLen(blockLen), matrixSize(matrixSize) {
  blockLinearCount = matrixSize / blockLen;
  blockCapacity = blockLen * blockLen;
  data = new double[blockLinearCount * blockLinearCount * blockCapacity];
}

ResultMatrix::~ResultMatrix() {
  delete[] data;
}

double ResultMatrix::getValue(int row, int column) {
  int dataSize = blockLinearCount * blockLinearCount * blockCapacity;
  assert(dataSize > getLinearNumberByPosition(row, column));
  return data[getLinearNumberByPosition(row, column)];
}

void ResultMatrix::setValue(int row, int column, double newValue) {
  data[getLinearNumberByPosition(row, column)] = newValue;
}

double *ResultMatrix::getBlock(int blockY, int blockX) {
  auto block = data + getBlockLeftUpperCornerLinearNumber(blockY, blockX);
  return block;
}

int ResultMatrix::getLinearBlockCount() {
  return blockLinearCount;
}

int ResultMatrix::getBlockSize() {
  return blockLen;
}

void ResultMatrix::cleanByZero() {
  int dataSize = blockLinearCount * blockLinearCount * blockCapacity;
  for (auto i = 0; i < dataSize; i++) data[i] = 0;
}

bool ResultMatrix::compareResult(std::string filePath) {
	std::ifstream file(filePath);
	bool flag = true;
	const double eps = 1e-6;

	if (file.is_open()) {
		for (int i = 0; i < matrixSize; i++) {
			for (int j = 0; j < matrixSize; j++) {
				double val;
				double mVal = getValue(i, j);
				file >> val;
				flag = std::fabs(mVal - val) < eps;
				if (flag==false) {
					if (file.is_open()) {
						file.close();
					}
					return false;
				}
			}
		}
	}

	if (file.is_open()) {
		file.close();
	}
	return flag;
}