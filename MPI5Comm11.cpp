#include <iostream>
#include "pt4.h"
#include "mpi.h"

using namespace std;

void main()
{
    int f, r, s, q;
    MPI_Initialized(&f);
    MPI_Comm_size(MPI_COMM_WORLD, &s);
    MPI_Comm_rank(MPI_COMM_WORLD, &r);
    pt>>q;
    MPI_Comm c;
    MPI_Comm_split(MPI_COMM_WORLD, !q ? MPI_UNDEFINED : 0, r, &c);
    MPI_Comm_size(c, &s);
    MPI_Comm_rank(c, &r);
    float buf_s, buf_r;
    cin>>buf_s;
    if (q)
    {
        MPI_Allreduce(&buf_s, &buf_r, 1, MPI_FLOAT, MPI_SUM, c);
        cout<<buf_r;
    }
    return 0;
}
