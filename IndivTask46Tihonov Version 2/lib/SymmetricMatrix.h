template <class T> class SymmetricMatrix {
private:
  T * data;
  T * tmpBlock;
  int blockLinearCount, blockSize, blockCapacity, matrixSize;
  int getBlockLeftUpperCornerLinearNumber(int row, int col);
  int getLinearNumberByPosition(int row, int col);
  void printBlock(T * block);
  void reset();

public:
  SymmetricMatrix(int matrixSize, int blockSize);
  ~SymmetricMatrix();
  void readMatrixFromFile(std::string filePath);
  T *getBlockAt(int blockY, int blockX);
  T getValueAt(int row, int col);
  void setValueAt(int row, int col, T value);
};

template <class T> void SymmetricMatrix<T>::readMatrixFromFile(std::string filePath) {
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
SymmetricMatrix<T>::SymmetricMatrix(int matrixSize, int blockSize)
    : blockSize(blockSize), matrixSize(matrixSize) {
  blockLinearCount = matrixSize / blockSize;
  blockCapacity = blockSize * blockSize;

  data = new T[blockLinearCount * (blockLinearCount + 1) / 2 * blockSize *
               blockSize];
  tmpBlock = new T[blockCapacity];               
}

template <class T> void SymmetricMatrix<T>::reset() {
  int dataSize = blockLinearCount * (blockLinearCount + 1) / 2 * blockSize * blockSize;
  for (auto i = 0; i < dataSize; i++) data[i] = 0;
}

template <class T> SymmetricMatrix<T>::~SymmetricMatrix() {
  delete[] data; 
  delete[] tmpBlock;
}

template <class T> T SymmetricMatrix<T>::getValueAt(int row, int col) {
  if (col > row) {
    std::swap(col, row);
  }
  return data[getLinearNumberByPosition(row, col)];
}

template <class T>
void SymmetricMatrix<T>::setValueAt(int row, int col, T newValue) {
  data[getLinearNumberByPosition(row, col)] = newValue;
}

template <class T> T* SymmetricMatrix<T>::getBlockAt(int blockY, int blockX) {
  if (blockX > blockY) {
    std::swap(blockX, blockY);
  }

  T * block = data + getBlockLeftUpperCornerLinearNumber(blockY, blockX);
  return block;
}

template <class T>
int SymmetricMatrix<T>::getBlockLeftUpperCornerLinearNumber(int blockY, int blockX) {
  int sum = (2 * blockLinearCount - (blockX - 1));
  int outerNumber = blockX * (sum / 2.0) + (blockY - blockX);

  return outerNumber * blockCapacity;
}

template <class T>
int SymmetricMatrix<T>::getLinearNumberByPosition(int row, int col) {
  int innerNumber = (row % blockSize) * blockSize + (col % blockSize);
  int blockX = col / blockSize;
  int blockY = row / blockSize;
  int outerNumber = getBlockLeftUpperCornerLinearNumber(blockY, blockX);
  return innerNumber + outerNumber;
}

template <class T> void SymmetricMatrix<T>::printBlock(T * block) {
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
