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

    int myRank, size;

    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int rank = myRank;
    MPI_Status stat;

    printf("rank %d\n", myRank);

    if (myRank == 0)
    {
        int ch;

        MPI_Recv(&ch, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &stat);
        cout <<"source rank "<< stat.MPI_SOURCE<<" | "<< ch << endl;
    }
    else
    {
        int ch = 1;
        if (rank == size - 1)
            MPI_Send(&ch, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}