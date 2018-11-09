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

    MPI_Aint disp[3] = {0, sizeof(int), sizeof(int) + sizeof(int)};

    int position = 0;

    MPI_Type_create_struct(3, blocklen, disp, types, &mytype);
    MPI_Type_commit(&mytype);
    Data buf(rank, rank, rank);

    Data outbuf;
    Data dist[size];

    MPI_Pack(&buf, 1, mytype, &outbuf, sizeof(buf), &position, MPI_COMM_WORLD);

    MPI_Gather(&outbuf, position, MPI_PACKED, &dist, 1, mytype, 0, MPI_COMM_WORLD);
    if (!rank)
    {
        int pos = 0;
        Data dist2[size];
        MPI_Unpack(dist, sizeof(buf) * size, &pos, dist2, size, mytype, MPI_COMM_WORLD);

        for (int i = 0; i < size; ++i)
            printf("rank %d: %d %d %f\n", rank, dist2[i].a1, dist2[i].a2, dist2[i].a3);
    }
    MPI_Finalize();
    return 0;
}