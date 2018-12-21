template <class T> class UpperTriangleMatrix {
private:
  T *data;
  T *zeroBlock;
  int blockLinearCount, blockSize, blockCapacity, matrixSize;
  int getBlockLeftUpperCornerLinearNumber(int row, int col);
  int getLinearNumberByPosition(int row, int col);
  void reset();
  void printBlock(T * block);

public:
  UpperTriangleMatrix(int matrixSize, int blockSize);
  ~UpperTriangleMatrix();
  T *getBlockAt(int blockY, int blockX);
  T getValueAt(int row, int col);
  void readMatrixFromFile(std::string filePath);
  void setValueAt(int row, int col, T value);
};

template <class T>
UpperTriangleMatrix<T>::UpperTriangleMatrix(int matrixSize, int blockSize)
    : blockSize(blockSize), matrixSize(matrixSize) {
  blockLinearCount = matrixSize / blockSize;
  blockCapacity = blockSize * blockSize;

  data = new T[blockLinearCount * (blockLinearCount + 1) / 2 * blockSize *
               blockSize];
  zeroBlock = new T[blockCapacity];
  reset();
}

template <class T> void UpperTriangleMatrix<T>::reset() {
  int dataSize = blockLinearCount * (blockLinearCount + 1) / 2 * blockSize * blockSize;
  for (auto i = 0; i < dataSize; i++) data[i] = 0;
  for (auto i = 0; i < blockCapacity; i++) zeroBlock[i] = 0;
}

template <class T> UpperTriangleMatrix<T>::~UpperTriangleMatrix() {
  delete[] data;
  delete[] zeroBlock;
}

template <class T> T UpperTriangleMatrix<T>::getValueAt(int row, int col) {
  if (col > row) {
    return 0;
  }
  return data[getLinearNumberByPosition(row, col)];
}

template <class T> void UpperTriangleMatrix<T>::readMatrixFromFile(std::string filePath) {
  reset();
  std::fstream in(filePath);
  std::string line;
  int i = 0;

  while (std::getline(in, line))
  {
    
    int j = 0;
    std::stringstream linestream(line);

    T value;
    while (linestream >> value)
    {
      setValueAt(i, j, value);
      ++j;
    }
    ++i;
  }

  if (in.is_open()) {
    in.close();
  }
}

template <class T>
void UpperTriangleMatrix<T>::setValueAt(int row, int col, T newValue) {
  if (col > row) {
    return;
  }
  data[getLinearNumberByPosition(row, col)] = newValue;
}

template <class T> T *UpperTriangleMatrix<T>::getBlockAt(int blockY, int blockX) {
  T * block;
  if (blockX > blockY) {
    block = zeroBlock;
  } else {
    block = data + getBlockLeftUpperCornerLinearNumber(blockY, blockX);
  }
  return block;
}

template <class T>
int UpperTriangleMatrix<T>::getBlockLeftUpperCornerLinearNumber(int blockY, int blockX) {
  int sum = (2 * blockLinearCount - (blockX - 1));
  int outerNumber = blockX * (sum / 2.0) + (blockY - blockX);
  return outerNumber * blockCapacity;
}

template <class T>
int UpperTriangleMatrix<T>::getLinearNumberByPosition(int row, int col) {
  int innerNumber = (row % blockSize) * blockSize + (col % blockSize);
  int blockY = row / blockSize;
  int blockX = col / blockSize;
  int outerNumber = getBlockLeftUpperCornerLinearNumber(blockY, blockX);
  return innerNumber + outerNumber;
}

template <class T> void UpperTriangleMatrix<T>::printBlock(T * block) {
  for(int i = 0; i < blockSize; i++)
  {
    for(int j = 0; j < blockSize; j++)
    {
      std::cout << block[i * blockSize + j] << ' ';
    }
    std::cout << "| ";
  }
  std::cout << "\n";
}
