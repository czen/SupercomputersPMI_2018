// Matrix Multiplying
// Rudnev Danil
#include <iostream>
#include <omp.h>

using namespace std;

// n = 4 8 256 512 1024
// ThreadAmount = 2 4 8 16 32 64

int *A, *B, *C, *offsets;
double startTime, endTime;

int getA(int i, int j)
{
	if (i > j)
		return 0;
	return A[j - i + offsets[i]];
}

int getB(int i, int j)
{
	if (j > i)
		swap(i, j);
	return B[i - j + offsets[j]];
}

int main()
{
#ifndef _OPENMP
	printf("OpenMP is NOT supported!\n");
#endif

#ifdef _OPENMP
	printf("OpenMP is supported!\n");

	for (int n = 8; n <= 512; n *= 2)
	{
		int amountElements = 0;
		for (int k = 0; k < n; k++)
			amountElements += n - k;
		A = new int[amountElements];
		B = new int[amountElements];
		C = new int[n*n];
		offsets = new int[n];

		// Counting offsets
		offsets[0] = 0;
		for (int i = 1; i < n; i++)
		{
			offsets[i] = offsets[i - 1] + n - i + 1;
		}

		// Input A
		for (int i = 0; i < amountElements; i++)
		{
			A[i] = i;
		}

		// Input B
		for (int i = 0; i < amountElements; i++)
		{
			B[i] = i;
		}

		// Init C
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				C[i * n + j] = 0;
			}
		}

		startTime = omp_get_wtime();
		for (int k = 0; k < n; k++)
		{
			for (int z = 0; z < n; z++)
			{
				for (int p = 0; p < n; p++)
				{
					C[k * n + z] += getA(k, p) * getB(p, z);
				}
			}
		}
		endTime = omp_get_wtime();
		cout << "Usual multiplying" << "\ttime is " << endTime - startTime << "sec." << endl;

		// Calling Main Function
		for (int ThreadAmount = 1; ThreadAmount < 64; ThreadAmount *= ((ThreadAmount == 1) ? 2 : ThreadAmount))
		{
			for (int base = 2; base <= n; base *= 2)
			{
				// Init C
				for (int i = 0; i < n; i++)
				{
					for (int j = 0; j < n; j++)
					{
						C[i * n + j] = 0;
					}
				}

				startTime = omp_get_wtime();
				int power = (int)(log(n) * 1.0 / log(base));
				#pragma omp parallel num_threads(ThreadAmount)
				for (int k = 0; k < n; k += base)
				{
					for (int z = 0; z < n; z += base)
					{
						#pragma omp for nowait
						for (int p = 0; p < n; p += base)
						{
							if (k <= p)
							{
								for (int kk = k; kk < k + base; kk++)
								{
									for (int zz = z; zz < z + base; zz++)
									{
										for (int pp = p; pp < p + base; pp++)
										{
											C[kk * n + zz] += (kk > pp) ? 0 : getA(kk, pp) * getB(pp, zz);
										}
									}
								}
							}
						}
					}
				}
				endTime = omp_get_wtime();
				cout << "n = " << n << "\tThreadAmount = " << ThreadAmount << "\tbase = " << base << "\ttime is " << endTime - startTime << "sec." << endl;
			}
			cout << endl;
		}
		cout << endl << endl;
	}	

	/*if (n <= 8)
	{
		// Print A
		cout << "A : " << endl;
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				cout << getA(i, j) << "\t";
			}
			cout << endl;
		}
		cout << endl;

		// Print B
		cout << "B : " << endl;
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				cout << getB(i, j) << "\t";
			}
			cout << endl;
		}
		cout << endl;

		// Print C
		cout << "C : " << endl;
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				cout << C[i*n + j] << "\t";
			}
			cout << endl;
		}
		cout << endl;
	}*/

#endif	// _OPENMP
	system("pause");
	return 0;
}