// SuperPcIndIvaVar35.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include "stdafx.h"
#include <iostream>
#include "omp.h"
#include "windows.h"
#include <vector>
using namespace std;

int *off;
int *A;
int *B;
int **C;
int sizeBox;
const int N = 256;	// ����������� ������� A � B

// ������� ���������� ��������� � ������� ++
int countOfVectorEl(int n) {
	return ((2 + (n - 1))*n) / 2;
}


// ��������� �������� �� ������� � ����������� � ��� ��� �� �� ����� � ���������������� ������� N*N
int getNizTreygolEl(int i, int j) {
	return j <= i ? A[off[j] - (N - i)] : 0;
}

// ��������� �������� �� ������� � ����������� � ��� ��� �� �� ����� � ������������ ������� N*N
int getSimetrMatrixEl(int i, int j) {
	return j <= i ? B[off[j] - (N - i)] : B[off[i] - (N - j)];
}


// ������ ���������������� �������
void PrintNizTreygol() {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cout << getNizTreygolEl(i, j) << "\t";
		}
		cout << endl;
	}
	cout << endl;
}

// ������ ������������ �������
void PrintSimetrMatrix() {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cout << getSimetrMatrixEl(i, j) << "\t";
		}
		cout << endl;
	}
	cout << endl;
}

// ������ ����������
void PrintResult() {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cout << C[i][j] << "\t";
		}
		cout << endl;
	}
	cout << endl;
}


// ������� ����� ��������� �������
void boxSolver(int x1, int y1, int k1) {
	for (int i = x1; i < x1 + sizeBox; i++) {
		for (int j = y1; j < y1 + sizeBox; j++) {
			for (int k = k1; k < k1 + sizeBox; k++) {
				C[i][j] += getNizTreygolEl(i, k) * getSimetrMatrixEl(k, j);
				
			}
		}
	}
}

void boxSolver1(int x1, int y1, int k1, int numThread) {
#pragma omp parallel num_threads(numThread)
	{
#pragma omp for
		for (int i = x1; i < x1 + sizeBox; i++) {
			for (int j = y1; j < y1 + sizeBox; j++) {
				for (int k = k1; k < k1 + sizeBox; k++) {
					C[i][j] += getNizTreygolEl(i, k) * getSimetrMatrixEl(k, j);

				}
			}
		}
	}
}



int main()
{

	// ���������� ����������� ��������
	int countOfVectorElements = countOfVectorEl(N);		// ������������ ��������
	C = new int * [N];
	for (int i = 0; i < N; i++) {
		C[i] = new int[N];
	}
	A = new int[countOfVectorElements];
	B = new int[countOfVectorElements];
	
	off = new int[N];
	for (int i = 0; i < N; i++) {
		off[i] = (((2 * N - (i))*(i + 1)) / 2);
	}

	double mintime = _MAX_INT_DIG;
	int minthread;
	int minblock;
	// ���������� �������� A � B
	for (int i = 0; i < countOfVectorElements; i++) {
		A[i] = rand() % 9;
		B[i] = rand() % 9;
	}

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			C[i][j] = 0;
		}
	}

	// ���������� ��� �������
	double start = 0;
	double end = 0;

	//PrintNizTreygol();
	//PrintSimetrMatrix();
	sizeBox = N;
	cout << "Start standart multi" << endl;
	start = omp_get_wtime();
	boxSolver(0, 0, 0);
	end = omp_get_wtime();
	cout << "standart multi time " << end - start << endl;

	//PrintResult();

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			C[i][j] = 0;
		}
	}
	for (int numThread = 1; numThread < 3; numThread++) {
		for (int l = 2; l < N; l *= 2) {
			sizeBox = l;
			int newN = N / sizeBox;
			start = omp_get_wtime();
#pragma omp parallel num_threads(numThread)
			{
#pragma omp for
				for (int i = 0; i < newN; i++) {
					for (int j = 0; j < newN; j++){ 
							for (int k = 0; k < newN; k++) {
								if (k <= i) {
									boxSolver(i*sizeBox, j*sizeBox, k*sizeBox);
								}
						}
					}
				}
			}
			end = omp_get_wtime();
			if ((end - start) <= mintime) {
				minthread = numThread;
				minblock = l;
				mintime = end - start;
			}
			cout << "Block " << l << " \tThread " << numThread << " \t" << end - start << endl;
		}
	}
	cout<< "Result: " << "Block " << minblock << " \tThread " << minthread << " \t" << mintime << endl;

	for (int numThread = 1; numThread < 3; numThread++) {
		for (int l = 2; l < N; l *= 2) {
			sizeBox = l;
			int newN = N / sizeBox;
			start = omp_get_wtime();
			for (int i = 0; i < newN; i++) {
				for (int j = 0; j < newN; j++) {
					for (int k = 0; k < newN; k++) {
						if (k <= i) {
							boxSolver1(i*sizeBox, j*sizeBox, k*sizeBox, numThread);
						}
					}
				}
			}
			end = omp_get_wtime();
			if ((end - start) <= mintime) {
				minthread = numThread;
				minblock = l;
				mintime = end - start;
			}
			cout << "Block " << l << " \tThread " << numThread << " \t" << end - start << endl;
		}
		
	}
	cout << "Result: " << "Block " << minblock << " \tThread " << minthread << " \t" << mintime << endl;
	//PrintResult();

	return 0;
}

