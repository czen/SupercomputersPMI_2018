#pragma once

#ifndef MATR
#define MATR

#define SIZE 2880
#define BLOCK_SIZE 144
#define LENGTH (SIZE + BLOCK_SIZE)*SIZE/2

#define FULL_BLOCK BLOCK_SIZE * BLOCK_SIZE
#define FULL_LINE  BLOCK_SIZE * SIZE

#endif