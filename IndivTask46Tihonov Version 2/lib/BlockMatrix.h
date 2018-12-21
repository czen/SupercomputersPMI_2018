template <class T> class BlockMatrix {
private:
  T *data;
  int blockLinearCount, blockSize, blockCapacity, matrixSize;
  int getBlockLeftUpperCornerLinearNumber(int row, int col);
  int getLinearNumberByPosition(int row, int col);
  void printBlock(T * block);

public:
  BlockMatrix(int matrixSize, int blockSize);
  BlockMatrix(const BlockMatrix &other) = delete;
  ~BlockMatrix();
  int getLinearBlockCount();
  int getBlockSize();
  void reset();
  T *getBlockAt(int blockY, int blockX);
  T getValueAt(int row, int col);
  bool compareWithFile(std::string filePath);
  void setValueAt(int row, int col, T value);
};

template <class T>
BlockMatrix<T>::BlockMatrix(int matrixSize, int blockSize)
    : blockSize(blockSize), matrixSize(matrixSize) {
  blockLinearCount = matrixSize / blockSize;
  blockCapacity = blockSize * blockSize;
  data = new T[blockLinearCount * blockLinearCount * blockCapacity];
}

template <class T> BlockMatrix<T>::~BlockMatrix() {
  delete[] data;
}

template <class T> T BlockMatrix<T>::getValueAt(int row, int col) {
  int dataSize = blockLinearCount * blockLinearCount * blockCapacity;
  assert(dataSize > getLinearNumberByPosition(row, col));
  return data[getLinearNumberByPosition(row, col)];
}

template <class T> bool BlockMatrix<T>::compareWithFile(std::string filePath) {
  std::ifstream file(filePath);
  bool flag = true;
  const T eps = 0.0001;

  if (file.is_open()) {
    for (int i = 0; i < matrixSize; i++) {
      for (int j = 0; j < matrixSize; j++) {
        T val = 0;
        T mVal = getValueAt(i, j);
        file >> val;
        flag = std::fabs(mVal - val) < eps;
        if (!flag) {
          // std::cout << "WARNING: Matrix C is incorrect.\nExpected: " << val << ", got: " << mVal << ". Found at " << i << ", " << j << std::endl;
          if (file.is_open()) {
            file.close();
          }
          return false;
        }
      }
    }
  } else {
    std::cout << "Cannot found file: " << filePath << std::endl;
  }

  if (file.is_open()) {
    file.close();
  }

  return flag;
}

template <class T>
void BlockMatrix<T>::setValueAt(int row, int col, T newValue) {
  data[getLinearNumberByPosition(row, col)] = newValue;
}

template <class T> T *BlockMatrix<T>::getBlockAt(int blockY, int blockX) {
  auto block = data + getBlockLeftUpperCornerLinearNumber(blockY, blockX);
  return block;
}

template <class T> int BlockMatrix<T>::getLinearBlockCount() {
  return blockLinearCount;
}

template <class T> int BlockMatrix<T>::getBlockSize() {
  return blockSize;
}

template <class T>
int BlockMatrix<T>::getBlockLeftUpperCornerLinearNumber(int blockY, int blockX) {
  int outerNumber = blockX * blockLinearCount + blockY;
  return outerNumber * blockCapacity;
}

template <class T>
int BlockMatrix<T>::getLinearNumberByPosition(int row, int col) {
  int innerNumber = (row % blockSize) * blockSize + (col % blockSize);
  int blockY = row / blockSize;
  int blockX = col / blockSize;
  int outerNumber = getBlockLeftUpperCornerLinearNumber(blockY, blockX);
  return innerNumber + outerNumber;
}

template <class T> void BlockMatrix<T>::reset() {
  int dataSize = blockLinearCount * blockLinearCount * blockCapacity;
  for (auto i = 0; i < dataSize; i++) data[i] = 0;
}

template <class T> void BlockMatrix<T>::printBlock(T * block) {
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
