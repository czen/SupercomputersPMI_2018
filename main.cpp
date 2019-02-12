#include <iostream>
#include "func.h"
#include <time.h>
#include <omp.h>

using namespace std;



int main()
{
	int n = 500;
	/*int div[] = { 1, 2, 3, 4, 5, 6, 8, 9, 10, 12, 15, 16, 18,
		20, 24, 30, 32, 36, 40, 45, 48, 60, 64, 72, 80, 90, 96,
		120,  144,  160,  180,  192,  240,  288,  320,  360,  480,
		576,  720,  960, 1440, 2880 };*/
	int div[] = { 1, 2, 4, 5, 10, 20, 25, 50,  100,  125,  250,  500 };


	create_A_to_text("matrA.txt", n);
	create_B_to_text("matrB.txt", n);

	int *matr_A = 0; int  *matr_B = 0;
	int nA, nB;

	read_from_text("matrA.txt", matr_A, nA);
	read_from_text("matrB.txt", matr_B, nB);

	int *vec_A = (int*)calloc(nA*nA, sizeof(int));
	int *vec_B = (int*)calloc(nB*nB, sizeof(int));

	FILE *fp; FILE *fp2;
	fopen_s(&fp, "results.txt", "w");
	fopen_s(&fp2, "results2.txt", "w");

	if (!fp)
		perror("no file");

	for (int i = 0; i < 12; i++)
	{
		int p = div[i];

		create_A_array(matr_A, vec_A, p, nA);
		create_B_array(matr_B, vec_B, p, nB);

		int *vec_C = (int*)calloc(nA*nA, sizeof(int));

		int tt = clock();
		block_mul(vec_A, vec_B, vec_C, n, p);
		fprintf(fp, "%d ; %f\n", p, (double)(clock() - tt) / CLOCKS_PER_SEC);

		tt = clock();
		block_mul_parallel(vec_A, vec_B, vec_C, n, p);
		fprintf(fp2, "%d ; %f\n", p, (double)(clock() - tt) / CLOCKS_PER_SEC);

		free(vec_C);

	}
	free(vec_A);
	free(vec_B);

	free(matr_A);
	free(matr_B);

	system("pause");
	return 0;
}
