#include "pch.h"
#include <iostream>

double* CreateBlock(double * m, size_t size_m, size_t size_block)
{
	double* res = new double[size_m*size_m];
	size_t pe = NULL;
	for (size_t jk = NULL; jk < size_m; jk += size_block)
	{
		for (size_t kk = NULL; kk < size_m; kk += size_block)
		{
			for (size_t j = NULL; j < size_block; ++j)
			{
				for (size_t k = NULL; k < size_block; ++k)
				{
					res[pe] = m[(jk + j)*size_m + (kk + k)];
					pe++;
				}
			}
		}
	}
	return res;
}
double* CreateMatrixA(double * m, size_t size_m)
{
	double * res = new double[size_m*size_m];
	size_t pos = NULL, pe = NULL;
	for (size_t i = NULL; i < size_m; ++i)
	{
		for (size_t j = NULL; j < size_m; ++j)
		{
			if (i < j)
			{
				res[pe] = NULL;
			}
			else
			{
				res[pe] = m[pos];
				pos++;
			}
			pe++;
		}
	}
	return res;
}
double* CreateMatrixB(double * m, size_t size_m)
{
	double * res = new double[size_m*size_m];
	size_t pos = NULL;
	size_t p = NULL, pe = NULL;
	size_t length, ll = 2, l = NULL;
	for (size_t i = NULL; i < size_m; ++i)
	{
		size_t pos_blok = i + 1, pos_elem = 2 * i, s = l;
		length = ll;
		l += ll;
		for (size_t j = NULL; j < size_m; ++j)
		{
			if (i < j)
			{
				size_t sz = pos_blok + s;
				res[pe] = m[sz];
				pe++;
				p++;
				s += length;
				length++;
			}
			else
			{
				res[pe] = m[pos];
				pe++;
				pos++;
			}
		}
		ll++;
	}
	return res;
}
