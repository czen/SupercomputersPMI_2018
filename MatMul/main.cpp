#include <iostream>
#include <vector>
#include <fstream>
#include <omp.h>

using namespace std;

void mult(vector<double> &A, vector<double> &B, vector<double> &C, int b, int ai, int bi, int ci, bool t)
{
    int js = t ? 1 : b,
        ks = t ? b : 1;
#pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < b; ++i)
        for (int j = 0, jj = 0; j < b; ++j, jj += js)
            for (int k = 0, kk = 0; k < b; k++, kk += ks)
                C[ci + i * b + j] += A[ai + i * b + k] * B[bi + jj + kk];
}

void block_multi(vector<double> &A, vector<double> &B, vector<double> &C, int n, int b)
{
#pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < n / b; ++i)
        for (int j = 0; j < n / b; ++j)
        {
            int ai = i * b * b * (i + 1) / 2;
            int bi = j * b * b * (j + 1) / 2;
            int ci = i * b * n + j * b * b;
            for (int k = 0; k <= j && k <= i; ++k)
            {
                mult(A, B, C, b, ai, bi, ci, false);
                bi += b * b;
                ai += b * b;
            }

            bi = (j + 1) * b * b * (j + 2) / 2 + j * b * b;
            for (int k = j + 1; k <= i; ++k)
            {
                mult(A, B, C, b, ai, bi, ci, true);
                bi += (k + 1) * b * b;
                ai += b * b;
            }
        }
}

void read_A(vector<double> &vec, int n, int b, string st)
{
    ifstream in(st);
    vec.resize(n * (n + b) / 2);
    for (int i = 0; i < n / b; ++i)
        for (int bi = 0; bi < b; ++bi)
        {
            for (int j = 0; j <= i; ++j)
                for (int bj = 0; bj < b; ++bj)
                    in >> vec[i * b * b * (i + 1) / 2 + j * b * b + bi * b + bj];

            for (int bj = 0; bj < n - b * (i + 1); ++bj)
                in >> vec.back();
        }
    in.close();
}

void read_B(vector<double> &vec, int n, int b, string st)
{
    ifstream in(st);
    vec.resize(n * (n + b) / 2);
    for (int i = 0; i < n / b; ++i)
        for (int bi = 0; bi < b; ++bi)
        {
            for (int bj = 0; bj < b * i; ++bj)
                in >> vec.back();

            for (int j = 0; j < n / b - i; ++j)
                for (int bj = 0; bj < b; ++bj)
                    in >> vec[(i + j) * b * b * (i + j + 1) / 2 + i * b * b + b * bj + bi];
        }

    in.close();
}

void write_C(vector<double> &vec, int n, int b, string st)
{
    ofstream out(st);
    vec.resize(n * (n + b) / 2);
    for (int i = 0; i < n / b; ++i)
        for (int bi = 0; bi < b; ++bi)
        {
            for (int j = 0; j < n / b; ++j)

                for (int bj = 0; bj < b; ++bj)
                    out << vec[i * b * n + bi * b + j * b * b + bj] << " ";
            out << endl;
        }
    out.close();
}

int main(int argc, char const *argv[])
{
    int n = 1024, b = 64;
    vector<double> A;
    vector<double> B;

    read_A(A, n, b, "A1024.txt");
    read_B(B, n, b, "B1024.txt");

    // for (int i = 0, k = 0; i < n / b; ++i)
    // {
    //     for (int g = 0; g < i + 1; ++g)
    //     {
    //         for (int j = 0; j < b * b; ++j)
    //             cout << A[k++] << " ";

    //         cout << "| ";
    //     }
    //     cout << endl;
    // }
    // cout << endl;

    // for (int i = 0, k = 0; i < n / b; ++i)
    // {
    //     for (int g = 0; g < i + 1; ++g)
    //     {
    //         for (int j = 0; j < b * b; ++j)
    //             cout << B[k++] << " ";

    //         cout << "| ";
    //     }
    //     cout << endl;
    // }
    // cout << endl;
    vector<double> C(n * n);
    block_multi(A, B, C, n, b);

    // for (int i = 0, k = 0; i < n / b; ++i)
    // {
    //     for (int g = 0; g < n / b; ++g)
    //     {
    //         for (int j = 0; j < b * b; ++j)
    //             cout << C[k++] << " ";

    //         cout << "| ";
    //     }
    //     cout << endl;
    // }
    // cout << endl;
    write_C(C, n, b, "c.txt");

    return 0;
}