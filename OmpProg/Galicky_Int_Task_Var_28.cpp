// Galicky_Int_Task_Var_28.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>

int main(int argc, char** argv)
{
	Create();
	std::cout << "Normal : " << std::endl;
	Mul();
	std::cout << "Block : " << std::endl;
	MulBlock();
	std::cout << "Kernel : " << std::endl;
	MulKernel();
	Check();

	return 0;
}