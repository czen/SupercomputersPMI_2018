template <class T>
void multiplicateMatricesWithOuterThreading(UpperTriangleMatrix<T> * A, SymmetricMatrix<T> * B,
                          BlockMatrix<T> * C) {
  auto blockCount = C->getLinearBlockCount();
  auto blockSize = C->getBlockSize();

  #pragma omp parallel for collapse(2)
  for (auto i = 0; i < blockCount; i++) {
    for (auto j = 0; j < blockCount; j++) {
      for (auto k = 0; k < blockCount; k++) {
        calculateBlock(A->getBlockAt(i, k), B->getBlockAt(k, j),
                       C->getBlockAt(i, j), blockSize);
      }
    }
  }
}
