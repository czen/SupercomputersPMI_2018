#include "func.h"
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <omp.h>

using namespace std;

inline int A(int i, int j, int p)
{
	return p*p*(i + j*(j + 1) / 2);
}

inline int B(int i, int j, int p, int n)
{
	return p*p*(i*(n / p) + j);
}

void create_A_array(int *input, int *a, int p, int n)
{
	for (int j = 0; j < n / p; j++)
		for (int i = 0; i < p*(j + 1); i++)
			for (int k = 0; k < p; k++)
			{
				*a = input[i*n + p*j + k];
				a++;
			}
}

void create_B_array(int *input, int b[], int p, int n)
{
	for (int i = 0; i < n / p; i++)
		for (int j = 0; j < n / p; j++)
			for (int k = 0; k < p; k++)
				for (int z = 0; z < p; z++)
				{
					*b = input[(i*n + j)*p + k*n + z];
					b++;
				}
}


void read_from_text(const char *fname, int *&matr, int &n)
{
	FILE *fp;
	fopen_s(&fp, fname, "r");
	if (!fp)
		perror("no file");

	fscanf_s(fp, "%d\n", &n);
	matr = (int*)(calloc(n*n, sizeof(int)));
	int s = 0;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			fscanf_s(fp, "%d ", &matr[s]);
			s++;
		}
		fscanf_s(fp, "\n");
	}
	fclose(fp);
}


//функция перемножения двух матриц a и b размера n по блокам размера p
void block_mul(int * a, int * b, int * c, int n, int p)
{
	for (int i = 0; i < n / p; ++i)
		for (int j = 0; j < n / p; ++j)
			block_mul(a, b, c, i, j, n, p);
}

void block_mul_parallel(int * a, int * b, int * c, int n, int p)
{
	for (int i = 0; i < n / p; ++i)
		for (int j = 0; j < n / p; ++j)
			block_mul_parallel(a, b, c, i, j, n, p);
}

//функция перемножения двух блоков с индексом (i,j)
void block_mul(int * a, int * b, int * c, int i, int j, int n, int p)
{
	int* t = (int*)calloc(p*p, sizeof(int));
	for (int k = 0; k < n / p; ++k)
	{
		if (i > k) continue;
		mul(block_A(a, i, k, n, p), block_B(b, k, j, n, p), t, p);
		add(t, block_B(c, i, j, n, p), block_B(c, i, j, n, p), p);
	}
	free(t);
}

void block_mul_parallel(int * a, int * b, int * c, int i, int j, int n, int p)
{
#pragma omp parallel  num_threads(4) 
	{
		int* t = (int*)calloc(p*p, sizeof(int));
#pragma omp for schedule (dynamic) 
		for (int k = 0; k < n / p; ++k)
		{
			if (i > k) continue;
			mul(block_A(a, i, k, n, p), block_B(b, k, j, n, p), t, p);
			add(t, block_B(c, i, j, n, p), block_B(c, i, j, n, p), p);

		}
		free(t);
	}

}


void mul(int * ab, int * bb, int * cb, int p)
{
	int t;
#pragma omp parallel private(t), num_threads(4) 
	{
#pragma omp for schedule (dynamic) 
		for (int i = 0; i < p; i++)
			for (int j = 0; j < p; j++)
			{
				t = 0;
				for (int k = 0; k < p; k++)
				{
					t += ab[i*p + k] * bb[p*k + j];
				}
				cb[i*p + j] = t;
			}
	}
}

//void mul(int * ab, int * bb, int * cb, int p)
//{
//	for (int i = 0; i < p; i++)
//		for (int j = 0; j < p; j++)
//		{
//			int t = 0;  int ts;
//#pragma omp parallel private(ts), num_threads(4) 
//			{
//#pragma omp for schedule (dynamic) 
//				for (int k = 0; k < p; k++)
//				{
//					ts = ab[i*p + k] * bb[p*k + j];
//#pragma omp atomic
//					t += ts;
//					//std::cout << omp_get_thread_num() << ":" <<endl;
//					//std::cout << omp_get_thread_num() << ":" << i << "," << j << ":" << t << endl;	
//				}
//#pragma omp single 
//				{
//					cout << t;
//					cb[i*p + j] = t;
//				}
//
//			}
//		}
//}

//void mul(int * ab, int * bb, int * cb, int p)
//{
//	for (int i = 0; i < p; i++)
//		for (int j = 0; j < p; j++)
//		{
//			int t = 0;
//			for (int k = 0; k < p; k++)
//			{
//				t += ab[i*p + k] * bb[p*k + j];
//			}
//			cb[i*p + j] = t;
//		}
//}



void add(int * ab, int * bb, int * cb, int p)
{
#pragma omp parallel num_threads(4) 
	{
#pragma omp for schedule (dynamic) 
		for (int i = 0; i < p*p; i++)
		{
			cb[i] = ab[i] + bb[i];
		}
	}
}

inline int* block_A(int *a, int i, int j, int n, int p)
{
	return (a + A(i, j, p));
}


inline int* block_B(int *b, int i, int j, int n, int p)
{
	return (b + B(i, j, p, n));
}

void create_A_to_text(const char *fname, int n)
{
	FILE *fp;
	fopen_s(&fp, fname, "w");

	if (!fp)
		perror("no file");

	fprintf(fp, "%d\n", n);
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (i <= j) fprintf(fp, "%5d ", rand() % 20 - 10);
			else fprintf(fp, "%5d ", 0);
		}
		fprintf(fp, "\n");
	}
	fclose(fp);
}

void create_B_to_text(const char *fname, int n)
{
	FILE *fp;
	fopen_s(&fp, fname, "w");
	if (!fp)
		perror("no file");
	fprintf(fp, "%d\n", n);
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			fprintf(fp, "%5d ", rand() % 19 - 9);
		}
		fprintf(fp, "\n");
	}
	fclose(fp);
}

void out_matr(const char *fname, int n, int *matr, int p)
{
	FILE *fp;
	fopen_s(&fp, fname, "w");
	if (!fp)
		perror("no file");
	fprintf(fp, "%d\n", n);
	for (int i = 0; i < n / p; i++)
	{
		for (int z = 0; z < p; z++)
		{
			for (int j = 0; j < n / p; j++)
				for (int k = 0; k < p; k++)
					fprintf(fp, "%7d ", matr[z*p + j*p*p + k + i*p*p*(n / p)]);
			fprintf(fp, "\n");
		}
	}
	fclose(fp);
}
