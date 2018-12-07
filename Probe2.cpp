//������ �.�. 2 ������

#include "stdafx.h"
#include <stdio.h>
#include "omp.h"
#include "assert.h"
#include <iostream>

using namespace std;

const int n = 480;
double *A, *B, **C, **T;
int blockSize = 8;


int sumProgression(int N) {
	return ((N + 1) * N) / 2;
}

void initTest() {
	A = new double[sumProgression(n)];
	B = new double[sumProgression(n)];
	C = new double *[n];
	T = new double *[n];
	for (int i = 0; i < sumProgression(n); ++i)
		A[i] = i;

	for (int i = 0; i < sumProgression(n); ++i)
		B[i] = i;

	for (int i = 0; i < n; ++i)
		C[i] = new double[n];

	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j)
			C[i][j] = 0;

	for (int i = 0; i < n; ++i)
		T[i] = new double[n];

	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j)
			T[i][j] = 0;
}

void initRand() {
	A = new double[sumProgression(n)];
	B = new double[sumProgression(n)];
	C = new double *[n];
	T = new double *[n];
	//a - ����� �������, b - ������ �������
	int a = 1, b = 99;
	for (int i = 0; i < sumProgression(n); ++i)
		A[i] = rand() % (b - a + 1) + a;

	for (int i = 0; i < sumProgression(n); ++i)
		B[i] = rand() % (b - a + 1) + a;

	for (int i = 0; i < n; ++i)
		C[i] = new double[n];

	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j)
			C[i][j] = 0;

	for (int i = 0; i < n; ++i)
		T[i] = new double[n];

	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j)
			T[i][j] = 0;
}

void print() {

	cout << "A:" << endl;
	int k = 0; //������ ��� ����, ����� ������� �����, ��� ������ ������� ������������
	int *index = new int[n]; //������ ��������
	for (int i = 0; i < n; ++i)
		index[i] = sumProgression(i);

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j)
			if (i > j) { //������ �����������
				index[j] += k;
				cout << A[index[j]] << " ";
			}
			else {// ������� �����������
				if (i == 0)
					cout << A[index[j]] << " ";
				else {
					index[j] += 1;
					cout << A[index[j]] << " ";
				}
			}
			k = i + 1;
			cout << endl;
	}

	cout << endl << "B:" << endl;
	k = 0;
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j)
			if (i >= j) { //������ ���������
				cout << B[k] << " ";
				++k;
			}
			else // ������� �����������
				cout << 0 << " ";
		cout << endl;
	}

	cout << endl << "C:" << endl;
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j)
			cout << C[i][j] << " ";
		cout << endl;
	}
}

int Aij(int ii, int jj) {

	int k = 0; //������ ��� ����, ����� ������� �����, ��� ������ ������� ������������
	int *index = new int[n]; //������ ��������
	for (int i = 0; i < n; ++i)
		index[i] = sumProgression(i);
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j)
			if (i > j) { //������ �����������
				index[j] += k;
				if (i == ii && j == jj) {
					//cout << A[index[j]] << " ";
					return A[index[j]];
				}
			}
			else {// ������� �����������
				if (i == 0) {
					if (i == ii && j == jj) {
						//cout << A[index[j]] << " ";
						return A[index[j]];
					}
				}
				else {
					index[j] += 1;
					if (i == ii && j == jj) {
						//cout << A[index[j]] << " ";
						return A[index[j]];
					}
				}
			}
			k = i + 1;
	}
}

int Bij(int ii, int jj) {

	int k = 0;
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j)
			if (i >= j) { //������ ���������
				if (i == ii && j == jj)
					return B[k];
				++k;
			}
			else // ������� �����������
				if (i == ii && j == jj)
					return 0;
	}
}

void multSimple(int blockSize) {
	int N = n / blockSize;
	//�������� ����������� ��������� ������ ������ ��������� ������
	for (int i = 0; i < N; ++i) {
		cout << "111" << " " << i << endl;
		for (int j = 0; j < N; ++j)
			//����� ������������ ������� ������ - ������� �������
			//1-� ������� ������ �, 1-� ������� ������� �, i = 0, j = 0
			//1-� ������� ������ �, 2-� ������� ������� �, i = 0, j = 1
			//2-� ������� ������ �, 1-� ������� ������� �, i = 1, j = 0
			//2-� ������� ������ �, 2-� ������� ������� �, i = 1, j = 1
			//� �.�....
			//block - �������� �� ������� ������/������� ������ �����
			for (int block = 0; block < N; ++block)
				//����� ����������� ���� ������
				for (int ii = i*blockSize; ii < (i + 1)*blockSize; ++ii)
					for (int jj = j*blockSize; jj < (j + 1)*blockSize; ++jj)
						for (int k = block*blockSize; k < (block + 1)*blockSize; ++k)
							//1-� ������ ����� �, 1-� ������� ����� �, ii = 0, jj = 0
							//1-� ������ ����� �, 2-� ������� ����� �, ii = 0, jj = 1
							//2-� ������ ����� �, 1-� ������� ����� �, ii = 1, jj = 0
							//2-� ������ ����� �, 2-� ������� ����� �, ii = 1, jj = 1
							//� �.�....
							//k - �������� �� ������/������� ������ �����
							C[ii][jj] += Aij(ii, k) * Bij(k, jj);
		
	}
		
}

void multThread(int blockSize) {
	int N = n / blockSize;
	omp_set_num_threads(N);

	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j)

#pragma omp parallel 
		{
			for (int ii = i*blockSize; ii < (i + 1)*blockSize; ++ii)
				for (int jj = j*blockSize; jj < (j + 1)*blockSize; ++jj)
					for (int k = omp_get_thread_num()*blockSize; k < (omp_get_thread_num() + 1)*blockSize; ++k)
#pragma omp atomic 
						C[ii][jj] += Aij(ii, k) * Bij(k, jj);
		}
		cout << "111 " << i << endl;
	}
				
}


void test() {
	
	double Accuracy = 1.e-6; 
	int equal = 0; 
	for (int i = 0; i < n; ++i) 
		for (int j = 0; j < n; ++j) 
			for (int k = 0; k < n; ++k) 
				T[i][j] += Aij(i, k) * Bij(k, j);
	for (int i = 0; i<n; ++i) 
		for (int j = 0; j < n; ++j) 
			if (fabs(T[i][j] - C[i][j]) >= Accuracy) {
				cout << T[i][j] << " " << C[i][j] << endl;
				assert(false);
			}
}


int main()
{
	
	if (blockSize > n || n % blockSize != 0 || n % 2 != 0 /*|| blockSize % 2 != 0*/)
		cout << "������! ������ ����� ������ ������� ������� ��� ���������� ��������� ������� �� �����!" << endl;
	else {
		double start_time, end_time, tick;
		
		
		//print();
		/*cout << "INIT" << endl;
		initRand();
		cout << "INIT" << endl;
		start_time = omp_get_wtime();
		multThread(blockSize);
		end_time = omp_get_wtime();
		test();
		cout << "Time with threads: " << end_time - start_time << endl;*/
		cout << "INIT" << endl;
		initRand();
		cout << "INIT" << endl;
		start_time = omp_get_wtime();
		multSimple(blockSize);
		end_time = omp_get_wtime();
		test();
		cout << "Time without threads: " << end_time - start_time << endl;
		//print();
		
	}



	delete[] A;
	delete[] B;
	for (int i = 0; i < n; i++) {
		delete[] C[i];
	}
	delete[] C;
	return 0;
}
