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

    int buf[3 * size];
    for (int i = 0; i < 3 * size; ++i)
        buf[i] = i;
    int dist[3];
    MPI_Scatter(buf, 3, MPI_INT, dist, 3, MPI_INT, 0, MPI_COMM_WORLD);

    printf("rank=%d: %d %d %d\n", rank, dist[0], dist[1], dist[2]);
    MPI_Finalize();
    return 0;
}