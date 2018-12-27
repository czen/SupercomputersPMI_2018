#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

const int N = 1024;

double multiply (double *A, double *B, double *&C, int n) {
  memset(C, 0, N * N * sizeof(double));

  int num = N / n;
  double *a, *b, *c;
  double timeStart = omp_get_wtime();

  for (int i = 0; i < num; i++)
  for (int j = 0; j < num; j++) {
    int k;
    for (k = 0; k < i && k <= j; k++) {
      a = A + (i * (i + 1) / 2 + k) * n * n;
      b = B + (j + k * (num - k) + k * (k - 1) / 2) * n * n;
      c = C + i * N * n + j * n * n;
      for (int ii = 0; ii < n; ii++)
      for (int jj = 0; jj < n; jj++)
      for (int kk = 0; kk < n; kk++)
        c[ii * n + jj] += a[ii * n + kk] * b[kk * n + jj];
    }
    if (k <= j) {
      a = A + (k *(k + 1) / 2 + i) * n * n;
      b = B + (j + k * (num - k) + k * (k - 1) / 2) * n * n;
      c = C + i * N * n + j * n * n;
      for (int ii = 0; ii < n; ii++)
      for (int jj = 0; jj < n; jj++)
      for (int kk = 0; kk < n; kk++)
        c[ii * n + jj] += a[((ii < kk) ? (kk * n + ii) : (ii * n + kk))] * b[kk* n + jj];
    }
    for (k++; k <= j; k++) {
      a = A + (k *(k + 1) / 2 + i) * n * n;
      b = B + (j + k * (num - k) + k * (k - 1) / 2) * n * n;
      c = C + i * N * n + j * n * n;
      for (int ii = 0; ii < n; ii++)
      for (int jj = 0; jj < n; jj++)
      for (int kk = 0; kk < n; kk++)
        c[ii * n + jj] += a[kk * n + ii] * b[kk * n + jj];
    }
  }

  return omp_get_wtime() - timeStart;
}

double multiplyParallel (double *A, double *B, double *&C, int n) {
  memset(C, 0, N * N * sizeof(double));

  int num = N / n;
  double *a, *b, *c;
  double timeStart = omp_get_wtime();  

#pragma omp parallel num_threads(8) 
{
#pragma omp for schedule(static)
  for (int i = 0; i < num; i++)
  for (int j = 0; j < num; j++) {
    int k;
    for (k = 0; k < i && k <= j; ++k) {
      a = A + (i * (i + 1) / 2 + k) * n * n;
      b = B + (j + k * (num - k) + k * (k - 1) / 2) * n * n;
      c = C + i * N * n + j * n * n;
      for (int ii = 0; ii < n; ii++)
      for (int jj = 0; jj < n; jj++)
      for (int kk = 0; kk < n; kk++)
        c[ii * n + jj] += a[ii * n + kk] * b[kk * n + jj];
    }
    if (k <= j){
      a = A + (k * (k + 1) / 2 + i) * n * n;
      b = B + (j + k * (num - k) + k * (k - 1) / 2) * n * n;
      c = C + i * N * n + j * n * n;
      for (int ii = 0; ii < n; ii++)
      for (int jj = 0; jj < n; jj++)
      for (int kk = 0; kk < n; kk++)
        c[ii * n + jj] += a[((ii < kk) ? (kk * n + ii) : (ii * n + kk))] * b[kk * n + jj];
    }
    for (k++; k <= j; k++) {
      a = A + (k * (k + 1) / 2 + i) * n * n;
      b = B + (j + k * (num - k) + k * (k - 1) / 2) * n * n;
      c = C + i * N * n + j * n * n;
      for (int ii = 0; ii < n; ii++)
      for (int jj = 0; jj < n; jj++)
      for (int kk = 0; kk < n; kk++)
        c[ii * n + jj] += a[kk * n + ii] * b[kk * n + jj];
    }
  }
}
  return omp_get_wtime() - timeStart;
}

void getBlock (double *A, double *&buffer, int size) {
  memcpy(buffer, A, size * size * sizeof(double));
}

double multiplyParallelBlock (double *A, double *B, double *&C, int n) {
  memset(C, 0, N * N * sizeof(double));

  int num = N / n;
  double *a, *b, *c;
  double timeStart = omp_get_wtime();

  a = new double[n];
  b = new double[n];


#pragma omp parallel num_threads(6) 
{
#pragma omp for schedule(static)
  for (int i = 0; i < num; i++)
  for (int j = 0; j < num; j++) {
    int k;
    for (k = 0; k < i && k <= j; k++) {
      c = C + (i * N + j * n) * n;
      getBlock(A + (i *(i + 1) / 2 + k) * n * n, a, n);
      getBlock(B + (j + k * (num - k) + k * (k - 1) / 2) * n * n, b, n);
      for (int ii = 0; ii < n; ++ii)
      for (int jj = 0; jj < n; ++jj)
      for (int kk = 0; kk < n; ++kk)
        c[ii * n + jj] += a[ii * n + kk] * b[kk * n + jj];
    }
    c = C + (i * N + j * n) * n;
    getBlock(A + (k *(k + 1) / 2 + i) * n * n, a, n);
    getBlock(B + (j + k * (num - k) + k * (k - 1) / 2) * n * n, b, n);
    for (int ii = 0; ii < n; ii++)
    for (int jj = 0; jj < n; jj++)
    for (int kk = 0; kk < n; kk++)
      c[ii * n + jj] += a[(ii < kk) ? (kk * n + ii) : (ii * n + kk)] * b[kk* n + jj];
    for (k++; k <= j; k++) {
      c = C + (i * N + j * n) * n;
      getBlock(A + (k *(k + 1) / 2 + i) * n * n, a, n);
      getBlock(B + (j + k * (num - k) + k * (k - 1) / 2) * n * n, b, n);
      for (int ii = 0; ii < n; ii++)
      for (int jj = 0; jj < n; jj++)
      for (int kk = 0; kk < n; kk++)
        c[ii * n + jj] += a[kk * n + ii] * b[kk * n + jj];
    }
  }
}
  delete []a;
  delete []b;

  return omp_get_wtime() - timeStart;
}

void displayMatrix (double *A) {
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      printf("%f ", A[N * i + j]);
    }
    printf("\n");
  }
}

void initialize (double *&A, int size = N, int value = 0) {
  A = new double[size];
  for (int i = 0; i < size; i++) {
    A[i] = value;
  }
}

int main () {

  double t1, t2, t3;
  double *a, *b, *c;

  initialize(a, N * (N + 1) / 2, 1);
  initialize(b, N * (N + 1) / 2, 1);
  initialize(c, N * N);

  int i = 1;
  int blockSize = 1;

  while (blockSize < N) {
    t1 = multiply(a, b, c, blockSize);
    t2 = multiplyParallel(a, b, c, blockSize);
    t3 = multiplyParallelBlock(a, b, c, blockSize);

    printf("%d.\t%d\t%.2f\t%.2f\t%.2f\n", i, blockSize, t1, t2, t3);

    blockSize = (blockSize << 1);
  }

  delete []a;
  delete []b;
  delete []c;

  return 0;
}
