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

    int src=3;

    if (rank == src)
    {
        int v[size - 1];
        int j = 0;
        for (int i = 0; i < size; ++i)
        {
            if (i != src)
            {
                MPI_Ssend(v + j, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
                j++;
            }
        }
    }
    else
    {
        int ch;
        MPI_Recv(&ch, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &stat);
        cout <<rank<<" "<< ch << endl;
    }
    MPI_Finalize();
    return 0;
}