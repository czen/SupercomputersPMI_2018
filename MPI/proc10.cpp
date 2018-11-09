#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>
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
    printf("rank %d\n", myRank);
    srand(time(NULL));
    if (myRank == 0)
    {
        if (size % 2 == 0)
            myRank = 1;
    }
    if (myRank % 2)
    {
        int n = rand() % 10 + 1;
        cout << n << endl;
        vector<double> v(n);
        for (auto &u : v)
        {
            u = (double)(rand()) / RAND_MAX * 10;
            cout << u << " ";
        }
        cout << endl;
        if (rank)
            cout << "max=" << *max_element(v.begin(), v.end()) << endl;
        else
            cout << "min=" << *min_element(v.begin(), v.end()) << endl;
    }
    else
    {
        int n = rand() % 10 + 1;
        cout << n << endl;
        vector<int> v(n);
        for (auto &u : v)
        {
            u = rand() % 10;
            cout << u << " ";
        }
        cout << endl;
        cout << "min=" << *min_element(v.begin(), v.end()) << endl;
    }

    MPI_Finalize();
    return 0;
}