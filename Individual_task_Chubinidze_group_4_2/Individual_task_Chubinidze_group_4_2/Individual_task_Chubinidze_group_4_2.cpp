// Individual_task_Chubinidze_group_4_2.cpp: определяет точку входа для консольного приложения.
//
//Задание 48
//Написать программу блочного умножения двух матриц C = A*B.
//
//Матрица A верхне - треугольная.Хранится в виде одномерного массива по блочным столбцам.
//Матрица B симметричная, хранится как верхне - треугольная.Хранится в виде одномерного массива по блочным столбцам.
//
//Распараллелить блочную программу умножения двух матриц C = A*B с использованием технологии OpenMP двумя способами:
//		Перемножение каждых двух блоков выполнить параллельно
//		В разных вычислительных ядрах одновременно перемножать разные пары блоков.
//
//Определить оптимальные размеры блоков в обоих случаях.
//Провести численные эксперименты и построить таблицу сравнений времени выполнения различных программных реализаций решения задачи.
//Определить лучшие реализации.
//Проверить корректность(правильность) программ.


#include "stdafx.h"
#include <omp.h>
#include <iostream>
#include <cmath>


double* geherate_matrix(int size) 
{
	int size_array = (size*(size + 1)) / 2;
	double* matrix = new double[size_array];
	
	for (int i = 0; i <  size_array; ++i)
	{
		matrix[i] = 1;
	}

	return matrix;
}

bool correct_answer(double*result,int size) 
{
	int current = size;
	int size_array = size*size;

	for (int i = 0; i < size_array; ++i)
	{
		if (i != 0 && i%size == 0)
			current--;

		if (result[i] != current)
			return false;
	}
	return true;

}

void non_parallel(int size, double*A, double*B) 
{

	std::cout << "\tNon parallel" << std::endl;

	int size_array = size*size;
	double* C = new double[size_array];
	for (int i = 0; i < size_array; ++i)
		C[i] = 0;

	int j = 0, k = 0, i = 0, c = 0;

	double start_time = omp_get_wtime();

	for (; i < size_array; ++i)
	{
		if (i != 0 && i%size == 0)
		{
			c++;
			j = 0;
		}
		for (int k = c; k < size; ++k)
		{
			if (k < j)
			{
				C[i] += A[k*(k + 1) / 2 + c] * B[j*(j + 1) / 2 + k];
			}
			else
			{
				C[i] += A[k*(k + 1) / 2 + c] * B[k*(k + 1) / 2 + j];
			}
		}
		++j;
	}

	double end_time = omp_get_wtime();

	std::cout << "Run time: " << end_time - start_time << std::endl;
	//Проверка на матрицах заполненных единицами
	std::cout << "Correct answer: " << correct_answer(C, size) << std::endl;

	//Печать
	/*
	std::cout << "C:" << std::endl;
	for (int i = 0; i < size*size; ++i)
	{
		if (i != 0 && i%size == 0)
			std::cout << std::endl;
		std::cout << C[i] << " ";
	}
	std::cout << std::endl;
	*/

	delete[] C;

}

void non_parallel_blocks(int size, double*A, double*B) {

	std::cout << "\tNon parallel blocks"<<std::endl;

	int size_array = size*size;
	double* C = new double[size_array];
	for (int i = 0; i < size_array; ++i)
		C[i] = 0;


		int size_block =512, amount_blocks = size / size_block;

		std::cout << "Size of block: " << size_block << std::endl
			<< "Amount of block: " << amount_blocks << std::endl;

		double start_time = omp_get_wtime();

		for (int ik = 0; ik < amount_blocks; ++ik)
		{
			for (int jk = 0; jk < amount_blocks; ++jk)
			{

				for (int kk = 0; kk < amount_blocks; ++kk)
				{
					if (kk == ik)
					{
						int c = ik*size_block,
							j = jk*size_block;

						double  *cc = C + ik*size_block*size + size_block*jk;

						for (int i = 0; i < size_block*size_block; ++i)
						{
							if (i != 0 && i%size_block == 0)
							{
								c++;
								j = jk*size_block;
								cc += size_block*(amount_blocks - 1);
							}
							for (int k = c; k < ik*size_block + size_block; ++k)
							{
								if (k < j)
									*cc += A[k*(k + 1) / 2 + c] * B[j*(j + 1) / 2 + k];
								else
									*cc += A[k*(k + 1) / 2 + c] * B[k*(k + 1) / 2 + j];
							}
							++j;
							++cc;
						}
					}

					if (kk > ik)
					{

						int c = ik*size_block,
							j = jk*size_block;

						double *cc = C + ik*size_block*size + size_block*jk;

						for (int i = 0; i < size_block*size_block; ++i)
						{
							if (i != 0 && i%size_block == 0)
							{
								c++;
								j = jk*size_block;
								cc += size_block*(amount_blocks - 1);
							}
							for (int k = kk*size_block; k < kk*size_block + size_block; ++k)
							{
								if (k < j)
									*cc += A[k*(k + 1) / 2 + c] * B[j*(j + 1) / 2 + k];
								else
									*cc += A[k*(k + 1) / 2 + c] * B[k*(k + 1) / 2 + j];
							}
							++j;
							++cc;
						}
					}
				}
			}
		}

		double end_time = omp_get_wtime();
		std::cout << "Run time: " << end_time - start_time << std::endl;

		//Проверка на матрицах заполненных единицами
		std::cout << "Correct answer: " << correct_answer(C,size) << std::endl;

		//Печать
		/*
		std::cout << "Matrix C:" << std::endl;
		for (int i = 0; i < size_array; ++i)
		{
			if (i != 0 && i%size == 0)
				std::cout << std::endl;
			std::cout << C[i] << " ";
		}
		std::cout << std::endl;
		*/

		delete[] C;

}

void check_matrix() {

	int size = 2;

	double A[] = { 1,4,3,9,9,2,3,23,6,22 };
	double B[] = { 1,23,3,65,7,5,8,22,34,4 };

	std::cout << "Matrix A:" << std::endl;
	for (int i = 0; i < size; ++i)
	{
		for (int j = 0; j < size; ++j)
		{
			if (j < i)
				std::cout << 0 << " ";
			else
				std::cout << A[j*(j + 1) / 2 + i] << " ";
		}
		std::cout << std::endl;
	}

	std::cout << "Matrix B:" << std::endl;
	for (int i = 0; i < size; ++i)
	{
		for (int j = 0; j < size; ++j)
		{
			if (j < i)
				std::cout << B[i*(i + 1) / 2 + j] << " ";
			else
				std::cout << B[j*(j + 1) / 2 + i] << " ";
		}
		std::cout << std::endl;
	}

	std::cout << "Multiply block: " << std::endl;

	non_parallel_blocks(size, A, B);

	std::cout << "Multiply: " << std::endl;

	non_parallel(size, A, B);

}

void parallel_one(int size, double*A, double*B)
{
	std::cout << "\tParallel. Point one" << std::endl;

	int size_array = size*size;
	double* C = new double[size_array];
	for (int i = 0; i < size_array; ++i)
		C[i] = 0;
	int size_block =512;

	double start_time = omp_get_wtime();

#pragma omp parallel num_threads(8)
	{
		int amount_blocks = size / size_block;
#pragma omp	for 
			for (int ik = 0; ik < amount_blocks; ++ik)
			{
				for (int jk = 0; jk < amount_blocks; ++jk)
				{
					for (int kk = 0; kk < amount_blocks; ++kk)
					{
						if (kk == ik)
						{
							int c = ik*size_block,
								j = jk*size_block;

							double  *cc = C + ik*size_block*size + size_block*jk;

							for (int i = 0; i < size_block*size_block; ++i)
							{
								if (i != 0 && i%size_block == 0)
								{
									c++;
									j = jk*size_block;
									cc += size_block*(amount_blocks - 1);
								}
								for (int k = c; k < ik*size_block + size_block; ++k)
								{
									if (k < j)
										*cc += A[k*(k + 1) / 2 + c] * B[j*(j + 1) / 2 + k];
									else
										*cc += A[k*(k + 1) / 2 + c] * B[k*(k + 1) / 2 + j];
								}
								++j;
								++cc;
							}
						}

						if (kk > ik)
						{

							int c = ik*size_block,
								j = jk*size_block;

							double *cc = C + ik*size_block*size + size_block*jk;

							for (int i = 0; i < size_block*size_block; ++i)
							{
								if (i != 0 && i%size_block == 0)
								{
									c++;
									j = jk*size_block;
									cc += size_block*(amount_blocks - 1);
								}
								for (int k = kk*size_block; k < kk*size_block + size_block; ++k)
								{
									if (k < j)
										*cc += A[k*(k + 1) / 2 + c] * B[j*(j + 1) / 2 + k];
									else
										*cc += A[k*(k + 1) / 2 + c] * B[k*(k + 1) / 2 + j];
								}
								++j;
								++cc;
							}
						}
					}
				}
			}
	}
	double end_time = omp_get_wtime();


	std::cout << "Run time: " << end_time - start_time << std::endl;

	//Проверка на матрицах заполненных единицами
	std::cout << "Correct answer: " << correct_answer(C, size) << std::endl;

	//Печать
	/*
	std::cout << "C:" << std::endl;
	for (int i = 0; i < size_array; ++i)
	{
		if (i != 0 && i%size == 0)
			std::cout << std::endl;
		std::cout << C[i] << " ";
	}
	std::cout << std::endl;
	*/

	delete[] C;
}

void parallel_two(int size, double*A, double*B)
{
	std::cout << "\tParallel. Point two" << std::endl;

	int size_array = size*size;
	double* C = new double[size_array];
	for (int i = 0; i < size_array; ++i)
		C[i] = 0;
	int size_block=512;

	double start_time = omp_get_wtime();

#pragma omp parallel num_threads(8)
	{
		int amount_blocks = size / size_block;
		for (int ik = 0; ik < amount_blocks; ++ik)
		{
#pragma omp	for schedule(dynamic,1)
			for (int jk = 0; jk < amount_blocks; ++jk)
			{
				for (int kk = 0; kk < amount_blocks; ++kk)
				{
					if (kk == ik)
					{
						int c = ik*size_block,
							j = jk*size_block;

						double *cc = C + ik*size_block*size + size_block*jk;

						for (int i = 0; i < size_block*size_block; ++i)
						{
							if (i != 0 && i%size_block == 0)
							{
								c++;
								j = jk*size_block;
								cc += size_block*(amount_blocks - 1);
							}
							for (int k = c; k < ik*size_block + size_block; ++k)
							{
								if (k < j)
									*cc += A[k*(k + 1) / 2 + c] * B[j*(j + 1) / 2 + k];
								else
									*cc += A[k*(k + 1) / 2 + c] * B[k*(k + 1) / 2 + j];
							}
							++j;
							++cc;
						}
					}

					if (kk > ik)
					{

						int c = ik*size_block,
							j = jk*size_block;

						double *cc = C + ik*size_block*size + size_block*jk;

						for (int i = 0; i < size_block*size_block; ++i)
						{
							if (i != 0 && i%size_block == 0)
							{
								c++;
								j = jk*size_block;
								cc += size_block*(amount_blocks - 1);
							}
							for (int k = kk*size_block; k < kk*size_block + size_block; ++k)
							{
								if (k < j)
									*cc += A[k*(k + 1) / 2 + c] * B[j*(j + 1) / 2 + k];
								else
									*cc += A[k*(k + 1) / 2 + c] * B[k*(k + 1) / 2 + j];
							}
							++j;
							++cc;
						}
					}
				}
			}
		}
	}
	double end_time = omp_get_wtime();


	std::cout << "Run time: " << end_time - start_time << std::endl;

	//Проверка на матрицах заполненных единицами
	std::cout << "Correct answer: " << correct_answer(C, size) << std::endl;
	
	/*
	std::cout << "Matrix C:" << std::endl;
	for (int i = 0; i < size_array; ++i)
	{
	if (i != 0 && i%size == 0)
	std::cout << std::endl;
	std::cout << C[i] << " ";
	}
	std::cout << std::endl;
	*/

	delete[] C;
}

int main()
{
	int size = 2048;

	double* A = geherate_matrix(size);
	double* B = geherate_matrix(size);

	//Для подсчета цикл j пойдет от i
	//k элемент одномерного массива соответствует i j элементу массива по формуле k=j*(j+1)/2+i, где i<=j

	//Печать матриц для проверки
	/*
	//A
	std::cout << "A:" <<std::endl;
	for (int i = 0; i <  size; ++i)
	{
		for (int j = 0; j < size; ++j)
		{
			if (j < i)
				std::cout << 0 << " ";
			else
				std::cout << A[j*(j+1)/2+i] << " ";
		}
		std::cout << std::endl;
	}
	//B
	std::cout << "B:" <<std::endl;
	for (int i = 0; i < size; ++i)
	{
		for (int j = 0; j < size; ++j)
		{
			if (j < i)
				std::cout << B[i*(i + 1) / 2 + j] << " ";
			else
				std::cout << B[j*(j + 1) / 2 + i] << " ";
		}
		std::cout << std::endl;
	}
	*/

	//Функция проверки на рандомных числах
	//check_matrix();

	//non_parallel(size, A, B);	

	non_parallel_blocks(size, A, B);

	parallel_one(size, A, B);

	parallel_two(size, A, B);

	delete[] A;
	delete[] B;


    return 0;
}

