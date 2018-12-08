#include "pch.h"
#include <iostream>
#include <ctime>
#include "TimeQuantMS.h"
using namespace std;

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

	matrix_block Suba;
	matrix_block Subb;

	block_matrix bm;

	int bsize = convert_upper_triangular_to_block(b, Subb);
	make_symmetric(a, bm);
	convert_block_matrix_to_block(bm, Suba);

	matrix_block B;
	init_matrix_block(B, bsize);
	mult_add_matrix_block(Suba, Subb, B, bsize);

	//print_matrix_block(B, bsize);

	cout << "Correct: " << (is_equal(c, B) ? "Yes" : "No") << endl;

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

	auto elapsed = QuantMS();
	mult_symmetric_upper_triangular_matrix(a, b, c);
	elapsed = QuantMS();
	cout << " Время = " << elapsed / 1000000.0 - 6.05 << " Sec" << endl;

	cout << "Размер блока = " << blocksize << endl;

	free_upper_triangular_matrix(a);
	free_upper_triangular_matrix(b);
	free_block_matrix(c);
	///////////////////////////////////////////////////////////////////////
	init_upper_triangular_matrix(a, size, blocksize);
	init_upper_triangular_matrix(b, size, blocksize);
	init_block_matrix(c, size, blocksize);

	random_fill_upper_triangular_matrix(a);
	random_fill_upper_triangular_matrix(b);

	elapsed = QuantMS();
	mult_symmetric_upper_triangular_matrix_parallel(a, b, c);
	elapsed = QuantMS();
	cout << " Время = " << elapsed / 1000000.0 - 6.05 << " Sec" << endl;

	cout << "Размер блока = " << blocksize << endl;

	free_upper_triangular_matrix(a);
	free_upper_triangular_matrix(b);
	free_block_matrix(c);
}