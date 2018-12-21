#include "pch.h"
#include <iostream>
#include <omp.h>

//#define DEBUG
using namespace std;

int *A, *B, *C, *ofsA, *ofsB;

int getA(int i, int j)
{
	if (j > i)
		return 0;
	return A[i - j + ofsA[j]];
}

int getB(int i, int j)
{
	if (j < i)
		swap(i, j);
	//cout << "-"<<i<<"-" << i +  ofsB[j] << "-" << j << "-";
	return B[i + ofsB[j]];
}

void printA(int n)
{
#ifdef DEBUG
	if (n <= 8)
	{
		cout << "A: " << endl;
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
				cout << getA(i, j) << "\t";
			cout << endl;
		}
		cout << endl << endl;
	}
#endif
}

void printB(int n)
{
#ifdef DEBUG
	if (n <= 8)
	{
		cout << "B: " << endl;
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
				cout << getB(i, j) << "\t";
			cout << endl;
		}
		cout << endl << endl;
	}
#endif
}

void printC(int n)
{
#ifdef DEBUG
	if (n <= 8)
	{
		cout << "C: " << endl;
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
				cout << C[i * n + j] << "\t";
			cout << endl;
		}
		cout << endl << endl;
	}
#endif
}

void mul(int *c, int n, int blockSize = 0, int blockNumb_i = 0, int blockNumb_j = 0, int posInBlock = 0)
{
	if (blockSize == 0) 
		blockSize = n;
	for (int i = blockNumb_i; i < blockNumb_i + blockSize; i++)
	{
		for (int j = blockNumb_j; j < blockNumb_j + blockSize; j++)
		{
			for (int k = posInBlock; k < posInBlock + blockSize; k++)
			{
				C[i * n + j] += getA(i, k) * getB(k, j);
			}
		}
	}
}

int main()
{
#ifndef _OPENMP
	printf("OpenMP NOT OK\n");
#endif
#ifdef _OPENMP
	printf("OpenMP OK\n");
	for (int n = 4; n <= 512; n += n)
	{
		int amountElements = 0;
		for (int i = 0; i < n; i++)
			amountElements += n - i;
		cout << "amountElements = " << amountElements << endl << endl;

		A = new int[amountElements];
		B = new int[amountElements];
		C = new int[n*n];
		ofsA = new int[amountElements];
		ofsB = new int[amountElements];

		// Init Ofs
		ofsA[0] = 0;
		ofsB[0] = 0;
		for (int i = 1; i < n; i++)
		{
			ofsA[i] = ofsA[i - 1] + n - i + 1;
			ofsB[i] = ofsB[i - 1] + i;		
		}

		// fill A and B
		for (int i = 0; i < amountElements; i++)
		{
			A[i] = i;
			B[i] = i;
		}
		printA(n); printB(n);

		// Make  C zeros
		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++)
				C[i * n + j] = 0;

		double time_1, time_2;

		time_1 = omp_get_wtime();
		mul(C,n);
		time_2 = omp_get_wtime();
		cout << "Mullt. isBlockedMult : no\tsize = " << n << "\t\t---\t\t---\ttime is " << time_2 - time_1 << "sec." << endl;
		printC(n);

		time_1 = omp_get_wtime();
		mul(C, n, n, 0, 0, 0);
		time_2 = omp_get_wtime();
		cout << "Mullt. isBlockedMult : yes\tsize = " << n << "\t Threads : 1\tblockSize = " << n << "\ttime is " << time_2 - time_1 << "sec." << endl << endl;
		printC(n);


		for (int ThCount = 2; ThCount < 64; ThCount += ThCount)
		{
			for (int blockSize = 2; blockSize <= n; blockSize *= 2)
			{
				// Make  C zeros
				for (int i = 0; i < n; i++)
					for (int j = 0; j < n; j++)
						C[i * n + j] = 0;

				time_1 = omp_get_wtime();
				#pragma omp parallel num_threads(ThCount)
				for (int blockNumb_i = 0; blockNumb_i < n; blockNumb_i += blockSize)
					for (int blockNumb_j = 0; blockNumb_j < n; blockNumb_j += blockSize)
						#pragma omp for nowait
						for (int posInBlock = 0; posInBlock < n; posInBlock += blockSize)
							if (blockNumb_i >= posInBlock)
								mul(C, n, blockSize, blockNumb_i, blockNumb_j,posInBlock);
				time_2 = omp_get_wtime();
				cout << "Mullt. isBlockedMult : yes\tsize = "  << n << "\t Threads : "<< ThCount << "\tblockSize = " << blockSize << "\ttime is " << time_2 - time_1 << "sec." << endl;
				printC(n);
			}
			cout << endl;
		}
		cout << endl << endl;
	}
#endif	// _OPENMP
	system("pause");
	return 0;
}

