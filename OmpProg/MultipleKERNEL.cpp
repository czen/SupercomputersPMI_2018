#define _CRT_SECURE_NO_WARNINGS

#include "pch.h"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <chrono>
#include "matrix.h"
#include "TimeQuantMS2.h"



#define REP 1//0 // кол-во проходов для получения среднего значения времени


int MulKernel()
{
	srand(time(0));
	FILE *f;
	QuantMS2();
	int i, j, k, m, r, t;
	int pos;
	double* ResMatr = new double[SIZE*SIZE];
	double* Matr = new double[SIZE*SIZE];
	double* buff = new double[FULL_LINE];
	double* LinMatr1 = new double[LENGTH];
	double* LinMatr2 = new double[LENGTH];
	double* LinMatr1_Start;
	double* LinMatr2_Start;
	double* p;
	double CurSum;
	int kol_blocks = SIZE / BLOCK_SIZE;
	//-----------------------------------------------------------------
	for (pos = 0; pos < REP; pos++)
	{
		p = ResMatr;                    // инициализация результирующей матрицы нулями
		for (i = 0; i < SIZE*SIZE; i++)
		{
			p[i] = 0;
		}


		//чтение матрицы из файла и "выпрямление" поблочно
		f = fopen("matr1.bin", "rb");
		LinMatr1_Start = LinMatr1;
		for (i = 0; i < kol_blocks; i++)
		{
			fread(buff, sizeof(double), FULL_LINE, f);
			for (j = i; j < kol_blocks; j++)
			{
				p = buff + BLOCK_SIZE * j;
				for (k = 0; k < BLOCK_SIZE; k++)
				{
					for (m = 0; m < BLOCK_SIZE; m++, LinMatr1++, p++)
					{
						*LinMatr1 = *p;
					}
					p += SIZE - BLOCK_SIZE;
				}
			}
		}
		fclose(f);

		f = fopen("matr2.bin", "rb");
		LinMatr2_Start = LinMatr2;
		for (i = 0; i < kol_blocks; i++)
		{
			fread(buff, sizeof(double), BLOCK_SIZE*SIZE, f);
			for (j = i; j < kol_blocks; j++)
			{
				p = buff + BLOCK_SIZE * j;
				LinMatr2 = LinMatr2_Start + (j*FULL_LINE - (2 * kol_blocks - j - 1)*j*FULL_BLOCK / 2 + i * FULL_BLOCK);
				for (k = 0; k < BLOCK_SIZE; k++)
				{
					for (m = 0; m < BLOCK_SIZE; m++, LinMatr2++, p += SIZE)//-BLOCK_SIZE)
					{
						*LinMatr2 = *p;
					}
					p -= SIZE * BLOCK_SIZE - 1;
				}
			}
		}
		fclose(f);


		//основной цикл умножения
		LinMatr1 = LinMatr1_Start;
		LinMatr2 = LinMatr2_Start;
		int raw1, cal1, raw2, cal2, curraw;
#pragma omp parallel num_threads(4)
#pragma omp for schedule(guided)
		for (i = 0; i < kol_blocks; i++)
		{
			for (j = 0; j < kol_blocks; j++)
			{
				for (k = 0; (k < i) && (k <= j); k++)
				{
					raw1 = k * FULL_LINE - (k - 1)*k*FULL_BLOCK / 2;
					cal1 = (i - k)*FULL_BLOCK;
					raw2 = k * FULL_BLOCK;
					cal2 = j * FULL_LINE - (2 * kol_blocks - j - 1)*j*FULL_BLOCK / 2;
					for (m = 0; m < BLOCK_SIZE; m++)
					{
						for (r = 0; r < BLOCK_SIZE; r++)
						{
							CurSum = 0;
							for (t = 0; t < BLOCK_SIZE; t++)
							{
								CurSum += LinMatr1[raw1 + cal1 + t * BLOCK_SIZE + m] * LinMatr2[raw2 + cal2 + r * BLOCK_SIZE + t];
							}
							ResMatr[(i*BLOCK_SIZE + m)*SIZE + j * BLOCK_SIZE + r] += CurSum;
						}
					}
				}

				for (k = i; k <= j; k++)
				{
					raw1 = i * FULL_LINE - (i - 1)*i*FULL_BLOCK / 2;
					cal1 = (k - i)*FULL_BLOCK;
					raw2 = k * FULL_BLOCK;
					cal2 = j * FULL_LINE - (2 * kol_blocks - j - 1)*j*FULL_BLOCK / 2;
					for (m = 0; m < BLOCK_SIZE; m++)
					{
						curraw = m * BLOCK_SIZE;
						for (r = 0; r < BLOCK_SIZE; r++)
						{
							CurSum = 0;
							for (t = 0; t < BLOCK_SIZE; t++)
							{
								CurSum += LinMatr1[raw1 + cal1 + curraw + t] * LinMatr2[raw2 + cal2 + r * BLOCK_SIZE + t];
							}
							ResMatr[(i*BLOCK_SIZE + m)*SIZE + j * BLOCK_SIZE + r] += CurSum;
						}
					}
				}
			}
		}

	}

	// Усредненное время работы цикла умножения
	const auto elapsed2 = QuantMS2();

	std::cout << "Time = " << elapsed2 / 1000000.0 << " Sec\n";

	//Сохранение результата умножения для последующего умножения
	f = fopen("res.bin", "wb");
	fwrite(ResMatr, sizeof(double), SIZE*SIZE, f);
	fclose(f);

	// Освобождение памяти
	delete[]ResMatr;
	delete[]LinMatr1;
	delete[]LinMatr2;
	delete[]buff;
	return 0;
}
