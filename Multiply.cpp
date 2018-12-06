#include "pch.h"
#include <iostream>

double* Multiply(double* A, double*B, size_t size_m, size_t size_block) 
{
	double* C = new double[size_m*size_m];
	for (size_t j = NULL; j < size_m; ++j)
	{
		for (size_t i = NULL; i < size_m; ++i)
		{
			C[j*size_m + i] = NULL;
		}
	}
	for (size_t jk = NULL; jk < size_m; jk += size_block)
	{
		for (size_t kk = NULL; kk < size_m; kk += size_block)
		{
			for (size_t ik = NULL; ik < size_m; ik += size_block)
			{
				for (size_t j = NULL; j < size_block; ++j)
				{
					for (size_t k = NULL; k < size_block; ++k)
					{
						for (size_t i = NULL; i < size_block; ++i)
						{
							C[(jk + j)*size_m + (ik + i)] += A[(jk + j)*size_m + (kk + k)] * B[(kk + k)*size_m + (ik + i)];
						}
					}
				}
			}
		}
	}
	return C;
}
