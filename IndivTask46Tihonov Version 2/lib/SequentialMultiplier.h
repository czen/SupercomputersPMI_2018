template <class T>
inline void calculateBlock(T *blockA, T *blockB, T *blockC, int blockSize) {
  for (int i = 0; i < blockSize; i++) {
    for (int j = 0; j < blockSize; j++) {
      for (int k = 0; k < blockSize; k++) {
        T total = blockA[i * blockSize + k] * blockB[k * blockSize + j];
        blockC[i * blockSize + j] += total;
      }
    }
  }
}

template <class T>
void multiplicateMatrices(UpperTriangleMatrix<T> * A, SymmetricMatrix<T> * B,
                          BlockMatrix<T> * C) {
  int blockCount = C->getLinearBlockCount();
  int blockSize = C->getBlockSize();

  for (int i = 0; i < blockCount; i++) {
    for (int j = 0; j < blockCount; j++) {
      for (int k = 0; k < blockCount; k++) {
        calculateBlock(A->getBlockAt(i, k), B->getBlockAt(k, j),
                       C->getBlockAt(i, j), blockSize);
      }
    }
  }
}
