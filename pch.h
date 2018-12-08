// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

#ifndef PCH_H
#define PCH_H

struct matrix_block
{
	double* value;
};

struct matrix_row
{
	matrix_block* blocks;
};

struct upper_triangular_matrix
{
	matrix_row* rows;
	int size;
	int blocksize;
};

typedef struct upper_triangular_matrix block_matrix;
typedef struct upper_triangular_matrix symmetric_matrix;

/* Low level */
void transpose(double *A, double *B, int n);
void mult_A_B(double *A, double *B, double *C, int n);
void mult_A_B_openmp(double *A, double *B, double *C, int n);

/* Block level */
void init_matrix_block(matrix_block& block, int blocksize);
void free_matrix_block(matrix_block& block);
void fill_random_matrix_block(matrix_block& block, int blocksize);
void print_matrix_block(matrix_block& block, int blocksize);
void mult_add_matrix_block(matrix_block& left, matrix_block& right,
	matrix_block& result, int blocksize);
void mult_add_matrix_block_parallel(matrix_block& left, matrix_block& right,
	matrix_block& result, int blocksize);
void mult_add_matrix_block_transpose(matrix_block& left, matrix_block& right,
	matrix_block& result, int blocksize);
void mult_add_matrix_block_transpose_parallel(matrix_block& left, matrix_block& right,
	matrix_block& result, int blocksize);
void mirror(matrix_block& block, int blocksize);
void unmirror(matrix_block& block, int blocksize);
void block_transpose(matrix_block& block, matrix_block& result, int blocksize);

/* Row level */
void init_matrix_row(matrix_row& row, int rowsize, int blocksize);
void free_matrix_row(matrix_row& row, int rowsize);
void print_matrix_row(matrix_row& row, int rowsize, int blocksize, int empty);

/* Matrix level */
void init_upper_triangular_matrix(upper_triangular_matrix& matrix, int size, int blocksize);
void free_upper_triangular_matrix(upper_triangular_matrix& matrix);
void random_fill_upper_triangular_matrix(upper_triangular_matrix& matrix);
void print_upper_triangular_matrix(upper_triangular_matrix& matrix);

void init_block_matrix(block_matrix& matrix, int size, int blocksize);
void free_block_matrix(block_matrix& matrix);
void print_block_matrix(block_matrix& matrix);
void print_symmetric_matrix(upper_triangular_matrix& matrix);

/* The first one is symmetric, the second is upper-triangular */
void mult_symmetric_upper_triangular_matrix(upper_triangular_matrix& left,
	upper_triangular_matrix& right, upper_triangular_matrix& result);
void mult_symmetric_upper_triangular_matrix_parallel(upper_triangular_matrix& left,
	upper_triangular_matrix& right, block_matrix& result);

/* "Converters". Don't init newm! */
int convert_upper_triangular_to_block(upper_triangular_matrix& matrix,
	matrix_block& newm);
void convert_upper_triangular_to_block_matrix(upper_triangular_matrix& matrix,
	block_matrix& newm);
void convert_block_matrix_to_block(block_matrix& matrix, matrix_block& block);
void make_symmetric(upper_triangular_matrix& matrix, block_matrix& newm);

int is_empty_block(matrix_block& block, int blocksize);
int is_equal(block_matrix& matrix, matrix_block& block);

void difference(matrix_block& left, matrix_block& right,
	matrix_block& result, int blocksize);

void correctness(int appsize, int blocksize);
void timetest(int appsize, int blocksize);

#endif //PCH_H
