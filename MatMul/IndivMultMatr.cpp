/*Задание 15.
Матрица A верхне-треугольная. Хранится в виде одномерного массива
по блочным столбцам.
Матрица B верхне-треугольная. Хранится в виде одномерного массива
по блочным строкам.*/

#include <iostream>
#include <omp.h>
#include <random>
#include <algorithm>
#include <fstream>
#include <ctime>
#include <chrono>
std::ofstream fout("block.txt");

void createMatrix(int **matr, int sz) {
	//генерация случайных элементов матрицы
	unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine random0(seed1);
	std::uniform_int_distribution<int> distribution(1, 5);
	int s;
	for (int i(0); i < sz; ++i) {
		for (int j(0); j < i; ++j) matr[i][j] = 0;
		for (int j(i); j < sz; ++j) {
			s = distribution(random0);
			matr[i][j] = s;
		}
	}
}

void outputMatrix(int **matr, int sz) {
	for (int i(0); i < sz; ++i) {
		for (int j(0); j < sz; ++j) {
			printf("%4d", matr[i][j]);
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void inputVecA(int *vec, int sz, int szBlock) {
	int **matr = new int *[sz];
	for (int i(0); i < sz; ++i) {
		matr[i] = new int[sz];
	}
	createMatrix(matr, sz);
	//outputMatrix(matr, sz);
	int count = (int)sz / szBlock;
	int ii(0);
	try {
		for (int temp0(0); temp0 < count; ++temp0) {
			for (int temp1(temp0); temp1 < count; ++temp1)
				for (int i(0); i < szBlock; ++i) {
						for (int j(0); j < szBlock; ++j) {
							//проверяет выход за границу вектора
							if (ii == _msize(vec) / sizeof(int)) throw - 1;
							vec[ii++] = matr[j + temp0 * szBlock][i + temp1 * szBlock];
						}
					
				}
		}
	}
	catch (int e) {
		std::cout << "Caught exception: out of bounds array in function 'inputVecA' " << std::endl;
	}
	for (int i(0); i < sz; ++i)
		delete[] matr[i];
	delete[] matr;
}

void inputVecB(int *vec, int sz, int szBlock) {
	int **matr = new int *[sz];
	for (int i(0); i < sz; ++i) {
		matr[i] = new int[sz];
	}
	createMatrix(matr, sz);
	//outputMatrix(matr, sz);
	int count = (int)sz / szBlock;
	int ii(0);
	try {
		for (int temp0(0); temp0 < count; ++temp0) {
			for (int temp1(temp0); temp1 < count; ++temp1) {
				for (int i(0); i < szBlock; ++i) {
					for (int j(0); j < szBlock; ++j) {
						//проверяет выход за границу вектора
						if (ii == _msize(vec) / sizeof(int)) throw - 1;
						vec[ii++] = matr[i + temp0 * szBlock][j + temp1 * szBlock];
					}
				}
			}
		}
	}
	catch (int e) {
		std::cout << "Caught exception: out of bounds array in function 'inputVecB' " << std::endl;
	}
	for (int i(0); i < sz; ++i)
		delete[] matr[i];
	delete[] matr;
}

void outputResult(int* vec, int sz, int szBlock) {
	for (int j(0); j < _msize(vec) / sizeof(int); ++j) {
		std::cout << vec[j] << " ";
	}
	std::cout << std::endl << std::endl;
}

	//Функция перемножения двух блоков (А и В)
	void twoBlockMultiply(int* block, const int* vecA, const int* vecB, int szBlock){
		int bl;
		for (int i(0); i < szBlock; ++i){
			for (int j(0); j < szBlock; ++j){
				bl = 0;
				for (int k(0); k < szBlock; ++k){
					bl += vecA[k*szBlock + i] * vecB[k*szBlock + j];
				}
				block[i*szBlock + j] = bl;
			}
		}
	}

	//реализация умножения
	void multiply(int* vecA, int* vecB, int* vecRes, int sz, int szBlock) {
		double endTime, startTime, resTime;
		int count = (int)sz / szBlock;
		int *blockA = new int[szBlock*szBlock];
		int *blockB = new int[szBlock*szBlock];
		int *block = new int[szBlock*szBlock];
		int tempCount = 0;
		startTime = omp_get_wtime();
		for (int i(0); i < count; ++i)
			for (int j(i); j < count; ++j) {
				int *blockRes = vecRes + i * count*szBlock + j * szBlock * szBlock;
				for (int ii(0); ii < szBlock*szBlock; ii++)
					blockRes[ii] = 0;

				for (int jj(0); jj < szBlock*szBlock; jj++) {
					blockA[jj] = vecA[tempCount*szBlock*szBlock + jj];
					blockB[jj] = vecB[tempCount*szBlock*szBlock + jj];
				}
				twoBlockMultiply(block, blockA, blockB, szBlock);

				for (int ij(0); ij < szBlock*szBlock; ij++)
					blockRes[ij] += block[ij];
				tempCount++;

			}
		endTime = omp_get_wtime();
		resTime = endTime - startTime;
		fout << "Time1: " << resTime << "; ";

		delete[] blockA;
		delete[] blockB;
		delete[] block;
	}

	//реализация параллельного умножения
	void multiplyParallel(int* vecA, int* vecB, int* vecRes, int sz, int szBlock) {
		double endTime, startTime, resTime;
		int *blockA = new int[szBlock*szBlock];
		int *blockB = new int[szBlock*szBlock];
		int *block = new int[szBlock*szBlock];
		int tempCount = 0;
		startTime = omp_get_wtime();
#pragma omp parallel num_threads(8)
		{
			int count = (int)sz / szBlock;
#pragma omp for  schedule(static)
			for (int i(0); i < count; ++i) {
				for (int j(i); j < count; ++j) {
					int *blockRes = vecRes + i * count*szBlock + j * szBlock * szBlock;
					for (int ii(0); ii < szBlock*szBlock; ii++)
						blockRes[ii] = 0;

					for (int jj(0); jj < szBlock*szBlock; jj++) {
						blockA[jj] = vecA[tempCount*szBlock*szBlock + jj];
						blockB[jj] = vecB[tempCount*szBlock*szBlock + jj];
					}
					twoBlockMultiply(block, blockA, blockB, szBlock);

					for (int ij(0); ij < szBlock*szBlock; ij++)
						blockRes[ij] += block[ij];
					tempCount++;

				}

			}
		}
		endTime = omp_get_wtime();
		resTime = endTime - startTime;
		fout << "Time2: " << resTime << std::endl;

		delete[] blockA;
		delete[] blockB;
		delete[] block;
	}

	int main() {
		int sz = 720;
		//int szBlock = 12;
		for (int szBlock(1); szBlock <= sz; ++szBlock) {
			if (sz % szBlock == 0) {
				fout <<"block = " << szBlock << "; ";
				int *vecA = new int[sz*sz];
				int *vecB = new int[sz*sz];
				int *vecRes1 = new int[sz*sz];
				int *vecRes2 = new int[sz*sz];
				for (int i(0); i < sz*sz; ++i) {
					vecRes1[i] = 0;
					vecRes2[i] = 0;
				}
				std::cout << "MatrixA: \n";
				inputVecA(vecA, sz, szBlock);
				std::cout << "MatrixB: \n";
				inputVecB(vecB, sz, szBlock);

				std::cout << "The result of the multiplication: \n";
				multiply(vecA, vecB, vecRes1, sz, szBlock);
				//outputResult(vecRes1, sz, szBlock);

				std::cout << "The result of the parallel multiplication: \n";
				multiplyParallel(vecA, vecB, vecRes2, sz, szBlock);
				//outputResult(vecRes2, sz, szBlock);

				delete[]vecA;
				delete[]vecB;
				delete[]vecRes1;
				delete[]vecRes2;
			}
		}
		fout.close();
		system("pause");
		return 0;
	}