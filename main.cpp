#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;

struct matrix_block
{
	int* value;
};
struct block_matrix_row
{
	matrix_block* blocks;
	int row_length;
};
struct block_matrix_col
{
	matrix_block* blocks;
	int col_length;
};
struct block_matrix_row_made
{
	block_matrix_row* row;
	int col_length;
	int blocksize;
};
struct block_matrix_col_made
{
	block_matrix_col* col;
	int row_length;
	int blocksize;
};
void Matrix_Multiplication_Parallel_Static(block_matrix_col_made& A, block_matrix_row_made& B,
										block_matrix_row_made& C);
void Matrix_Multiplication_Parallel_Dynamic(block_matrix_col_made& A, block_matrix_row_made& B,
										 block_matrix_row_made& C);
void Matrix_Multiplication_Non_Paralell(block_matrix_col_made& A, block_matrix_row_made& B,
									 block_matrix_row_made& C);
void Block_Multiplication_Parallel_Static(int *A_block, int *B_block, int *C_block, int blocksize);
void Block_Multiplication_Parallel_Dynamic(int *A_block, int *B_block, int *C_block, int blocksize);

int main() {
	setlocale(LC_ALL, "Rus");

	int matrix_size = 1024;
	cout << "Matrix size = " << matrix_size << "x" << matrix_size << endl;
	int block_size = 2;

	while (block_size < matrix_size) {
		cout << "Block's size = " << block_size << "x" << block_size << endl;
		int size = matrix_size / block_size;

		block_matrix_col_made a;
		a.row_length = size;
		a.blocksize = block_size;
		a.col = new block_matrix_col[size];
		for (int i = 0; i < size; i++)
		{
			a.col[i].blocks = new matrix_block[size];
			a.col[i].col_length = size;
			for (int j = 0; j < size; j++)
			{
				a.col[i].blocks[j].value = new int[block_size * block_size];
				for (int k = 0; k < block_size * block_size; ++k)
					a.col[i].blocks[j].value[k] = rand() % 10 + 1;
			}
		}

		block_matrix_row_made b;
		b.col_length = size;
		b.blocksize = block_size;
		b.row = new block_matrix_row[size];
		for (int i = 0; i < size; i++)
		{
			b.row[i].blocks = new matrix_block[size - i];
			b.row[i].row_length = size - i;
			for (int j = 0; j < size - i; j++)
			{
				b.row[i].blocks[j].value = new int[block_size * block_size];
				for (int k = 0; k < block_size * block_size; ++k)
					b.row[i].blocks[j].value[k] = rand() % 10 + 1;
			}
		}
		for (int i = 0; i < b.col_length; i++)
			for (int j = 0; j < size - i; j++)
				for (int k = 0; k < b.blocksize; k++)
					for (int l = 0; l < b.blocksize; l++)
						if (k>l)
							b.row[i].blocks[0].value[k*b.blocksize + l] = 0;

		block_matrix_row_made c1;
		c1.col_length = size;
		c1.blocksize = block_size;
		c1.row = new block_matrix_row[size];
		for (int i = 0; i < size; i++)
		{
			c1.row[i].blocks = new matrix_block[size];
			c1.row[i].row_length = size;
			for (int j = 0; j < size; j++)
			{
				c1.row[i].blocks[j].value = new int[block_size * block_size];
				for (int k = 0; k < block_size * block_size; ++k)
					c1.row[i].blocks[j].value[k] = 0;
			}

		}
		block_matrix_row_made c2;
		c2.col_length = size;
		c2.blocksize = block_size;
		c2.row = new block_matrix_row[size];
		for (int i = 0; i < size; i++)
		{
			c2.row[i].blocks = new matrix_block[size];
			c2.row[i].row_length = size;
			for (int j = 0; j < size; j++)
			{
				c2.row[i].blocks[j].value = new int[block_size * block_size];
				for (int k = 0; k < block_size * block_size; ++k)
					c2.row[i].blocks[j].value[k] = 0;
			}

		}
		block_matrix_row_made c3;
		c3.col_length = size;
		c3.blocksize = block_size;
		c3.row = new block_matrix_row[size];
		for (int i = 0; i < size; i++)
		{
			c3.row[i].blocks = new matrix_block[size];
			c3.row[i].row_length = size;
			for (int j = 0; j < size; j++)
			{
				c3.row[i].blocks[j].value = new int[block_size * block_size];
				for (int k = 0; k < block_size * block_size; ++k)
					c3.row[i].blocks[j].value[k] = 0;
			}

		}

		double start_time, end_time;

		start_time = clock();
		Matrix_Multiplication_Parallel_Static(a, b, c1);
		end_time = clock();
		cout << "Parallel Static time = " << (end_time - start_time) / 1000. << endl;

		start_time = clock();
		Matrix_Multiplication_Parallel_Dynamic(a, b, c2);
		end_time = clock();
		cout << "Parallel Dynamic time = " << (end_time - start_time) / 1000. << endl;

		start_time = clock();
		Matrix_Multiplication_Non_Paralell(a, b, c3);
		end_time = clock();
		cout << "NonParallel time = " << (end_time - start_time) / 1000. << endl;

		/*for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				for (int k = 0; k < block_size * block_size; ++k)
					cout << a.col[j].blocks[i].value[k] << " ";
				cout << "|";
			}
			cout << endl;
		}
		cout << endl;
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size - i; j++)
			{
				for (int k = 0; k < block_size * block_size; ++k)
					cout << b.row[i].blocks[j].value[k] << " ";
				cout << "|";
			}
			cout << endl;
		}
		cout << endl;
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				for (int k = 0; k < block_size * block_size; ++k)
					cout << c1.row[i].blocks[j].value[k] << " ";
				cout << "|";
			}
			cout << endl;
		}
		cout << endl;
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				for (int k = 0; k < block_size * block_size; ++k)
					cout << c2.row[i].blocks[j].value[k] << " ";
				cout << "|";
			}
			cout << endl;
		}
		cout << endl;
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				for (int k = 0; k < block_size * block_size; ++k)
					cout << c3.row[i].blocks[j].value[k] << " ";
				cout << "|";
			}
			cout << endl;
		}
		cout << endl;*/

		block_size *= 2;
	}
	system("pause");
	return 0;
}

void Matrix_Multiplication_Parallel_Static(block_matrix_col_made& A, block_matrix_row_made& B,
										block_matrix_row_made& C)
{
#pragma omp parallel for schedule(static, C.col_length/4) collapse(2) shared(A, B, C) num_threads(4)
	for (int ik = 0; ik < C.col_length; ik++)
		for (int jk = 0; jk < C.col_length; jk++)
			for (int k = 0; k < C.col_length; k++)
			{
				if (jk - C.row[k].row_length + B.row[k].row_length >= 0)
				{
					int nn = jk - C.row[k].row_length + B.row[k].row_length;
					Block_Multiplication_Parallel_Static(A.col[k].blocks[ik].value,
						B.row[k].blocks[nn].value,
						C.row[ik].blocks[jk].value,
						B.blocksize);
				}
			}
}

void Matrix_Multiplication_Parallel_Dynamic(block_matrix_col_made& A, block_matrix_row_made& B,
	block_matrix_row_made& C)
{
#pragma omp parallel for schedule(dynamic, C.col_length/4) collapse(2) shared(A, B, C) num_threads(4)
	for (int ik = 0; ik < C.col_length; ik++)
		for (int jk = 0; jk < C.col_length; jk++)
			for (int k = 0; k < C.col_length; k++)
			{
				if (jk - C.row[k].row_length + B.row[k].row_length >= 0)
				{
					int nn = jk - C.row[k].row_length + B.row[k].row_length;
					Block_Multiplication_Parallel_Dynamic(A.col[k].blocks[ik].value,
						B.row[k].blocks[nn].value,
						C.row[ik].blocks[jk].value,
						B.blocksize);
				}
			}
}

void Matrix_Multiplication_Non_Paralell(block_matrix_col_made& A, block_matrix_row_made& B,
									 block_matrix_row_made& C)
{
	for (int ik = 0; ik < C.col_length; ik++)
		for (int jk = 0; jk < C.col_length; jk++)
			for (int kk = 0; kk < C.col_length; kk++)
				if (jk - C.row[kk].row_length + B.row[kk].row_length >= 0)
				{
					int nn = jk - C.row[kk].row_length + B.row[kk].row_length;
					for (int i = 0; i < C.blocksize; i++)
						for (int j = 0; j < C.blocksize; j++)
							for (int k = 0; k < C.blocksize; k++)
								C.row[ik].blocks[jk].value[i*C.blocksize + j] +=
								A.col[kk].blocks[ik].value[i*C.blocksize + k] *
								B.row[kk].blocks[nn].value[k*C.blocksize + j];
				}
}

void Block_Multiplication_Parallel_Static(int *A_block, int *B_block, int *C_block, int blocksize)
{
#pragma omp parallel for schedule(static, blocksize/4) collapse(2) shared(A_block, B_block, C_block, blocksize) num_threads(4)
	for (int i = 0; i < blocksize; i++)
		for (int j = 0; j < blocksize; j++)
			for (int k = 0; k < blocksize; k++)
				C_block[i*blocksize + j] += A_block[i*blocksize + k] * B_block[k*blocksize + j];
}

void Block_Multiplication_Parallel_Dynamic(int *A_block, int *B_block, int *C_block, int blocksize)
{
#pragma omp parallel for schedule(dynamic, blocksize/4) collapse(2) shared(A_block, B_block, C_block, blocksize) num_threads(4)
	for (int i = 0; i < blocksize; i++)
		for (int j = 0; j < blocksize; j++)
			for (int k = 0; k < blocksize; k++)
				C_block[i*blocksize + j] += A_block[i*blocksize + k] * B_block[k*blocksize + j];
}
