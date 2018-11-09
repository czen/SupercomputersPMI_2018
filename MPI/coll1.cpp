#include <stdio.h>
//#include <iostream>
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

    int ss = 0;
    if (rank == 0)
        ss = 123;

    MPI_Bcast(&ss, 1, MPI_INT, 0, MPI_COMM_WORLD);
    if (rank == 0)
        ss = 0;

    printf("rank %d s%d\n", rank, ss);
    MPI_Finalize();
    return 0;
}