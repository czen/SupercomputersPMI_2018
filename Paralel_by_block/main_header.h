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
    int size;
    int block_size;
} convert_inp;

typedef struct{
    int in_i;
    int in_j;
    int bl_cnt;
    int bl_size;
} adr_conv;

typedef struct{
    int *matrA;
    int *matrB;
    int bl_size;
    int *martC;
} bl_mult;

typedef struct{
    int **matrA;
    int **matrB;
    int **martC;
    int bl_size;
    int bl_cnt;
    
} matrix_mult;

int **read_matrix(char *fileName, int size);
int **convert_up_treungle(convert_inp inp);
int **convert_down_treungle(convert_inp inp);

int adress(adr_conv inp);

void matr_mult(matrix_mult inp);

#endif /* main_header_h */
