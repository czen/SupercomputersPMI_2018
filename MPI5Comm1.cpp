#include <iostream>
#include "pt4.h"
#include "mpi.h"

using namespace std;

void main()
{
    int f, r, s;
    MPI_Initialized(&f);
    MPI_Comm_size(MPI_COMM_WORLD, &s);
    MPI_Comm_rank(MPI_COMM_WORLD, &r);
    MPI_Group p, M_G;
    MPI_Comm M_C;
    int r_counts=(s+1)/2;
    int* r2=new int[r_counts];
    for (int i=0; i<r_counts; i++)
    {
        r2[i]=i*2;
    }
    MPI_Comm_group(MPI_COMM_WORLD, &p);
    MPI_Group_incl(p, r_counts, r2, &M_G);
    MPI_Comm_create(MPI_COMM_WORLD, M_G, &M_C);
    int *sbuf=new int[r_counts];
    if (!r)
    {
        for (int i=0; i<r_counts; i++)
        {
            cin>>sbuf[i];
        }
    }
    int rbuf;
    if (!(r%2))
    {
        MPI_Scatter(sbuf, 1, MPI_INT, &rbuf, 1, MPI_INT, 0, M_C);
        cout<<rbuf;
    }
    return 0;
}
