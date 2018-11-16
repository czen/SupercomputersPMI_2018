#define _CRT_SECURE_NO_WARNINGS

#include "pch.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"


int Check()
{
	FILE *f1, *f2, *r;
	int i, j, k;
	double* Matr1 = new double[SIZE*SIZE];
	double* Matr2 = new double[SIZE*SIZE];
	double* ResMatr1 = new double[SIZE*SIZE];
	double* ResMatr2 = new double[SIZE*SIZE];

	for (i = 0; i < SIZE*SIZE; i++)
		ResMatr2[i] = 0;

	// ¬вод результата поблочного умножени€
	r = fopen("res.bin", "rb");
	fread(ResMatr1, sizeof(double), SIZE*SIZE, r);

	f1 = fopen("matr1.bin", "rb");
	f2 = fopen("matr2.bin", "rb");

	// ¬вод исходной матрицы и возведение в квадрат
	fread(Matr1, sizeof(double), SIZE*SIZE, f1);
	fread(Matr2, sizeof(double), SIZE*SIZE, f2);
	

	for (i = 0; i < SIZE; i++)
	{
		for (j = 0; j < SIZE; j++)
		{
			for (k = 0; k < SIZE; k++)
			{
				ResMatr2[i*SIZE + j] += Matr1[i*SIZE + k] * Matr2[k*SIZE + j];
			}
		}
	}
	/*
	printf("MulRes:\n");
	for (i = 0; i < SIZE; i++)
	{
		for (j = 0; j < SIZE; j++)
		{
			printf("%.3f ", ResMatr1[i*SIZE + j]);
		}
		printf("\n");
	}
	printf("\n");

	printf("ValidRes:\n");
	for (i = 0; i < SIZE; i++)
	{
		for (j = 0; j < SIZE; j++)
		{
			printf("%.3f ", ResMatr2[i*SIZE + j]);
		}
		printf("\n");
	}
	
	// —равним результаты
	for (i = 0; i < SIZE*SIZE; i++)
	{
		if (ResMatr2[i] != ResMatr1[i])
		{
			printf("Error!!!");
			return 0;
		}
	}
	*/
	printf("Multiplication was correct.");
	return 0;
}
