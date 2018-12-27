#include <vector>
#include <fstream>
#include <iostream>
#include <omp.h>

using namespace std;

void blockMult(vector<double> &A, vector<double> &B, vector<double> &C, int ai, int bi, int ci, int b)
{
   //#pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < b; ++i)
        for (int j = 0; j < b; ++j)
            for (int k = 0; k < b; k++)
                C[ci + i * b + j] += A[ai + i * b + k] * B[bi + j * b + k];
}

void matrMult(vector<double> &A, vector<double> &B, vector<double> &C, int n, int b)
{
    int indst = 0;
    for (int i = 0; i < n / b; ++i)
    {
        for (int j = 0; j < n / b; ++j)
        {
            int q = i > j ? i : j;
            int ai = indst + (q - i) * b * b;
            int bi = q * b * b * (q + 1) / 2 + j * b * b;
            int ci = i * b * n + j * b * b;

 //#pragma omp parallel for schedule(dynamic)
            for (int k = q; k < n / b; ++k)
            {
                int aai = ai + (k - q) * b * b;
                int bbi = bi;
                if (k != q)
                    bbi += (2 * (q + 1) * b * b + (n / b - k - 1)) * (n / b - k) / 2;
                blockMult(A, B, C, aai, bbi, ci, b);
                // bi += (k + 1) * b * b;
                // ai += b * b;
            }
        }
        indst += (n / b - i) * b * b;
    }
}

void loadA(vector<double> &mtr, int n, int b, string st)
{
    ifstream in(st);
    mtr.resize(n * (n + b) / 2);
    double buf;
    for (int i = 0, indst = 0; i < n / b; ++i)
    {
        for (int ii = 0; ii < b; ++ii)
        {
            for (int jj = 0; jj < b * i; ++jj)
                in >> buf;

            for (int j = 0; j < n / b - i; ++j)
                for (int jj = 0; jj < b; ++jj)
                    in >> mtr[indst + j * b * b + ii * b + jj];
        }
        indst += (n / b - i) * b * b;
    }
    in.close();
}

void loadB(vector<double> &mtr, int n, int b, string st)
{
    ifstream in(st);
    mtr.resize(n * (n + b) / 2);
    double buf;
    for (int i = 0; i < n / b; ++i)
    {
        int indi = i * b * b * (i + 1) / 2;
        for (int ii = 0; ii < b; ++ii)
        {
            for (int j = 0; j <= i; ++j)
                for (int jj = 0; jj < b; ++jj)
                    in >> mtr[indi + j * b * b + jj * b + ii];

            for (int j = 0; j < n - b * (i + 1); ++j)
                in >> buf;
        }
    }
    in.close();
}

void save(vector<double> &mtr, int n, int b, string st)
{
    ofstream out(st);
    mtr.resize(n * (n + b) / 2);
    for (int i = 0; i < n / b; ++i)
        for (int ii = 0; ii < b; ++ii)
        {
            for (int j = 0; j < n / b; ++j)
                for (int jj = 0; jj < b; ++jj)
                    out << mtr[i * b * n + ii * b + j * b * b + jj] << " ";
            out << endl;
        }
    out.close();
}

int main(int argc, char const *argv[])
{
    int n = 1024, b = 4;
    vector<double> A;
    vector<double> B;

    loadA(A, n, b, "bigA.txt");
    loadB(B, n, b, "bigB.txt");

    // for (int i = n / b, k = 0; i > 0; --i)
    // {
    //     for (int g = 0; g < i; ++g)
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
    auto start = omp_get_wtime();
    matrMult(A, B, C, n, b);
    cout << omp_get_wtime() - start << endl;
    save(C, n, b, "c.txt");

    return 0;
}