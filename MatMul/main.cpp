#include <iostream>
#include <omp.h>
using namespace std;


double* rowBlock(double * m, size_t size_m, size_t size_block);
double* rowMatrixA(double * m, size_t size_m);
double* rowMatrixB(double * m, size_t size_m);
double* multRowMAtrixNonParallel(double* A, double*B, size_t size_m, size_t size_block);
double* multRowMAtrixParallel(double* A, double*B, size_t size_m, size_t size_block);
double* multBlockMAtrixParallel(double* A, double*B, size_t size_m, size_t size_block);

int main() {
	setlocale(LC_ALL, "Rus");
	size_t size_m = 1024 , size_block;
	double *A1 = new double[size_m*size_m];
	double *A2 = new double[size_m*size_m];
	double *B1 = new double[size_m*size_m];
	double *B2 = new double[size_m*size_m];
	size_t pos = 0;
	int size_arr = size_m  * size_m - ((size_m  * size_m - size_m) / 2);
	double *A = new double[size_arr];
	for (int i = 0; i < size_m; ++i)
		for (int j = 0; j < size_m; ++j) {
			if (i >= j) {
				A[pos] = rand() % 10 + 1;
				pos++;
			}
		}
	pos = 0;
	double *B = new double[size_arr];
	for (int i = 0; i < size_m; ++i)
		for (int j = 0; j < size_m; ++j) {
			if (i >= j) {
				B[pos] = rand() % 10 + 1;
				pos++;
			}
		}
	cout << "Count of thread=32" << endl;
	cout << endl;
	size_block = 2;
	while (size_block <= 256) {
		A1 = rowMatrixA(A, size_m);
		A2 = rowBlock(A1, size_m, size_block);
		B1 = rowMatrixB(B, size_m);
		B2 = rowBlock(B1, size_m, size_block);
		cout << "Blok's size=" << size_block << endl;
		double start_time, end_time;
		start_time = omp_get_wtime();
		double* C1 = multRowMAtrixNonParallel(A1, B1, size_m, size_block);
		end_time = omp_get_wtime();
		cout << "NonParallel time=" << end_time - start_time << endl;
		start_time = omp_get_wtime();
		C1 = multRowMAtrixParallel(A1, B1, size_m, size_block);
		end_time = omp_get_wtime();
		cout << "Parallel time by RowMAtrix=" << end_time - start_time << endl;
		start_time = omp_get_wtime();
		C1 = multBlockMAtrixParallel(A2, B2, size_m, size_block);
		end_time = omp_get_wtime();
		cout << "Parallel time by BlockMAtrix=" << end_time - start_time << endl;
		size_block *= 2;
		cout << endl;
	}
	system("pause");
	return 0;
}

double* rowBlock(double * m, size_t size_m, size_t size_block) {
	double* res = new double[size_m*size_m];
	int pe = 0;
	for (int jk = 0; jk < size_m; jk += size_block) {
		for (int kk = 0; kk < size_m; kk += size_block) {
			for (int j = 0; j < size_block; ++j) {
				for (int k = 0; k < size_block; ++k) {
					res[pe] = m[(jk + j)*size_m + (kk + k)];
					pe++;
				}
			}
		}

	}
	return res;
}

double* rowMatrixA(double * m, size_t size_m) {
	double * res = new double[size_m*size_m];
	int pos = 0, pe = 0;
	for (int i = 0; i < size_m; ++i) {
		for (int j = 0; j < size_m; ++j) {
			if (i < j)
				res[pe] = 0;
			else {
				res[pe] = m[pos];
				pos++;
			}
			pe++;
		}
	}
	return res;
}

double* rowMatrixB(double * m, size_t size_m) {
	double * res = new double[size_m*size_m];
	int pos = 0;
	int p = 0, pe = 0;
	int length, ll = 2, l = 0;
	for (int i = 0; i < size_m; ++i) {
		int pos_blok = i + 1, pos_elem = 2 * i, s = l;
		length = ll;
		l += ll;
		for (int j = 0; j < size_m; ++j) {
			if (i < j) {
				int sz = pos_blok + s;
				res[pe] = m[sz];
				pe++;
				p++;
				s += length;
				length++;
			}
			else {
				res[pe] = m[pos];
				pe++;
				pos++;
			}
		}
		ll++;

	}
	return res;
}

double* multRowMAtrixNonParallel(double* A, double*B, size_t size_m, size_t size_block) {
	double* C = new double[size_m*size_m];
	for (int j = 0; j < size_m; ++j)
		for (int i = 0; i < size_m; ++i)
			C[j*size_m + i] = 0;
	for (int jk = 0; jk < size_m; jk += size_block)
		for (int kk = 0; kk < size_m; kk += size_block)
			for (int ik = 0; ik<size_m; ik += size_block)
				for (int j = 0; j < size_block; ++j)
					for (int k = 0; k < size_block; ++k)
						for (int i = 0; i < size_block; ++i)
							C[(jk + j)*size_m + (ik + i)] += A[(jk + j)*size_m + (kk + k)] * B[(kk + k)*size_m + (ik + i)];

	return C;
}

double* multRowMAtrixParallel(double* A, double*B, size_t size_m, size_t size_block) {
	double* C = new double[size_m*size_m];
	for (int j = 0; j < size_m; ++j)
		for (int i = 0; i < size_m; ++i)
			C[j*size_m + i] = 0;
#pragma omp parallel num_threads(32), shared(A, B,size_block,size_m,C)
	{
#pragma omp for schedule(dynamic,2)
		//#pragma omp for 
		for (int jk = 0; jk < size_m; jk += size_block)
			for (int kk = 0; kk < size_m; kk += size_block)
				for (int ik = 0; ik < size_m; ik += size_block)
					for (int j = 0; j < size_block; ++j)
						for (int k = 0; k < size_block; ++k)
							for (int i = 0; i < size_block; ++i)
								C[(jk + j)*size_m + (ik + i)] += A[(jk + j)*size_m + (kk + k)] * B[(kk + k)*size_m + (ik + i)];
	}
	return C;
}

double* multBlockMAtrixParallel(double* A, double*B, size_t size_m, size_t size_block) {
	double* C = new double[size_m*size_m];
	for (int j = 0; j < size_m; ++j)
		for (int i = 0; i < size_m; ++i)
			C[j*size_m + i] = 0;
	int p1, p2,p3,p4,p;
#pragma omp parallel num_threads(32), shared(A, B,size_block,size_m,C), private(p1,p2,p3,p4,p)
	{
        p1 = 0, p2 = (-1)*size_m;
		p3 = 0, p4 = 0,p=0;
#pragma omp for schedule(dynamic,2)
//#pragma omp for 
		for (int jk = 0; jk < size_m; jk += size_block) {
			p3 = 0, p4 = 0, p = 0;
			p2 += size_m;
			for (int kk = 0; kk < size_m; kk += size_block) {
				for (int ik = 0; ik < size_m; ik += size_block) {
					p1 = 0;
					for (int j = 0; j < size_block; ++j) {
						p = p3;
						for (int k = 0; k < size_block; ++k) {

							for (int i = 0; i < size_block; ++i) {
								C[(jk + j)*size_m + (ik + i)] += B[p + p4]*A[p1 + p2];
								p++;
							}
							p1++;
						}
					}
					p4 += size_block;
					p3 += size_block;
				}
			}
		}}

	return C;
}