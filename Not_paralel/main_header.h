//
//  main_header.h
//  Individual_task
//
//  Created by Artem on 20/12/2018.
//  Copyright © 2018 Artem. All rights reserved.
//

#ifndef main_header_h

#define main_header_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

typedef struct{
    int **matrix;
    unsigned int size;
    unsigned int block_size;
} convert_inp;

typedef struct{
    unsigned int in_i;
    unsigned int in_j;
    unsigned int bl_cnt;
    unsigned int bl_size;
} adr_conv;

typedef struct{
    int *matrA;
    int *matrB;
    unsigned int adr_A;
    unsigned int adr_B;
    unsigned int bl_size;
    int *martC;
    unsigned int adr_C;
} bl_mult;

typedef struct{
    int *matrA;
    int *matrB;
    int *martC;
    unsigned int bl_size;
    unsigned int bl_cnt;
    
} matrix_mult;

int **read_matrix(char *fileName, unsigned int size);
int *convert_up_treungle(convert_inp inp);
int *convert_down_treungle(convert_inp inp);

unsigned int adress(adr_conv inp);

void matr_mult(matrix_mult inp);

#endif /* main_header_h */
