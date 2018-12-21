#define MATRIX_SIZE 2880

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <omp.h>
#include <memory>
#include <cmath>
#include <assert.h>

#include "lib/BlockMatrix.h"
#include "lib/UpperTriangleMatrix.h"
#include "lib/SymmetricMatrix.h"


#include "lib/SequentialMultiplier.h"
#include "lib/ParallelInnerMultiplier.h"
#include "lib/ParallelOuterMultiplier.h"

auto blockSizes = {1, 6, 10,  15,  20,  24,  30,  36,  40,  60, 72,
                    80, 96, 120, 144, 160, 180, 240, 360, 480, 720,2880};

std::string matrixFile(int blockSize, std::string name)
{
  return "../python/size" + std::to_string(blockSize) + "/" + name + "_float.txt";
}

template<typename T> void calculate() {
  double startedAt[3], endedAt[3];
  bool isValid[3];

  std::cout << "Processing " << typeid(T).name() << " arrays...\n";

  for (auto blockSize : blockSizes) {
    std::cout << ' ' << blockSize << std::flush;

    UpperTriangleMatrix<T> * A = new UpperTriangleMatrix<T>(MATRIX_SIZE, blockSize);
    A->readMatrixFromFile(matrixFile(blockSize, "A"));

    SymmetricMatrix<T> * B = new SymmetricMatrix<T>(MATRIX_SIZE, blockSize);
    B->readMatrixFromFile(matrixFile(blockSize, "B"));

    BlockMatrix<T> * C = new BlockMatrix<T>(MATRIX_SIZE, blockSize);
    C->reset();

    startedAt[0] = omp_get_wtime();
    multiplicateMatrices(A, B, C);
    endedAt[0] = omp_get_wtime();
    isValid[0] = C->compareWithFile(matrixFile(blockSize, "C"));
    C->reset();

    std::cout << ' ' << endedAt[0] - startedAt[0] << std::flush;

    startedAt[1] = omp_get_wtime();
    multiplicateMatricesWithInnerThreading(A, B, C);
    endedAt[1] = omp_get_wtime();
    isValid[1] = C->compareWithFile(matrixFile(blockSize, "C"));
    C->reset();

    std::cout << ' ' << endedAt[1] - startedAt[1] << std::flush;

    startedAt[2] = omp_get_wtime();
    multiplicateMatricesWithOuterThreading(A, B, C);
    endedAt[2] = omp_get_wtime();
    isValid[2] = C->compareWithFile(matrixFile(blockSize, "C"));
    C->reset();

    std::cout << ' ' << endedAt[2] - startedAt[2] << std::endl;
    
    delete A;
    delete B;
    delete C;
  }

  std::cout << "Done!\n\n";
}

int main() {
  std::cout << "Start programm...\n";
  calculate<double>();
  calculate<float>();
  return 0;
}
