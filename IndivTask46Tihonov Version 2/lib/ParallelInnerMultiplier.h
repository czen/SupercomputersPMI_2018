template <class T>
void calculateBlockInParallel(T *blockA, T *blockB, T *blockC, int blockSize) {
  #pragma omp parallel for collapse(2)
  for (auto i = 0; i < blockSize; i++) {
    for (auto j = 0; j < blockSize; j++) {
      for (auto k = 0; k < blockSize; k++) {
        blockC[i * blockSize + j] += blockA[i * blockSize + k] * blockB[k * blockSize + j];
      }
    }
  }
}

template <class T>
void multiplicateMatricesWithInnerThreading(UpperTriangleMatrix<T> * A, SymmetricMatrix<T> * B,
                          BlockMatrix<T> * C) {
  auto blockCount = C->getLinearBlockCount();
  auto blockSize = C->getBlockSize();

  for (auto i = 0; i < blockCount; i++) {
    for (auto j = 0; j < blockCount; j++) {
      for (auto k = 0; k < blockCount; k++) {
        calculateBlockInParallel(A->getBlockAt(i, k), B->getBlockAt(k, j),
                       C->getBlockAt(i, j), blockSize);
      }
    }
  }
}
