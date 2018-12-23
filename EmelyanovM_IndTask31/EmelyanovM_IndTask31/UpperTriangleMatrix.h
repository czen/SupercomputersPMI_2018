#pragma once
class UpperTriangleMatrix {
private:
  double *data;
  double *zeroBlock;
  int blockLinearCount, blockLen, blockCapacity, matrixSize;

  int getBlockLeftUpperCornerLinearNumber(int row, int column);
  int getLinearNumberByPosition(int row, int column);
  void cleanByZero();
public:
  UpperTriangleMatrix(int matrixSize, int blockLen);
  ~UpperTriangleMatrix();

  double getValue(int row, int column);
  void setValue(int row, int column, double value);
  double *getBlock(int blockY, int blockX);
  void readMatrixFromFile(std::string filePath);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int UpperTriangleMatrix::getBlockLeftUpperCornerLinearNumber(int blockY, int blockX) {
	int sum = (2 * blockLinearCount - (blockX - 1));
	int outerNumber = blockX * (sum / 2.0) + (blockY - blockX);
	return outerNumber * blockCapacity;
}

int UpperTriangleMatrix::getLinearNumberByPosition(int row, int column) {
	int innerNumber = (row % blockLen) * blockLen + (column % blockLen);
	int blockY = row / blockLen;
	int blockX = column / blockLen;
	int outerNumber = getBlockLeftUpperCornerLinearNumber(blockY, blockX);
	return innerNumber + outerNumber;
}

void UpperTriangleMatrix::cleanByZero() {
	int dataSize = blockLinearCount * (blockLinearCount + 1) / 2 * blockLen * blockLen;
	for (auto i = 0; i < dataSize; i++) data[i] = 0;
	for (auto i = 0; i < blockCapacity; i++) zeroBlock[i] = 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UpperTriangleMatrix::UpperTriangleMatrix(int matrixSize, int blockLen): blockLen(blockLen), matrixSize(matrixSize) {
  blockLinearCount = matrixSize / blockLen;
  blockCapacity = blockLen * blockLen;

  data = new double[blockLinearCount * (blockLinearCount + 1) / 2 * blockLen * blockLen];
  zeroBlock = new double[blockCapacity];
  cleanByZero();
}

UpperTriangleMatrix::~UpperTriangleMatrix() {
  delete[] data;
  delete[] zeroBlock;
}

double UpperTriangleMatrix::getValue(int row, int column) {
  if (column > row) 
    return 0;
  return data[getLinearNumberByPosition(row, column)];
}

void UpperTriangleMatrix::setValue(int row, int column, double newValue) {
	if (column > row)
		return;
	data[getLinearNumberByPosition(row, column)] = newValue;
}

double *UpperTriangleMatrix::getBlock(int blockY, int blockX) {
	double * block;
	if (blockX > blockY)
		block = zeroBlock;
	else
		block = data + getBlockLeftUpperCornerLinearNumber(blockY, blockX);
	return block;
}

void UpperTriangleMatrix::readMatrixFromFile(std::string filePath) {
  cleanByZero();
  std::fstream in(filePath);
  std::string line;
  int i = 0;

  while (std::getline(in, line)){
    int j = 0;
    std::stringstream linestream(line);

    double value;
    while (linestream >> value){
      setValue(i, j, value);
      ++j;
    }
    ++i;
  }

  if (in.is_open()) {
    in.close();
  }
}