//
//  adreses.cpp
//  Individual_task
//
//  Created by Artem on 21/12/2018.
//  Copyright © 2018 Artem. All rights reserved.
//

#include "main_header.h"

void block_mult(bl_mult inp){

    #pragma omp parallel num_threads(2)
    {
    #pragma omp for schedule (static)
    for(int i = 0; i < inp.bl_size; ++i)
        for(int j = 0; j < inp.bl_size; ++j)
            for(int k = 0; k < inp.bl_size; ++k){
                int bl_sz = inp.bl_size;
                int *A, *B, *C;
                A = inp.matrA;
                B = inp.matrB;
                C = inp.martC;
                int adA, adB, adC;
                adA = inp.adr_A;
                adB = inp.adr_B;
                adC = inp.adr_C;
                C[adC + i*bl_sz + j] += A[adA + i*bl_sz + k]*B[adB + k*bl_sz + j];
            }
    }
}

void matr_mult(matrix_mult inp){
    int *A = inp.matrA, *B = inp.matrB, *C = inp.martC;
    bl_mult blok_mult_in;
    blok_mult_in.martC = inp.martC;
    blok_mult_in.matrA = inp.matrA;
    blok_mult_in.matrB = inp.matrB;
    blok_mult_in.bl_size = inp.bl_size;
    
    adr_conv adr_convA, adr_convB;
    adr_convA.bl_cnt = inp.bl_cnt;
    adr_convA.bl_size = inp.bl_size;
    adr_convB = adr_convA;
    
    int adrA, adrB, adrC;
    for(int i = 0; i < inp.bl_cnt; ++i){
        for (int j = 0; j < inp.bl_cnt; ++j)
            for(int k = 0; k <= j; ++k){
                if (k > i){  // в силу симметрии
                    adr_convA.in_i = i;
                    adr_convA.in_j = k;
                }
                else{
                    adr_convA.in_i = k;
                    adr_convA.in_j = i;
                }
                adr_convB.in_i = j; // Изиенены в силу особенностетй функции адресации
                adr_convB.in_j = k; // (индексы i и j поменяны местами)
                
                adrA = adress(adr_convA);
                adrB = adress(adr_convB);
                adrC = (i * inp.bl_cnt + j)*inp.bl_size*inp.bl_size;
                blok_mult_in.adr_A = adrA;
                blok_mult_in.adr_B = adrB;
                blok_mult_in.adr_C = adrC;
                block_mult(blok_mult_in);
            }
    }
}
