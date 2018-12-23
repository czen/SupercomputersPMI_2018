#pragma once
#define MATRIX_SIZE 1440

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <omp.h>
#include <memory>
#include <cmath>
#include <assert.h>

#include "SymmetricMatrix.h"
#include "UpperTriangleMatrix.h"
#include "ResultMatrix.h"
#include "Multipliers.h"


auto blocks = { 6, 8, 9, 10, 12, 15, 16, 18, 20, 24, 30, 
32, 36, 40, 45, 48, 60, 72, 80, 90, 96,  120,  144,  160,  180,  
240,  288,  360,  480,  720, 1440 };

std::string matrixFile(int blockLen, std::string fname){
	return "/Matrix/size" + std::to_string(blockLen) + "/" + fname + ".txt";
}

int main() {
	double startTime[3], endTime[3];

	for (auto blockLen : blocks) {
		std::cout << "Block length: " << blockLen << std::endl;

		SymmetricMatrix *A = new SymmetricMatrix(MATRIX_SIZE, blockLen);
		A->readMatrixFromFile(matrixFile(blockLen, "Matrix_A"));

		UpperTriangleMatrix *B = new UpperTriangleMatrix(MATRIX_SIZE, blockLen);
		B->readMatrixFromFile(matrixFile(blockLen, "Matrix_B"));

		ResultMatrix *C = new ResultMatrix(MATRIX_SIZE, blockLen);
		C->cleanByZero();

		startTime[0] = omp_get_wtime();
		serialMultiplication(A, B, C);
		endTime[0] = omp_get_wtime();
		std::cout << "Serial: " << endTime[0] - startTime[0] << std::endl;
		if (C->compareResult(matrixFile(blockLen, "Matrix_C")) == false) printf("Bad result!\n");
		C->cleanByZero();

		startTime[1] = omp_get_wtime();
		twoBlocksMultiplication(A, B, C);
		endTime[1] = omp_get_wtime();
		std::cout << "2 blocks: " << endTime[1] - startTime[1] << std::endl;
		if (C->compareResult(matrixFile(blockLen, "Matrix_C")) == false) printf("Bad result!\n");
		C->cleanByZero();

		startTime[2] = omp_get_wtime();
		twoRandomBlocksMultiplication(A, B, C);
		endTime[2] = omp_get_wtime();
		std::cout << "2 rand blocks: " << endTime[2] - startTime[2] << std::endl;
		if (C->compareResult(matrixFile(blockLen, "Matrix_C"))== false) printf("Bad result!\n");
		C->cleanByZero();

		delete A;
		delete B;
		delete C;
		std::cout << "===============================" << std::endl;
	}
	system("pause");
	return 0;
}
