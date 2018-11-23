#include "pch.h"
#include <iostream>
#include <time.h>
#include <omp.h>
#include <Windows.h>
using namespace std;


int ** get_A_block(int **A, int position1, int position2, int k)
{
	int **D = new int*[k];
	int x = position1 * k;
	int y = position2 * k;
	for (int i = 0; i < k; i++)
	{
		D[i] = new int[k];
		for (int j = 0; j < k; j++)
		{
			D[i][j] = A[i + x][j + y];
		}
	}
	return D;
}


int ** get_B_block(int **B, int position1, int position2, int k)
{
	int **D = new int*[k];
	int x = position1 * k;
	int y = position2 * k;
	for (int i = 0; i < k; i++)
	{
		D[i] = new int[k];
		for (int j = 0; j < k; j++)
		{
			D[i][j] = B[i + y][j + x];
		}
	}
	return D;
}


int ** create_rect_matrix(int **A, int n, int *sub_A, int count)
{
	int chet = n;
	int k = 0;
	int sum = 0;
	
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < i; j++)
		{
			A[i][j] = 0;
		}
		for (int j = i; j < n; j++)
		{
			A[i][j] = sub_A[k];
			k++;
		}
	}
	return A;
}



int main()
{
	const int n = 1024;
	int count = 0;
	for (int i = 1; i < n+1;  i++) {
		count += i;
	}
	int *sub_A = new int[count];
	int *sub_B = new int[count];
	for (int i = 0; i < count; i++) {
		sub_A[i] = rand() % 9;
		sub_B[i] = rand() % 9;
	}
	/*int A[n][n];
	int B[n][n];
	int D[n][n];*/
	int **A = new int *[n];
	int **B = new int *[n];
	int **D = new int *[n];
	
	for (int i = 0; i < n; i++)
	{
		A[i] = new int[n];
		B[i] = new int[n];
		D[i] = new int[n];
		for (int j = 0; j < n; j++)
		{
			//A[i][j] = rand() % 9;
			//B[i][j] = rand() % 9;
			A[i][j] = 0;
			B[i][j] = 0;
			D[i][j] = 0;
		}
	}
	A = create_rect_matrix(A, n, sub_A, count);
	B = create_rect_matrix(B, n, sub_B, count);
	double start_time, end_time;
	for (int k = 1; k < n + 1; k++)
	{
		if (n%k == 0)
		{

			cout << "block " << k << "\n";
			cout << "with parallel: ";
			start_time = omp_get_wtime();
			for (int block_row = 0; block_row < int(n/k); block_row++)
			{
				int i, j, block_column, br;
				#pragma omp parallel num_threads(8) //private(block_column, i, j)
				{
					#pragma omp for  schedule(static)
					for (int block_column = 0; block_column < int(n/k); block_column++)
					{
						for (int i = 0; i < k; i++) {
							for (int j = 0; j < k; j++)
							{
								//D[block_row*k + i][block_column*k + j] += A[block_row*k + i][block_column*k + j] * B[block_column*k + j][block_row*k + i];
								for (int f = 0; f < k; f++) {
									D[block_row*k + i][block_column*k + j] += A[block_row*k + i][block_column*k + f] * B[block_column*k + j][block_row*k + f];
								}
							}
						}
					}
				}
			}
			end_time = omp_get_wtime();
			cout << end_time - start_time << "\n";
			

			cout << "with parallel in single block: ";
			start_time = omp_get_wtime();
			for (int block_row = 0; block_row < int(n / k); block_row++)
			{
				int i, j, block_column, br;
#pragma omp parallel num_threads(8) //private(block_column, i, j)
				{
#pragma omp for  schedule(static)
					for (int block_column = 0; block_column < int(n / k); block_column++)
					{

						int ** sub_A = get_A_block(A, block_column, block_column, k);
						int ** sub_B = get_B_block(A, block_column, block_column, k);
						for (int i = 0; i < k; i++) {
							for (int j = 0; j < k; j++)
							{
								for (int f = 0; f < k; f++) {
									D[block_row*k + i][block_column*k + j] += sub_A[i][j] * sub_B[j][f];
								}
							}
						}
					}
				}
			}
			end_time = omp_get_wtime();
			cout << end_time - start_time << "\n";
			

			start_time = omp_get_wtime();
			cout << "no parallel: ";
			for (int block_row = 0; block_row < int(n / k); block_row++)
			{
				int i, j, block_column, br;
				for (int block_column = 0; block_column < int(n / k); block_column++)
				{
					for (int i = 0; i < k; i++) {
						for (int j = 0; j < k; j++)
						{
							//D[block_row*k + i][block_column*k + j] += A[block_row*k + i][block_column*k + j] * B[block_row*k + i][block_column*k + j];
							for (int f = 0; f < k; f++) {
								D[block_row*k + i][block_column*k + j] += A[block_row*k + i][block_column*k + f] * B[block_column*k + j][block_row*k + f];
							}
						}
					}
				}
			}
			end_time = omp_get_wtime();
			cout << "   " <<end_time - start_time << "\n";
			cout << "***************************" << "\n";
		}
	}




	return 0;

}
