#include "pch.h"
#include <iostream>

double* MultiplyBlock(double* A, double*B, size_t size_m, size_t size_block) {
	double* C = new double[size_m*size_m];
	for (size_t j = NULL; j < size_m; ++j)
	{
		for (size_t i = NULL; i < size_m; ++i)
		{
			C[j*size_m + i] = NULL;
		}
	}
	size_t p1, p2, p3, p4, p;
#pragma omp parallel num_threads(NUM_THREAD), shared(A, B,size_block,size_m,C), private(p1,p2,p3,p4,p)
	{
		p1 = NULL, p2 = (-1)*size_m;
		p3 = NULL, p4 = NULL, p = NULL;
#pragma omp for schedule(dynamic,2)
		for (size_t jk = NULL; jk < size_m; jk += size_block)
		{
			p3 = NULL, p4 = NULL, p = NULL;
			p2 += size_m;
			for (size_t kk = NULL; kk < size_m; kk += size_block)
			{
				for (size_t ik = NULL; ik < size_m; ik += size_block)
				{
					p1 = NULL;
					for (size_t j = NULL; j < size_block; ++j)
					{
						p = p3;
						for (size_t k = NULL; k < size_block; ++k)
						{
							for (size_t i = NULL; i < size_block; ++i)
							{
								C[(jk + j)*size_m + (ik + i)] += B[p + p4] * A[p1 + p2];
								p++;
							}
							p1++;
						}
					}
					p4 += size_block;
					p3 += size_block;
				}
			}
		}
	}
	return C;
}