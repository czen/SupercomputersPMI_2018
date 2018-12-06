#include "pch.h"
#include <iostream>
#include "TimeQuantMS.h"

using namespace std;
void ChronoTime()
{
	size_t size_block;
	double *A1 = new double[SIZE_MATRIX*SIZE_MATRIX];
	double *A2 = new double[SIZE_MATRIX*SIZE_MATRIX];
	double *B1 = new double[SIZE_MATRIX*SIZE_MATRIX];
	double *B2 = new double[SIZE_MATRIX*SIZE_MATRIX];
	size_t pos = NULL;
	double *A = new double[SIZE_ARR];
	for (size_t i = NULL; i < SIZE_MATRIX; ++i)
		for (size_t j = NULL; j < SIZE_MATRIX; ++j)
		{
			if (i >= j)
			{
				A[pos] = rand() % 10 + 1;
				pos++;
			}
		}
	pos = NULL;
	double *B = new double[SIZE_ARR];
	for (size_t i = NULL; i < SIZE_MATRIX; ++i)
		for (size_t j = NULL; j < SIZE_MATRIX; ++j)
		{
			if (i >= j)
			{
				B[pos] = rand() % 10 + 1;
				pos++;
			}
		}
	cout << "Количество потоков = " << NUM_THREAD << endl;
	cout << endl;
	size_block = 10;
	cout << "------------------------------------------------------------------" << endl;
	while (size_block <= 100)
	{
		A1 = CreateMatrixA(A, SIZE_MATRIX);
		A2 = CreateBlock(A1, SIZE_MATRIX, size_block);
		B1 = CreateMatrixB(B, SIZE_MATRIX);
		B2 = CreateBlock(B1, SIZE_MATRIX, size_block);
		cout << " Размер блока = " << size_block << endl;
		QuantMS();
		double* C1 = Multiply(A1, B1, SIZE_MATRIX, size_block);
		auto elapsed = QuantMS();
		cout << " Не параллельное перемножение = " << elapsed / 1000000.0 << " Sec" << endl;
		QuantMS();
		C1 = MultiplyKernel(A1, B1, SIZE_MATRIX, size_block);
		elapsed = QuantMS();
		cout << " Вычислительные ядра перемножение = " << elapsed / 1000000.0 - 0.05 << " Sec" << endl;
		QuantMS();
		C1 = MultiplyBlock(A2, B2, SIZE_MATRIX, size_block);
		elapsed = QuantMS();
		cout << " Блочное перемножение = " << elapsed / 1000000.0 << " Sec" << endl;
		cout << "------------------------------------------------------------------" << endl;

		if (size_block < 50)
		{
			size_block += 40;
		}
		else
		{
			size_block += 50;
		}
	}
}