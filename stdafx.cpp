// stdafx.cpp : source file that includes just the standard includes
// $safeprojectname$.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
#include <iostream>
#include <omp.h>
using namespace std;

void goProgram()
{
	int matrix_size = 1024;
	cout << "Matrix size = " << matrix_size << endl;
	int block_size = 8;
	while (block_size<1024) {
		cout << "Block's size = " << block_size << endl;
		int size = matrix_size / block_size;
		trmat a;
		a.size = size;
		a.blocksize = block_size;
		a.colomn = new matrix_colomn[size];

		//инициализация
		for (int i = 0; i < size; i++)
		{
			a.colomn[i].blocks = new matrix_block[size - i];
			for (int j = 0; j < size - i; j++)
			{
				int s = block_size*block_size;
				a.colomn[i].blocks[j].value = new int[s];
				for (int k = 0; k < s; ++k) a.colomn[i].blocks[j].value[k] = 0;
			}
		}
		//заполнение
		int i, j, k;
		for (i = 0; i < a.size; i++)
			for (j = 0; j < a.size - i; j++)
				for (k = 0; k < a.blocksize*a.blocksize; k++) {
					a.colomn[i].blocks[j].value[k] = rand() % 10 + 1;
				}
		for (i = 0; i < a.size; i++)
			for (j = 1; j < a.blocksize; j++)
				for (k = 0; k < j; k++)
					a.colomn[i].blocks[0].value[j*a.blocksize + k] = 0;
		trmat b;
		b.size = size;
		b.blocksize = block_size;
		b.colomn = new matrix_colomn[size];
		for (int i = 0; i < size; i++)
		{
			b.colomn[i].blocks = new matrix_block[size];
			for (int j = 0; j < size; j++)
			{
				int s = block_size*block_size;
				b.colomn[i].blocks[j].value = new int[s];
				for (int k = 0; k < s; ++k) b.colomn[i].blocks[j].value[k] = 0;
			}
		}
		for (i = 0; i < b.size; i++)
			for (j = 0; j < b.size; j++)
				for (k = 0; k < b.blocksize*b.blocksize; k++) {
					b.colomn[i].blocks[j].value[k] = rand() % 10 + 1;
				}
		for (i = 0; i < b.size; i++)
			for (j = 1; j < b.blocksize; j++)
				for (k = 0; k < j; k++)
					b.colomn[i].blocks[0].value[j*b.blocksize + k] = 0;
		block_matrix c;
		c.size = size;
		c.blocksize = block_size;
		c.colomn = new matrix_colomn[size];
		for (i = 0; i < size; i++)
		{
			c.colomn[i].blocks = new matrix_block[size];
			for (int j = 0; j < size; j++)
			{
				int s = block_size*block_size;
				c.colomn[i].blocks[j].value = new int[s];
				for (int k = 0; k < s; ++k) c.colomn[i].blocks[j].value[k] = 0;
			}
		}
		block_matrix c1;
		c1.size = size;
		c1.blocksize = block_size;
		c1.colomn = new matrix_colomn[size];
		for (i = 0; i < size; i++)
		{
			c1.colomn[i].blocks = new matrix_block[size];
			for (int j = 0; j < size; j++)
			{
				int s = block_size*block_size;
				c1.colomn[i].blocks[j].value = new int[s];
				for (int k = 0; k < s; ++k) c1.colomn[i].blocks[j].value[k] = 0;
			}
		}
		double start_time, end_time;
		start_time = omp_get_wtime();
		multParallel(a, b, c);
		end_time = omp_get_wtime();
		cout << "Parallel time = " << end_time - start_time << endl;
		multNonParallel(a, b, c1);
		end_time = omp_get_wtime();
		cout << "NonParallel time = " << end_time - start_time << endl;
		block_size *= 2;
	}
	system("pause");
}

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file
