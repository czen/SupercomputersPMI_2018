#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const int r = 2048;

double randi(double b, double e)
{
    return b + (rand() % (int)((e - b) * 1000)) / 1000.;
}

//printf("%2.2f ", a[(i - 1)*r + j]);
//printf("\n");
int main()
{
    srand((time(NULL)));
    FILE *f1 = fopen("file1.txt", "w");
    FILE *f2 = fopen("file2.txt", "w");
    int i = 1, j = 1;
    for (i; i <= r; ++i)
    {
    j = 1;
    for (j; j <= r; ++j)
        {
        fprintf(f2, "%.2f ", randi(-10,10));
        if (i < j)
            fprintf(f1, "%.2f ", 0.0);
        else
            fprintf(f1, "%.2f ", randi(-10,10));
        }
        fprintf(f1,"\n");
        fprintf(f2,"\n");
    }
    fclose(f1);
    fclose(f2);
    return 0;
}
