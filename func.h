#pragma once

int A(int i, int j, int p);
int B(int i, int j, int p, int n);


void create_A_array(int *input, int *a, int p, int n);
void create_B_array(int *input, int b[], int p, int n);

void create_A_to_text(const char *fname, int n);
void create_B_to_text(const char *fname, int n);

void read_from_text(const char *fname, int *&matr, int &n);


void block_mul(int *a, int*b, int*c, int n, int p);
void block_mul_parallel(int *a, int*b, int*c, int n, int p);
void block_mul(int *a, int*b, int*c, int i, int j, int n, int p);
void block_mul_parallel(int *a, int*b, int*c, int i, int j, int n, int p);

void mul(int* ab, int *bb, int*cb, int p);
void add(int* ab, int *bb, int*cb, int p);

int * block_A(int *a, int i, int j, int n, int p);
int * block_B(int *b, int i, int j, int n, int p);

void out_matr(const char *fname, int n, int *matr, int p);
