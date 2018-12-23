#pragma once
inline void calculateBlock(double *blockA, double *blockB, double *blockC, int blockLen) {
  for (int i = 0; i < blockLen; i++) 
    for (int j = 0; j < blockLen; j++) 
      for (int k = 0; k < blockLen; k++) {
         double total = blockA[i * blockLen + k] * blockB[k * blockLen + j]; 
		 blockC[i * blockLen + j] += total;
      }
}

void calculateBlockInParallel(double *blockA, double *blockB, double *blockC, int blockLen) {
#pragma omp parallel for 
	for (auto i = 0; i < blockLen; i++)
		for (auto j = 0; j < blockLen; j++)
			for (auto k = 0; k < blockLen; k++)
				blockC[i * blockLen + j] += blockA[i * blockLen + k] * blockB[k * blockLen + j];
}

void serialMultiplication(SymmetricMatrix *A, UpperTriangleMatrix *B, ResultMatrix *C) {
  int blockCount = C->getLinearBlockCount();
  int blockLen = C->getBlockSize();

  for (int i = 0; i < blockCount; i++) 
    for (int j = 0; j < blockCount; j++) 
      for (int k = 0; k < blockCount; k++) 
        calculateBlock(A->getBlock(i, k), B->getBlock(k, j), C->getBlock(i, j), blockLen);
}

void twoBlocksMultiplication(SymmetricMatrix *A, UpperTriangleMatrix *B, ResultMatrix *C) {
	auto blockCount = C->getLinearBlockCount();
	auto blockLen = C->getBlockSize();

	for (auto i = 0; i < blockCount; i++)
		for (auto j = 0; j < blockCount; j++)
			for (auto k = 0; k < blockCount; k++)
				calculateBlockInParallel(A->getBlock(i, k), B->getBlock(k, j), C->getBlock(i, j), blockLen);
}

void twoRandomBlocksMultiplication(SymmetricMatrix *A, UpperTriangleMatrix *B, ResultMatrix *C) {
	auto blockCount = C->getLinearBlockCount();
	auto blockLen = C->getBlockSize();

#pragma omp parallel for 
	for (auto i = 0; i < blockCount; i++)
		for (auto j = 0; j < blockCount; j++)
			for (auto k = 0; k < blockCount; k++)
				calculateBlock(A->getBlock(i, k), B->getBlock(k, j), C->getBlock(i, j), blockLen);
}


