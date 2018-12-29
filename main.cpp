#pragma once
#pragma warning(disable : 4996)

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <omp.h>

#define N 512

using namespace std;

int* Get_Block(int* A, int blockSZ)
{
	int* result = new int[blockSZ*blockSZ];

	for (int i = 0; i < blockSZ*blockSZ; ++i)
		result[i] = A[i];

	return result;
}

void matrix_free(int **A)
{
	for (size_t i(0); i < N; i++)
		delete[] A[i];

	delete[] A;
}

void create_matrix_A() {
	srand(time(0));
	FILE* f = fopen("A.txt", "w");
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			if (i >= j) {
				fprintf(f,"%4d",rand() % 10);
			}
			else {
				fprintf(f, "%4d", 0);
			}
		}
		fprintf(f, "\n");
	}
	fclose(f);
}
void create_matrix_B() {
	srand(time(0));

	int** B = new int*[N];
	for (size_t i(0); i < N; i++)
		B[i] = new int[N];
	FILE* f = fopen("B.txt", "w");
	for (int i = 0; i < N; ++i) {
		for (int j = i; j < N; ++j) {
			if (i == j) {
				B[i][j] = rand() % 10;
			}
			else {
				B[i][j] = rand() % 10;
				B[j][i] = B[i][j];
			}
		}
	}
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			fprintf(f, "%4d", B[i][j]);
		}
		fprintf(f, "\n");
	}
	fclose(f);
}

int* read_matrix_A(){
	int* a = new int[N*N];
	int** A = new int*[N];
	for (int i = 0; i < N; i++)
		A[i] = new int[N];

	FILE *f;
	f = fopen("A.txt", "r");

	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			if (!feof(f)) {
				fscanf(f, "%d", &A[i][j]);
				a[j*N + i] = A[i][j];
			}
		}
	}
	fclose(f);
	return a;
}
int* read_matrix_B() {
	int* b = new int[N*N];
	int** B = new int*[N];
	for (int i = 0; i < N; i++)
		B[i] = new int[N];

	FILE *f;
	f = fopen("B.txt", "r");

	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			if (!feof(f)) {
				fscanf(f, "%d", &B[i][j]);
				b[i*N+j] = B[i][j];
			}
		}
	}
	fclose(f);
	return b;
}

int* multi(int* A, int* B, int blockSZ)
{
	int* C = new int[N*N];
	for (int i = 0; i < N*N; ++i)
		C[i] = 0;

	double t1, t2;
	t1 = omp_get_wtime();
	int S = N / blockSZ;

	for (int i = 0; i < S; ++i)
		for (int j = 0; j < S; ++j)
			for (int k = j; k < S; ++k)
			{
				int *a = A + (i * (S + 1) - (i + 1)*i / 2 + (k - i)) * blockSZ*blockSZ,
					*b = B + (j * (S + 1) - (j + 1)*j / 2 + (k - j)) * blockSZ*blockSZ,
					*c = C + i * N*blockSZ + j * blockSZ*blockSZ;

				for (int ii = 0; ii < blockSZ; ++ii)
					for (int jj = 0; jj < blockSZ; ++jj)
						for (int kk = 0; kk < blockSZ; ++kk)
							c[ii*blockSZ + jj] += a[ii*blockSZ + kk] * b[kk* blockSZ + jj];
			}

	t2 = omp_get_wtime();

	FILE* file = fopen("time1.txt", "a");
	fprintf(file, "%f\n", (t2 - t1));
	fclose(file);

	return C;
}

int* multi_parallel(int* A, int* B, int blockSZ)
{
	int* C = new int[N*N];
	for (int i = 0; i < N*N; ++i)
		C[i] = 0;

	double t1, t2;
	t1 = omp_get_wtime();

#pragma omp parallel num_threads(4) 
	{
		int S = N / blockSZ;
#pragma omp for  schedule(static)
		for (int i = 0; i < S; ++i)
			for (int j = 0; j < S; ++j)
				for (int k = j; k < S; ++k)
				{
					int *a = A + (i * (S + 1) - (i + 1)*i / 2 + (k - i)) * blockSZ*blockSZ,
						*b = B + (j * (S + 1) - (j + 1)*j / 2 + (k - j)) * blockSZ*blockSZ,
						*c = C + i * N*blockSZ + j * blockSZ*blockSZ;

					for (int ii = 0; ii < blockSZ; ++ii)
						for (int jj = 0; jj < blockSZ; ++jj)
							for (int kk = 0; kk < blockSZ; ++kk)
								c[ii*blockSZ + jj] += a[ii*blockSZ + kk] * b[kk* blockSZ + jj];
				}
	}
	t2 = omp_get_wtime();;

	FILE* file = fopen("time2.txt", "a");
	fprintf(file, "%f\n", (t2 - t1));
	fclose(file);

	return C;
}

int* multi_parallel_divide(int* A, int* B, int blockSZ)
{
	int* C = new int[N*N];
	for (int i = 0; i < N*N; ++i)
		C[i] = 0;

	double t1, t2;
	t1 = omp_get_wtime();

#pragma omp parallel num_threads(4)
	{
		size_t S = N / blockSZ;
#pragma omp for  schedule(static)
		for (int i = 0; i < S; ++i)
			for (int j = 0; j < S; ++j)
				for (int k = j; k < S; ++k)
				{
					int *a = Get_Block(A + (i * (S + 1) - (i + 1)*i / 2 + (k - i)) * blockSZ, blockSZ),
						*b = Get_Block(B + (j * (S + 1) - (j + 1)*j / 2 + (k - j)) * blockSZ*blockSZ,blockSZ),
						*c = C + i * N*blockSZ + j * blockSZ*blockSZ;

					for (int ii = 0; ii < blockSZ; ++ii)
						for (int jj = 0; jj < blockSZ; ++jj)
							for (int kk = 0; kk < blockSZ; ++kk)
								c[ii*blockSZ + jj] += a[ii*blockSZ + kk] * b[kk* blockSZ + jj];
					delete[] a;
					delete[] b;
				}
	}
	t2 = omp_get_wtime();;

	FILE* file = fopen("time3.txt", "a");
	fprintf(file, "%f\n", (t2 - t1));
	fclose(file);

	return C;
}

int main() {
	setlocale(LC_ALL, "Russian");
	create_matrix_A();
	create_matrix_B();
	int* a = read_matrix_A();
	int* b = read_matrix_B();
	int *c1, *c2, *c3;
	for (int i = 1; i <= N; i *= 2) {
		cout << "Размер блока : " << i << endl;
		multi(a,b,i);
		multi_parallel(a, b, i);
		multi_parallel_divide(a, b, i);
	}
	return 0;
}