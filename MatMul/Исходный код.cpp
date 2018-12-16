#pragma once
#pragma warning(disable : 4996)

#include <iostream>
#include <fstream>

#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include "omp.h"

const int N = 480;
int* get_block(int* A, int size)
{
	int* vector = new int[size*size];

	for (int i = 0; i < size*size; ++i)
		vector[i] = A[i];

	return vector;
}
void delete_matrix(int **A)
{
	for (int i=0; i < N; i++)
		delete[] A[i];

	delete[] A;
}

void matrix_B_in_file(const char* fname)
{
	int** B = new int*[N];
	for (int i=0; i < N; i++)
		B[i] = new int[N];

	for (int i=0; i < N; ++i)
	for (int j=0; j < N; ++j)
	if (i <= j) B[i][j] = rand() % 10;
	else B[i][j] = 0;

	FILE* file = fopen(fname, "w");

	for (int i=0; i < N; ++i)
	{
		for (int j=0; j < N; ++j)
			fprintf(file, "%4d", B[i][j]);
		fprintf(file, "\n");
	}
	fclose(file);
	delete_matrix(B);
}


void matrix_A_in_file(const char* fname)
{
	int** A = new int*[N];
	for (size_t i(0); i < N; i++)
		A[i] = new int[N];

	for (int i=0; i < N; ++i)
	for (int j=0; j < N; ++j)
	{
		if (i >= j) A[i][j] = rand() % 10;
		else A[i][j] = 0;
	}

	FILE* file = fopen(fname, "w");

	for (int i=0; i < N; ++i)
	{
		for (int j=0; j < N; ++j)
			fprintf(file, "%4d", A[i][j]);
		fprintf(file, "\n");
	}
	fclose(file);
	delete_matrix(A);

}


int* matrix_A_from_file(const size_t size, int** A)
{
	int num = N / size;
	int* vector = new int[(num + 1)*num / 2 * size*size];
	int k = 0;

	for (int l = 0; l< num; ++l)
	for (int m = 0; m< l+1; ++m)
	for (int i = 0; i < size; ++i)
	for (int j = 0; j < size; ++j)
		vector[k++] = A[i + l * size][j + m * size];

	/*printf("A: ");
	for (int j=0; j <(num + 1)*num / 2 * size*size; ++j)
	printf("%4d", vector[j]);
	printf("\n");*/

	return vector;
}

int* matrix_B_from_file(const int size, int** A)
{
	int num=N / size;
	int* vector = new int[(num + 1)*num / 2 * size*size];
	int k = 0;

	for (int l=0; l< num; ++l)
	for (int m=l; m< num; ++m)
	for (int i=0; i < size; ++i)
	for (int j=0; j < size; ++j)
		vector[k++] = A[i + l * size][j + m * size];

	/*printf("B: ");
	for (int j = 0; j < (num + 1)*num / 2 * size*size; ++j)
	printf("%4d", vector[j]);
	printf("\n");*/


	return vector    ;
}

int** read_file(const char* fname)
{
	int** A = new int*[N];
	for (int i=0; i < N; i++)
		A[i] = new int[N];

	FILE *file;
	file = fopen(fname, "r");

	for (int i = 0; i<N; ++i)
	for (int j = 0; j<N; ++j)
	if (!feof(file)) fscanf(file, "%d", &A[i][j]);
	fclose(file);
	return A;
}
void matrix_C(const char* fname1, const char* fname2)
{
	int** A = read_file(fname1);
	int** B = read_file(fname2);
	int **C = new int*[N];
	for (int i = 0; i < N; i++)
	{
		C[i] = new int[N];
		for (int j = 0; j < N; ++j)
			C[i][j] = 0;
	}
	double time_begin, time_end;
	time_begin = omp_get_wtime();
	for (int i = 0; i<N; ++i)
	for (int j = 0; j < N; ++j)
	for (int k = 0; k < N; ++k)
		C[i][j] += A[i][k] * B[k][j];
	time_end = omp_get_wtime();

	FILE* file = fopen("time_not_parallel.txt", "a");
	fprintf(file, "%f\n", (time_end - time_begin));
	fclose(file);

	
}
int* multiplication(int* A, int* B, int size)
{
	int* C = new int[N*N];
	for (int i=0; i < N*N; ++i)
		C[i] = 0;

	double time_begin, time_end;
	time_begin = omp_get_wtime();
	int num = N / size;

	for (int i=0; i<num; ++i)
	for (int j = 0; j < num; ++j)
	{
		int k = 0;
		for (; k < i && k<=j; ++k)
		{
			int *a = A + (i *(i+1)/2+k) * size*size,
				*b = B + (j + k*(num - k) +k*(k-1)/2) * size*size,
				*c = C + i * N*size + j * size*size;

			for (int ii = 0; ii < size; ++ii)
			for (int jj = 0; jj < size; ++jj)
			for (int kk = 0; kk < size; ++kk)
				c[ii*size + jj] += a[ii*size + kk] * b[kk* size + jj];
		}
		if (k <= j)
		{


			int *a1 = A + (k *(k + 1) / 2 + i) * size*size,
			*b1 = B + (j + k*(num - k) + k*(k - 1) / 2) * size*size,
			*c1 = C + i * N*size + j * size*size;

			for (int ii1 = 0; ii1 < size; ++ii1)
			for (int jj1 = 0; jj1 < size; ++jj1)
			for (int kk1 = 0; kk1 < size; ++kk1)
			if (ii1 >= kk1)
				c1[ii1*size + jj1] += a1[ii1*size + kk1] * b1[kk1* size + jj1];
			else
				c1[ii1*size + jj1] += a1[kk1*size + ii1] * b1[kk1* size + jj1];
		}
		k++; 
		for (; k <= j; k++)
		{
			int *a2 = A + (k *(k + 1) / 2 + i) * size*size,
				*b2 = B + (j + k*(num - k) + k*(k - 1) / 2)* size*size,
				*c2 = C + i * N*size + j * size*size;

			for (int ii2 = 0; ii2 < size; ++ii2)
			for (int jj2 = 0; jj2 < size; ++jj2)
			for (int kk2 = 0; kk2 < size; ++kk2)
				c2[ii2*size + jj2] += a2[kk2*size + ii2] * b2[kk2* size + jj2];
		}
	}
	

	time_end = omp_get_wtime();

	/*printf("\nanswer: ");
	for(size_t j = 0; j < N*N; ++j)
	printf("%4d", C[j]);
	printf("\n");
*/
	FILE* file = fopen("time.txt", "a");
	fprintf(file, "%f\n", (time_end - time_begin));
	fclose(file);

	return C;
}
int* multiplication_parallel(int* A, int* B, int size)
{
	int* C = new int[N*N];
	for (int i = 0; i < N*N; ++i)
		C[i] = 0;

	double time_begin, time_end;
	time_begin = omp_get_wtime();
#pragma omp parallel num_threads(8) 
	{
		int num = N / size;
#pragma omp for  schedule(static)
		for (int i = 0; i < num; ++i)
		for (int j = 0; j < num; ++j)
		{
			int k = 0;
			for (; k < i && k <= j; ++k)
			{
				int *a = A + (i *(i+1)/2+k) * size*size,
					*b = B + (j + k*(num - k) +k*(k-1)/2) * size*size,
					*c = C + i * N*size + j * size*size;

				for (int ii = 0; ii < size; ++ii)
				for (int jj = 0; jj < size; ++jj)
				for (int kk = 0; kk < size; ++kk)
					c[ii*size + jj] += a[ii*size + kk] * b[kk* size + jj];
			}
			if (k <= j){
				int *a1 = A + (k *(k + 1) / 2 + i) * size*size,
					*b1 = B + (j + k*(num - k) + k*(k - 1) / 2) * size*size,
					*c1 = C + i * N*size + j * size*size;

				for (int ii = 0; ii < size; ++ii)
				for (int jj = 0; jj < size; ++jj)
				for (int kk = 0; kk < size; ++kk)
				if (ii >= kk)
					c1[ii*size + jj] += a1[ii*size + kk] * b1[kk* size + jj];
				else
					c1[ii*size + jj] += a1[kk*size + ii] * b1[kk* size + jj];
			}
			k++;
			for (; k <=j; ++k)
			{
				int *a = A + (k *(k + 1) / 2 + i) * size*size,
					*b = B + (j + k*(num - k) +k*(k-1)/2) * size*size,
					*c = C + i * N*size + j * size*size;

				for (int ii = 0; ii < size; ++ii)
				for (int jj = 0; jj < size; ++jj)
				for (int kk = 0; kk < size; ++kk)
					c[ii*size + jj] += a[kk*size + ii] * b[kk* size + jj];
			}
		}
		
	}
	time_end = omp_get_wtime();

	/*printf("\nanswer2: ");
	for(size_t j = 0; j < N*N; ++j)
	printf("%4d", C[j]);
	printf("\n");
*/
	FILE* file = fopen("time_parallel.txt", "a");
	fprintf(file, "%f\n", (time_end - time_begin));
	fclose(file);

	return C;
}


int* multiplication_parallel_block(int* A, int* B, int size)
{
	int* C = new int[N*N];
	for (int i = 0; i < N*N; ++i)
		C[i] = 0;

	double time_begin, time_end;
	time_begin = omp_get_wtime();
#pragma omp parallel num_threads(6) 
	{
		int num = N / size;
#pragma omp for  schedule(static)
		for (int i = 0; i < num; ++i)
		for (int j = 0; j < num; ++j)
		{
			int k = 0;
			for (; k < i && k <= j; ++k)
			{
				int *a = get_block(A + (i *(i+1)/2+k) * size*size,size),
					*b = get_block(B + (j + k*(num - k) + k*(k - 1) / 2) * size*size,size),
					*c = C + i * N*size + j * size*size;

				for (int ii = 0; ii < size; ++ii)
				for (int jj = 0; jj < size; ++jj)
				for (int kk = 0; kk < size; ++kk)
					c[ii*size + jj] += a[ii*size + kk] * b[kk* size + jj];
				delete[] a;
				delete[] b;
			}
			int *a1 = get_block(A + (k *(k + 1) / 2 + i) * size*size,size),
				*b1 = get_block(B + (j + k*(num - k) + k*(k - 1) / 2) * size*size,size),
				*c1 = C + i * N*size + j * size*size;

			for (int ii = 0; ii < size; ++ii)
			for (int jj = 0; jj < size; ++jj)
			for (int kk = 0; kk < size; ++kk)
			if (ii >= kk)
				c1[ii*size + jj] += a1[ii*size + kk] * b1[kk* size + jj];
			else
				c1[ii*size + jj] += a1[kk*size + ii] * b1[kk* size + jj];
			delete[] a1;
			delete[] b1;
			k++;
			for (; k <=j; ++k)
			{
				int *a = get_block(A + (k *(k + 1) / 2 + i) * size*size,size),
					*b = get_block(B + (j + k*(num - k) + k*(k - 1) / 2) * size*size, size),
					*c = C + i * N*size + j * size*size;

				for (int ii = 0; ii < size; ++ii)
				for (int jj = 0; jj < size; ++jj)
				for (int kk = 0; kk < size; ++kk)
					c[ii*size + jj] += a[kk*size + ii] * b[kk* size + jj];
				delete[] a;
				delete[] b;
			}
		}

	}
	time_end = omp_get_wtime();

	/*printf("\nanswer3: ");
	for(size_t j = 0; j < N*N; ++j)
	printf("%4d", C[j]);
	printf("\n");*/

	FILE* file = fopen("time_parallel_block.txt", "a");
	fprintf(file, "%f\n", (time_end - time_begin));
	fclose(file);

	return C;
}


int main(int argc, char**argv)
{
	matrix_A_in_file("A.txt");
	matrix_B_in_file("B.txt");
	int** first = read_file("A.txt");
	int** second = read_file("B.txt" );
	matrix_C("A.txt", "B.txt");
	for (int size=1; size <= N; ++size)
	{
		if (N%size == 0)
		{
			FILE* dfile = fopen("block.txt", "a");
			fprintf(dfile, "%d\n", size);
			printf("\nblock size: %d\n", size);

			int* A = matrix_A_from_file(size, first);
			int* B = matrix_B_from_file(size, second);
			int* C1 = multiplication(A, B, size);
			delete[] C1;
			int* C2 = multiplication_parallel(A, B, size);
			delete[] C2;
			int* C3 = multiplication_parallel_block(A, B, size );
			delete[] C3;


			delete[] A;
			delete[] B;

			fclose(dfile);
		}
	}
	delete_matrix(first);
	delete_matrix(second);
	printf("\n");
	system("pause");
	return 0;
}

