#include <stdio.h>
#include <stdlib.h>
#include <time.h> 

const int n = 2048;
const int b = 512;

double randi(double b, double e)
{
    return b + (rand() % (int)((e - b) * 1000)) / 1000.;
}

void permatrix(double *A, double *B, double *C, int indA, int indB, int indC)
  {
    int li = indB;
    int i = 1;
    int j, l = 1;
    double kk;
    for(l; l <= b; ++l)
    {
    	i = 1;
        for(i; i <= b; ++i)
        {
            j = 1;
            kk = 0;
            for(j; j <= b; ++j)
            {
                kk = kk + A[indA]*B[indB];
                indB = indB+b;
                indA++;
            }
            C[indC++] += kk;
            kk = 0;
            j = 1;
            indA -= b;
            indB -= b + (b - 1)*b - 1;
        }
        indA += b;
        indB = li;
    }
}

int main()
{
    srand((time(NULL)));
    time_t start, end;
    /**Static arrays initiate **/
    double *A = new double[n * (n + b) / 2]; //double A[n*n];
    double *B = new double[n * n];           //double B[n*n/2/b/b + n/2/b];
    double *C = new double[n * n];           //double C[n*n];
    /**Filling linear array B with rectangular matrix blocks in strings**/
    FILE *f1 = fopen("file1.txt", "r");
    int t = 0, bli = 1, blj = 1, k = 0, fi = 1;
    int arr_z = 0;
    while (!feof(f1))
    {
        double m;

        if (feof(f1))
            break;
        fscanf(f1, "%lf", &m);
        A[arr_z] = m;
        //printf("A[%d] = %2.2f ", arr_z, A[arr_z]);
        k++;
        arr_z++;
        if ((k - t) == b)
        {
            if (blj == bli)
            {
                int skip = 0;
                for (skip; skip < b * (n / b - bli); skip++)
                {
                    fscanf(f1, "%lf", &m);
                    k++;
                }
                if (bli > 1)
                    arr_z -= (b * b) * (blj - 1);
                if ((k % (bli * b * n)) == 0)
                {
                    arr_z = k / (n / b - bli + 1);
                    bli++;
                }
                blj = 1;
            }
            else
            {
                arr_z += b * b - b;
                blj++;
            }
            t = k;
        }
    }
    fclose(f1);
    /**Filling linear array A with down-triangle matrix blocks in columns**/
    FILE *f2 = fopen("file2.txt", "r");
    k = 0;
    bli = 0;
    blj = 0;
    fi = 0;
    t = 0;
    while (!feof(f2))
    {
        double d;
        fscanf(f2, "%lf", &d);
        if (feof(f2))
            break;
        B[k] = d;
        //printf("B[%d] = %2.2f ", k, B[k]);
        k++;
        if ((k - t) == b)
            if (blj < (n/b - 1))
            {
                blj++;
                k = fi*b + blj*n*b;
                t = k;
            }
            else
            {
                fi++;
                k = fi*b;
                if (fi % b == 0)
                    bli++;
                blj = 0;
                t = k;
        }

    }
    fclose(f2);
    /**Matrices multiplication C = A*B**/
    FILE *f3 = fopen("file3.txt", "w");
    int indA = 0,indB = 0,indC = 0;
    int r, x, y, z;
    r = 1;
    start = clock();
    for(r; r <= n/b; r++)
    {
        x = 1;
        for(x; x <= n/b; x++)
        {
            y = 1;
            for(y; y <= r; y++)
            {
                permatrix(A, B, C, indA, indB, indC);
                indB += b*b;
                indA += b*b;
            }
            indC += b*b;
            indB += (n/b - r)*b*b;
            indA -= r*b*b;
        }
        indB = 0;
        indA += r*b*b;
    }
    end = clock();
    printf("%d", (end - start) / (CLOCKS_PER_SEC));
    /**Printing out array C to file3**/
    indC = 0; r = 1; fi = b; int sch = 1;
    for(int i=1; i <= n; i++)
    {
        for(int j=1; j <= n/b; j++)
        {
            for(int z=1; z <= b; z++)
            {
                fprintf(f3, "%2.2f ", C[indC]);
                indC++;
            }
            indC += (b - 1)*b;
        }
        if(r < b)
        {
            r++;
            indC = fi;
            fi += b;
        }
        else
        {
            r = 1;
            fi += (n/b - 1)*b*b + b;
            indC -= (b - 1)*b;
        }
        fprintf(f3, "\n");
    }
    /**Checking scope**/
    /*int K = 0;
    while (K < n*n)
    {
        printf("%2.2f ", C[K]);
        K++;
    } */

    fclose(f3);
    return 0;
}
