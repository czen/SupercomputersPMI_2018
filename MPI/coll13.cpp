#include <stdio.h>
#include <iostream>
#include <mpi.h>

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
    MPI_Status stat;

    int buf[rank + 2];
    for (int i = 0; i < rank + 2; ++i)
        buf[i] = i + 10 * rank;

    int scounts[size], displs[size];
    int count = 0;
    for (int i = 0; i < size; ++i)
    {
        scounts[i] = i + 2;
        displs[i] = count;
        count += scounts[i];
    }
    int dist[(3 + size) * size / 2];
    MPI_Allgatherv(buf, scounts[rank], MPI_INT, dist, scounts, displs, MPI_INT, MPI_COMM_WORLD);

    for (int i = 0; i < (3 + size) * size / 2; ++i)
        printf("%d ", dist[i]);
    printf("\n");

    MPI_Finalize();
    return 0;
}