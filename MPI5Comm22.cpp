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
    MPI_Comm c, c_t;
    const auto dim_n=3;
    auto dim=new int[dim_n] {2, 2, s/4}, per=new int[dim_n] {0, 0, 0}, crds=new int[dim_n] {1, 1, 0};
    MPI_Cart_create(MPI_COMM_WORLD, dim_n, dim, per, 0, &c_t);
    MPI_Cart_sub(c_t, crds, &c);
    MPI_Comm_size(c, &s);
    MPI_Comm_rank(c, &r);
    float q, w;
    cin>>q;
    MPI_Allreduce(&q, &w, 1, MPI_DOUBLE, MPI_SUM, c);
    cout<<w;
    return 0;
}
