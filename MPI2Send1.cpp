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
    MPI_Status stat;
    if (r>0)
    {
        cin>>q;
        MPI_Send(&q, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    else
    {
        for (auto i=1; i<s; ++i)
        {
            MPI_Recv(&q, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &stat);
            cout<<q;
        }
    }
    return 0;
}
