// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

struct matrix_block { int* value; };
struct matrix_colomn { matrix_block* blocks; };
//поблочное хранение в одномерном массиве
struct trmat
{
	matrix_colomn* colomn;
	int size;
	int blocksize;
};
typedef struct trmat block_matrix;
typedef struct trmat symmetric_matrix;

void multParallel(trmat& left, trmat& right, block_matrix& result);
void multParallelBlock(trmat& left, trmat& right, block_matrix& result);
void multTwoBlockParallel(int *A, int *B, int *C, int n);
void multNonParallel(trmat& left, trmat& right, block_matrix& result);
void multNonParallelBlock(trmat& left, trmat& right, block_matrix& result);
void multTwoBlockNonParallel(int *A, int *B, int *C, int n);
void goProgram();

// TODO: reference additional headers your program requires here
