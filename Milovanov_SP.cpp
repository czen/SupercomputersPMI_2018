// Milovanov_SP.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"

#include <iostream>
#include <omp.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include "omp.h"

using namespace std;


const int N = 440; //Размер матрицы 



void get_symmetric_from_NTmatrix(int* &v, int N) //Делаем из нижнетреугольной матрицы симметричную
{
	for (int i = 0; i < N; i++)
		for (int j = i + 1; j < N; j++)
			v[i*N + j] = v[j*N + i];
}

void print_array(const int arr[], int N) //Вывод массива на экран
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++) 
		{
			cout.width(5);
			cout << arr[i*N + j];
		}
		cout << endl;
	}
}

void print_vector(const int v[], int N)
{
	for (int i = 0; i < N; i++)
	{
		cout.width(5);
		cout << v[i];
	}
	cout << endl;
}

//Из считанной строки А делаем строку по блочным столбцам
void get_block_vector(int blockSize, int blockVector[], const int stringVector[], int N)
{
	int p = 0;
	for (int j = 0; j < N / blockSize; j++)
		for (int i = j * blockSize; i < N; i++)
			for (int k = 0; k < blockSize; k++)
			{
				blockVector[p] = stringVector[i * N + j * blockSize + k];
				p++;
			}
}

//Функция перемножения двух блоков (А и В)
void mult_2_block(int resultBlock[], const int A[], const int B[], int blockSize)
{
	int res;
	for (int i = 0; i < blockSize; i++)
	{
		for (int j = 0; j < blockSize; j++)
		{
			res = 0;
			for (int k = 0; k < blockSize; k++)
			{
				res += A[i*blockSize + k] * B[k*blockSize + j];
			}
			resultBlock[i*blockSize + j] = res;
		}
	}
}

//Функция умножения двух блоков (А транспонированного и В)
//Используется , когда нужен блок, симметричный блоку А
void mult_2_block_transpose(int resultBlock[], const int A[], const int B[], int blockSize)
{
	int res;
	for (int i = 0; i < blockSize; i++)
	{
		for (int j = 0; j < blockSize; j++)
		{
			res = 0;
			for (int k = 0; k < blockSize; k++)
			{
				res += A[k*blockSize + i] * B[k*blockSize + j];
			}
			resultBlock[i*blockSize + j] = res;
		}
	}
}


//Функция вывода на экран результатирующей матрицы (хранится в блочном виде, по строкам)
void print_block_matrix(const int array[], int blockSize, int N) //Вывод массива на экран
{
	for (int i = 0; i < N / blockSize; i++)
	{
		for (int i1 = 0; i1 < blockSize; i1++)
		{
			for (int j = 0; j < N / blockSize; ++j)

				for (int j1 = 0; j1 < blockSize; j1++)
				{
					cout.width(5);
					cout << array[(i * blockSize * N) + (j * blockSize * blockSize) + (i1 * blockSize) + j1];
				}
			cout << endl;
		}
	}
}

//new_block выделяет память для конкретных блоков и заполняет их необходимыми элементами
int* new_block(const int* v, int blockSize)
{
	int* res = new int[blockSize*blockSize];
	for (int i = 0; i < blockSize*blockSize; i++)
		res[i] = v[i];

	return res;
}

//Создаём файл с нижне-треугольной матрицей
void get_NTmatrix_FILE(int N, FILE *filename)
{
	int tmp;
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (j > i)
				fprintf(filename, "%5d", 0);

			else
				fprintf(filename, "%5d", 1 + rand() % 10);
		}
		fprintf(filename, "\n");
	}
}

//Создаём вектор из файла 
void get_vector_from_FILE(int* &v, const int N, FILE *filename)
{
	while (!feof(filename))
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++)
				fscanf(filename, "%5d", &v[i*N + j]);


}



//Блочное последовательное умножение двух матриц
int* Mult_Block_Posled(const int *A_String, const int *B_String, const int blockSize)
{
	int NB = N / blockSize; 
	int vecSize = ((NB*NB - NB) / 2 + NB)*blockSize*blockSize;
	int *A_Matrix = new int[vecSize];
	get_block_vector(blockSize, A_Matrix, A_String, N);
	int *B_Matrix = new int[vecSize];
	get_block_vector(blockSize, B_Matrix, B_String, N);
	int *result = new int[N*N];

	int *A_Matrix_Block = new int[blockSize*blockSize];
	int *B_Matrix_Block = new int[blockSize*blockSize];
	int *Tmp_Res_Matrix_Block = new int[blockSize*blockSize];

	double startTime, endTime;
	startTime = omp_get_wtime();

	for (int i = 0; i < NB; ++i)
	{
		for (int j = 0; j < NB; ++j)
		{
			int *Res_Matrix_Block = result + i * NB * blockSize*blockSize + j * blockSize*blockSize;
			for (int i1 = 0; i1 < blockSize*blockSize; i1++)
			{
				Res_Matrix_Block[i1] = 0;
			}

			for (int k = 0; k <= j; ++k)
			{
				int A_Offset = 0, B_Offset = 0;
				if (i >= k)
					A_Offset = (i + (2 * NB*i - i * i - i) / 2 + (k - i))*blockSize*blockSize;
				else
					(i + (2 * NB*k - k * k - k) / 2)*blockSize*blockSize;
				B_Offset = (j + (2 * NB*j - j * j - 3 * j) / 2 + k)*blockSize*blockSize;

				for (int ib = 0; ib < blockSize*blockSize; ib++)
				{
					A_Matrix_Block[ib] = A_Matrix[A_Offset + ib];
					B_Matrix_Block[ib] = B_Matrix[B_Offset + ib];
				}

				if (k < i)
					mult_2_block(Tmp_Res_Matrix_Block, A_Matrix_Block, B_Matrix_Block, blockSize);
				else
					mult_2_block_transpose(Tmp_Res_Matrix_Block, A_Matrix_Block, B_Matrix_Block, blockSize);

				for (int i1 = 0; i1 < blockSize*blockSize; i1++)
				{
					Res_Matrix_Block[i1] += Tmp_Res_Matrix_Block[i1];
				}
			}


		}
	}
	delete[] A_Matrix_Block;
	delete[] B_Matrix_Block;
	delete[] Tmp_Res_Matrix_Block;

	endTime = omp_get_wtime();;

	FILE* file = fopen("Time_Posled.txt", "a");
	fprintf(file, "%f\n", (endTime - startTime));
	fclose(file);

	return result;
}

//Бочное параллельное внутри блока умножение матриц
int* Mult_Block_Parallel(const int *A_String, const int *B_String, const int blockSize)
{
	int NB = N / blockSize;
	int vecSize = ((NB*NB - NB) / 2 + NB)*blockSize*blockSize;
	int *A_Matrix = new int[vecSize];
	get_block_vector(blockSize, A_Matrix, A_String, N);
	int *B_Matrix = new int[vecSize];
	get_block_vector(blockSize, B_Matrix, B_String, N);
	int *result = new int[N*N];

	double startTime, endTime;
	startTime = omp_get_wtime();

	for (int i = 0; i < NB; ++i)
	{
		for (int j = 0; j < NB; ++j)
		{
			int *Res_Matrix_Block = result + i * NB * blockSize*blockSize + j * blockSize*blockSize;
			for (int i1 = 0; i1 < blockSize*blockSize; i1++)
			{
				Res_Matrix_Block[i1] = 0;
			}
#pragma omp parallel num_threads(8) 
			{
#pragma omp for  schedule(static)
				for (int k = 0; k <= j; ++k)
				{
					int A_Offset = 0, B_Offset = 0;
					if (i >= k)
						A_Offset = (i + (2 * NB*i - i * i - i) / 2 + (k - i))*blockSize*blockSize;
					else
						(i + (2 * NB*k - k * k - k) / 2)*blockSize*blockSize;
					B_Offset = (j + (2 * NB*j - j * j - 3 * j) / 2 + k)*blockSize*blockSize;
					
					int *A_Matrix_Block = A_Matrix + A_Offset;
					int *B_Matrix_Block = B_Matrix + B_Offset;
					int *Tmp_Res_Matrix_Block = new int[blockSize*blockSize];
					

					if (k < i)
						mult_2_block(Tmp_Res_Matrix_Block, A_Matrix_Block, B_Matrix_Block, blockSize);
					else
						mult_2_block_transpose(Tmp_Res_Matrix_Block, A_Matrix_Block, B_Matrix_Block, blockSize);

					for (int i1 = 0; i1 < blockSize*blockSize; i1++)
					{
						Res_Matrix_Block[i1] += Tmp_Res_Matrix_Block[i1];
					}
					delete[] Tmp_Res_Matrix_Block;
				}
			}

			
		}
	}

	endTime = omp_get_wtime();;

	FILE* file = fopen("Time_Parallel.txt", "a");
	fprintf(file, "%f\n", (endTime - startTime));
	fclose(file);

	return result;
}



//Блочное параллельное внутри блока умножение двух матриц 
int* Mult_Block_Parallel_New(const int *A_String, const int *B_String, const int blockSize)
{
	int NB = N / blockSize;
	int vecSize = ((NB*NB - NB) / 2 + NB)*blockSize*blockSize;
	int *A_Matrix = new int[vecSize];
	get_block_vector(blockSize, A_Matrix, A_String, N);
	int *B_Matrix = new int[vecSize];
	get_block_vector(blockSize, B_Matrix, B_String, N);
	int *result = new int[N*N];

	double startTime, endTime;
	startTime = omp_get_wtime();

	for (int i = 0; i < NB; ++i)
	{
		for (int j = 0; j < NB; ++j)
		{
			int *Res_Matrix_Block = new int[blockSize*blockSize];
			for (int i1 = 0; i1 < blockSize*blockSize; i1++)
			{
				Res_Matrix_Block[i1] = 0;
			}
#pragma omp parallel num_threads(8) 
			{
#pragma omp for  schedule(static)
				for (int k = 0; k <= j; ++k)
				{
					int A_Offset = 0, B_Offset = 0;
					if (i >= k)
						A_Offset = (i + (2 * NB*i - i * i - i) / 2 + (k - i))*blockSize*blockSize;
					else
						(i + (2 * NB*k - k * k - k) / 2)*blockSize*blockSize;
					B_Offset = (j + (2 * NB*j - j * j - 3 * j) / 2 + k)*blockSize*blockSize;

					int *A_Matrix_Block = new_block(A_Matrix + A_Offset, blockSize);
					int *B_Matrix_Block = new_block(B_Matrix + B_Offset, blockSize);
					int *Tmp_Res_Matrix_Block = new int[blockSize*blockSize];


					if (k < i)
						mult_2_block(Tmp_Res_Matrix_Block, A_Matrix_Block, B_Matrix_Block, blockSize);
					else
						mult_2_block_transpose(Tmp_Res_Matrix_Block, A_Matrix_Block, B_Matrix_Block, blockSize);

					for (int i1 = 0; i1 < blockSize*blockSize; i1++)
					{
						Res_Matrix_Block[i1] += Tmp_Res_Matrix_Block[i1];
					}

					delete[] A_Matrix_Block;
					delete[] B_Matrix_Block;
					delete[] Tmp_Res_Matrix_Block;
				}
			}


		}
	}

	endTime = omp_get_wtime();;

	FILE* file = fopen("Time_Parallel_New.txt", "a");
	fprintf(file, "%f\n", (endTime - startTime));
	fclose(file);

	return result;
}






//Блочное параллельное по строкам умножение двух матриц
int* Mult_Block_Parallel_String(const int *A_String, const int *B_String, const int blockSize)
{
	int NB = N / blockSize;
	int vecSize = ((NB*NB - NB) / 2 + NB)*blockSize*blockSize;
	int *A_Matrix = new int[vecSize];
	get_block_vector(blockSize, A_Matrix, A_String, N);
	int *B_Matrix = new int[vecSize];
	get_block_vector(blockSize, B_Matrix, B_String, N);
	int *result = new int[N*N];

	double startTime, endTime;
	startTime = omp_get_wtime();

#pragma omp parallel num_threads(8) 
	{
#pragma omp for schedule(static)
		for (int i = 0; i < NB; ++i)
		{
			for (int j = 0; j < NB; ++j)
			{
				int *Res_Matrix_Block = result + i * NB * blockSize*blockSize + j * blockSize*blockSize;
				for (int i1 = 0; i1 < blockSize*blockSize; i1++)
				{
					Res_Matrix_Block[i1] = 0;
				}

				for (int k = 0; k <= j; ++k)
				{
					int A_Offset = 0, B_Offset = 0;
					if (i >= k)
						A_Offset = (i + (2 * NB*i - i * i - i) / 2 + (k - i))*blockSize*blockSize;
					else
						(i + (2 * NB*k - k * k - k) / 2)*blockSize*blockSize;
					B_Offset = (j + (2 * NB*j - j * j - 3 * j) / 2 + k)*blockSize*blockSize;

					int *A_Matrix_Block = A_Matrix + A_Offset;
					int *B_Matrix_Block = B_Matrix + B_Offset;
					int *Tmp_Res_Matrix_Block = new int[blockSize*blockSize];


					if (k < i)
						mult_2_block(Tmp_Res_Matrix_Block, A_Matrix_Block, B_Matrix_Block, blockSize);
					else
						mult_2_block_transpose(Tmp_Res_Matrix_Block, A_Matrix_Block, B_Matrix_Block, blockSize);

					for (int i1 = 0; i1 < blockSize*blockSize; i1++)
					{
						Res_Matrix_Block[i1] += Tmp_Res_Matrix_Block[i1];
					}
					delete[] Tmp_Res_Matrix_Block;
				}
			}
		}

	}


	endTime = omp_get_wtime();

	FILE* file = fopen("Time_Parallel_String.txt", "a");
	fprintf(file, "%f\n", (endTime - startTime));
	fclose(file);

	return result;
}

int main()
{
	setlocale(LC_ALL, "rus");
	cout << "Creating files ..." << endl;
	FILE *matrixA = fopen("matrixA.txt", "w");
	get_NTmatrix_FILE(N, matrixA);
	fclose(matrixA);

	FILE *matrixB = fopen("matrixB.txt", "w");
	get_NTmatrix_FILE(N, matrixB);
	fclose(matrixB);

	matrixA = fopen("matrixA.txt", "r");
	int *A_Vector = new int[N*N];
	get_vector_from_FILE(A_Vector, N, matrixA);
	fclose(matrixA);
	get_symmetric_from_NTmatrix(A_Vector, N);


	matrixB = fopen("matrixB.txt", "r");
	int *B_Vector = new int[N*N];
	get_vector_from_FILE(B_Vector, N, matrixB);
	fclose(matrixB);

	int *Res_Non_Block = new int[N*N];

	//Ищем время для последовательного неблочного умножения
	cout << "Evaluating for non block matrix ..." << endl;
	double start_Time_Non_Block, end_Time_Non_Block;
	start_Time_Non_Block = omp_get_wtime();
	mult_2_block(Res_Non_Block, A_Vector, B_Vector, N);
	end_Time_Non_Block = omp_get_wtime();
	FILE* file = fopen("Time_Non_Block.txt", "a");
	fprintf(file, "%f\n", (end_Time_Non_Block - start_Time_Non_Block));
	fclose(file);

	delete[] Res_Non_Block;


	for (int blockSize = 1; blockSize <= N; blockSize++)
	{
		if ((N % blockSize) == 0)
		{

			cout << "Evaluating for block_size = " << blockSize << " ..." << endl;
			int *Res_Block_Posled = Mult_Block_Posled(A_Vector, B_Vector, blockSize);
			delete[] Res_Block_Posled;

			int *Res_Block_Parallel = Mult_Block_Parallel(A_Vector, B_Vector, blockSize);
			delete[] Res_Block_Parallel;

			int *Res_Block_Parallel_New = Mult_Block_Parallel_New(A_Vector, B_Vector, blockSize);
			delete[] Res_Block_Parallel_New;

			int *Res_Block_Parallel_String = Mult_Block_Parallel_String(A_Vector, B_Vector, blockSize);
			delete[] Res_Block_Parallel_String;

			FILE* file = fopen("Block_Size.txt", "a");
			fprintf(file, "%5d\n", (blockSize));
			fclose(file);
		}

	}

	delete[] A_Vector;
	delete[] B_Vector;


	system("pause");
	return 0;
}




