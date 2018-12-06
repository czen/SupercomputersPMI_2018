// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

#ifndef PCH_H
#define PCH_H

#define NUM_THREAD 25
#define SIZE_ARR SIZE_MATRIX * SIZE_MATRIX - ((SIZE_MATRIX  * SIZE_MATRIX - SIZE_MATRIX) / 2)
#define SIZE_MATRIX 1000

double* CreateBlock(double * m, size_t size_m, size_t size_block);
double* CreateMatrixA(double * m, size_t size_m);
double* CreateMatrixB(double * m, size_t size_m);
double* Multiply(double* A, double*B, size_t size_m, size_t size_block);
double* MultiplyKernel(double* A, double*B, size_t size_m, size_t size_block);
double* MultiplyBlock(double* A, double*B, size_t size_m, size_t size_block);
void ChronoTime();
// TODO: add headers that you want to pre-compile here

#endif //PCH_H
