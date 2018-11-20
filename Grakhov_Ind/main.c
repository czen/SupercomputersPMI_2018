
//#define PRINT_MATRICES

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "blockmatrix.h"

void correctness(int appsize, int blocksize)
{
    upper_triangular_matrix a;
    upper_triangular_matrix b;
    block_matrix c;

    int size = appsize / blocksize;
    init_upper_triangular_matrix(a, size, blocksize);
    init_upper_triangular_matrix(b, size, blocksize);
    init_block_matrix(c, size, blocksize);

    random_fill_upper_triangular_matrix(a);
    random_fill_upper_triangular_matrix(b);

    mult_symmetric_upper_triangular_matrix(a, b, c);

#ifdef PRINT_MATRICES
    print_symmetric_matrix(a);
    printf("\n---------------------------------------------------\n");
    print_upper_triangular_matrix(b);
    printf("\n---------------------------------------------------\n");
    print_block_matrix(c);
    printf("\n---------------------------------------------------\n");
#endif

    matrix_block Suba;
    matrix_block Subb;

    block_matrix bm;

    int bsize = convert_upper_triangular_to_block(b, Subb);
    make_symmetric(a, bm);
    convert_block_matrix_to_block(bm, Suba);

    matrix_block B;
    init_matrix_block(B, bsize);
    mult_add_matrix_block(Suba, Subb, B, bsize);

#ifdef PRINT_MATRICES
    print_matrix_block(B, bsize);
#endif

    printf("Correct: %s\n ", is_equal(c, B) ? "Yes" : "No");

    free_upper_triangular_matrix(a);
    free_upper_triangular_matrix(b);
    free_block_matrix(c);
    free_matrix_block(B);
    free_matrix_block(Suba);
    free_matrix_block(Subb);
}

void timetest(int appsize, int blocksize)
{
    upper_triangular_matrix a;
    upper_triangular_matrix b;
    block_matrix c;

    int size = appsize / blocksize;
    init_upper_triangular_matrix(a, size, blocksize);
    init_upper_triangular_matrix(b, size, blocksize);
    init_block_matrix(c, size, blocksize);

    random_fill_upper_triangular_matrix(a);
    random_fill_upper_triangular_matrix(b);

    double start = omp_get_wtime();
    mult_symmetric_upper_triangular_matrix(a, b, c);
    double time = omp_get_wtime() - start;

    printf("%d %f \n", blocksize, time);

    free_upper_triangular_matrix(a);
    free_upper_triangular_matrix(b);
    free_block_matrix(c);
}

int main() {
    srand(122);
    timetest(2048, 256);

    correctness(8, 2);

    return 0;

}
