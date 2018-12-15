#define _CRT_SECURE_NO_WARNINGS

#include "pch.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include "matrix.h"


int Create()
{
	srand(time(0));

	FILE *f;
	int i, j;

	f = fopen("matr1.bin", "wb");
	double* matr = new double[SIZE * SIZE];
	for (i = 0; i < SIZE; i++)
	{
		for (j = 0; j < SIZE; j++)
		{
			double a;
			if (j < i)
			{
				a = matr[j*SIZE + i];
			}
			else
			{
				a = (double)(rand() % 10);
			}
			matr[i*SIZE + j] = a;
			fwrite(&a, sizeof(double), 1, f);
		}
	}
	fclose(f);

	f = fopen("matr2.bin", "wb");
	for (i = 0; i < SIZE; i++)
	{
		for (j = 0; j < SIZE; j++)
		{
			double a;
			if (j < i)
			{
				a = 0;
			}
			else
			{
				a = (double)(rand() % 10);
			}
			fwrite(&a, sizeof(double), 1, f);
		}
	}
	fclose(f);

	return 0;
}
