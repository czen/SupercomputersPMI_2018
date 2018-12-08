// FastMatrix.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <ctime>

using namespace std;

#define BLOCK_SIZE 64
#define SIZE_MATRIX 2048

int main() 
{
	setlocale(LC_ALL, "Rus");
	srand(time((unsigned int)0));
	timetest(SIZE_MATRIX, BLOCK_SIZE);

	correctness(8, 2);

	return 0;

}