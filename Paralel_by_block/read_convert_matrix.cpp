//
//  read_convert_matrix.cpp
//  Individual_task
//
//  Created by Artem on 20/12/2018.
//  Copyright © 2018 Artem. All rights reserved.
//

#include "main_header.h"

int** read_matrix(char *fileName, int size){
    int **result = NULL;
    result = (int **)calloc(size, sizeof(int*));
    for(int i = 0; i < size; ++i){
        result[i] = (int *)calloc(size, sizeof(int));
    }
    FILE *fin;
    fin = fopen(fileName, "r");
    for(int i = 0; i < size; ++i){
        for(int j = 0; j < size; ++j)
            fscanf(fin, "%d ", &result[i][j]);
    }
    fclose(fin);
    return result;
}

int **convert_up_treungle(convert_inp inp){ // for matrix B
    int sz_sz = inp.block_size * inp.block_size;
    int block_cnt = inp.size/inp.block_size;
    int tmp_last_bl = 0, bl_size = inp.block_size;
    int **result = (int **)calloc(block_cnt*(block_cnt + 1)/2, sizeof(int *));
    for(int i = 0; i < block_cnt*(block_cnt + 1)/2; ++i){
        result[i] = (int *)calloc(sz_sz, sizeof(int));
    }
    for( int i = 0; i < block_cnt; ++i){
        for( int j = i; j < block_cnt; ++j){
            for( int bl_i = 0; bl_i < bl_size; ++bl_i)
                for( int bl_j = 0; bl_j < bl_size; ++bl_j){
                    result[tmp_last_bl][bl_i*bl_size + bl_j] = inp.matrix[i*bl_size + bl_i][j*bl_size + bl_j];
                }
            ++tmp_last_bl;
        }
    }
    return result;
}


int **convert_down_treungle(convert_inp inp){ // for matrix A
    unsigned int sz_sz = inp.block_size * inp.block_size;
    unsigned int block_cnt = inp.size/inp.block_size;
    unsigned int tmp_last_bl = 0, bl_size = inp.block_size;
    int **result = (int **)calloc(block_cnt*(block_cnt + 1)/2, sizeof(int *));
    for(int i = 0; i < block_cnt*(block_cnt + 1)/2; ++i){
        result[i] = (int *)calloc(sz_sz, sizeof(int));
    }
    for(unsigned int j = 0; j < block_cnt; ++j){
        for(unsigned int i = j; i < block_cnt; ++i){
            for(unsigned int bl_i = 0; bl_i < bl_size; ++bl_i)
                for(unsigned int bl_j = 0; bl_j < bl_size; ++bl_j){
                    result[tmp_last_bl][bl_i*bl_size + bl_j] = inp.matrix[i*bl_size + bl_i][j*bl_size + bl_j];
                }
            ++tmp_last_bl;
        }
    }
    return result;
}

int adress(adr_conv inp){ // for A. change i and j to use it for B
    unsigned int result;
    result = inp.in_j * inp.bl_cnt - (inp.in_j - 1)*inp.in_j/2; // +- 1?
    result += inp.in_i - inp.in_j;
    return result;
}
