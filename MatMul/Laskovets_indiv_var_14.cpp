// 14.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//




/*int **sum_matrix(int **A, int **B, int k)
{
	int **D = new int*[k];
	for (int i = 0; i < k; i++)
	{
		D[i] = new int[k];
		for (int j = 0; j < k; j++)
		{
			D[i][j] = A[i][j] + B[i][j];
		}
	}

	return D;
}*/


/*int ** multiplate_A_B_block(int **A, int **B, int k)
{
	int **D = new int*[k];
	for (int i = 0; i < k; i++)
	{
		D[i] = new int[k];
		for (int j = 0; j < k; j++)
		{
			D[i][j] = 0;
		}
	}

	for (int row = 0; row < k; row++) {
		for (int col = 0; col < k; col++) {
			// Multiply the row of A by the column of B to get the row, column of product.
			for (int inner = 0; inner < k; inner++) {
				D[row][col] += A[row][inner] * B[inner][col];
			}
		}
	}
	return D;

}

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
			D[i][j] = A[position1 + x][position2 + y];
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
			D[i][j] = B[position2 + y][position1 + x];
		}
	}
	return D;
}


int **calculate_sub_D(int **A, int **B, int i, int j, int k, int **sub_D)
{
	int **sub_A = get_A_block(A, i, j, k);
	int **sub_B = get_B_block(B, i, j, k);
	int **res = multiplate_A_B_block(sub_A, sub_B, k);
	return sum_matrix(sub_D, res, k);
}*/


/*int ** claculate_A_B(int **A, int **B, int k, int n)
{
	int **D = new int*[n];
	for (int i = 0; i < n; i++)
	{
		D[i] = new int[n];
		for (int j = 0; j < n; j++)
		{
			D[i][j] = 0;
		}
	}

	for (int i = 0; i < round(n/k); i++)
	{
		int **sub_D = new int*[k];
		for (int j = 0; j < k; j++)
		{
			sub_D[i] = new int[k];
			for (int j = 0; j < k; j++)
			{
				sub_D[i][j] = 0;
			}
		}
		#pragma omp parallel for num_threads(8)
		for (int j = 0; j < round(n / k); j++)
		{
			sub_D = calculate_sub_D(A, B, i, j, k, sub_D);
		}


	}

}*/


/*int ** claculate_A_B(int **A, int **B, int k, int n)
{
	int **D = new int*[n];
	for (int i = 0; i < n; i++)
	{
		D[i] = new int[n];
		for (int j = 0; j < n; j++)
		{
			D[i][j] = 0;
		}
	}
}*/


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
	for (int i = 1; i < n + 1; i++) {
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
			for (int block_row = 0; block_row < int(n / k); block_row++)
			{
				int i, j, block_column, br;
#pragma omp parallel num_threads(8) //private(block_column, i, j)
				{
#pragma omp for  schedule(static)
					for (int block_column = 0; block_column < int(n / k); block_column++)
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
			cout << "   " << end_time - start_time << "\n";
			cout << "***************************" << "\n";
		}
	}




	return 0;

}


/*#include "pch.h"
#include <iostream>

#include <stdio.h>
#include <omp.h>
using namespace std;
int main(int argc, char *argv[])
{
	const int n = 72;
	int m = 0;
	for (int i = 0; i <= n; ++i)
		m += i;
	cout << m<<endl;
	int *a = new int[m];
	for (int i = 0; i <= m; ++i)
		a[i] = 1;
	int c[n][n];
	int c1[n][n];
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j)
		{
			c1[i][j] = 0;
			c[i][j] = 0;
		}
	int i = 0, k, K, J, j;

	double start_time, end_time;
	cout << "parallel" << endl;
	start_time = omp_get_wtime();

#pragma omp parallel private(i, J, K, k, j)
	{
#pragma omp for schedule(dynamic, 8)

		for (i = 0; i < n; ++i) {
			J = i;
			for (j = 0; j <= i; ++j) {
				K = j;


				for (k = 0; k < n; ++k) {

					c[i][k] += a[J] * a[K];
					if (j == 0)
						K = k + 1;
					if (j >= 1 && j < n - 1) {
						if (k <= j - 1)
							K += (n - k - 1);
						else
							K += 1;

					}
					if (j == n - 1) {
						K += (n - k - 1);
					}



				}

				J += n - j - 1;
			}


		}


	}




	end_time = omp_get_wtime();

	cout << end_time - start_time << endl;
	start_time = omp_get_wtime();
	for (i = 0; i < n; ++i) {
		J = i;
		for (j = 0; j <= i; ++j) {
			K = j;


			for (int k = 0; k < n; ++k) {

				c1[i][k] += a[J] * a[K];
				if (j == 0)
					K = k + 1;
				if (j >= 1 && j < n - 1) {
					if (k <= j - 1)
						K += (n - k - 1);
					else
						K += 1;

				}
				if (j == n - 1) {
					K += (n - k - 1);
				}



			}

			J += n - j - 1;
		}


	}
	end_time = omp_get_wtime();
	cout << end_time - start_time << endl;
	int aa[n][n];
	int bb[n][n];
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j) {
			bb[i][j] = 1;
			aa[i][j] = 0;
		}
	for (int i = 0; i < n; ++i)
		for (int j = 0; j <= i; ++j) {
			aa[i][j] = 1;
		}
	int cc[n][n];
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			cc[i][j] = 0;
			for (k = 0; k < n; k++)
				cc[i][j] += aa[i][k] * bb[k][j];

		}

	}
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
			if (c[i][j] != cc[i][j])
				cout<< "Error";
	}


	system("pause");
	return 0;
}*/