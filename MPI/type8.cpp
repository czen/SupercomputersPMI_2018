#include <stdio.h>
#include <iostream>
#include <mpi.h>

struct Data
{

    Data(int a, int b, double c) : a1(a), a2(b), a3(c) {}
    Data() {}
    int a1;
    int a2;
    double a3;
};

using namespace std;
int main(int argc, char *argv[])
{
    int errCode;

    if ((errCode = MPI_Init(&argc, &argv)) != 0)
    {
        return errCode;
    }

    int rank, size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Datatype mytype;
    MPI_Datatype types[3] = {MPI_INT, MPI_INT, MPI_DOUBLE};
    int blocklen[3] = {1, 1, 1};
    Data buf[rank];
    MPI_Aint disp[3] = {0, sizeof(int), sizeof(int) + sizeof(int)};

    MPI_Type_create_struct(3, blocklen, disp, types, &mytype);
    MPI_Type_commit(&mytype);

    for (int i = 0; i < rank; ++i)
        buf[i] = Data(rank, rank, rank);
    int n = (size + 1) * size / 2;
    Data dist[n];
    int scounts[size];
    int displs[size];
    int count = 0;
    for (int i = 0; i < size; ++i)
    {
        scounts[i] = i;
        displs[i] = count;
        count += scounts[i];
    }

    MPI_Gatherv(&buf, rank, mytype, &dist, scounts, displs, mytype, 0, MPI_COMM_WORLD);
    if (rank == 0)
        for (int i = 0; i < size; ++i)
            printf("rank %d: %d %d %f\n", rank, dist[i].a1, dist[i].a2, dist[i].a3);
    MPI_Finalize();
    return 0;
}