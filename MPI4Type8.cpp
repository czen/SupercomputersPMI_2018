#include <iostream>
#include "pt4.h"
#include "mpi.h"

using namespace std;

const auto COUNT=3;

struct triad
{
	int t1, t2;
	double t3;
	triad(): t1(0), t2(0), t3(0) {}
	friend PTIO& operator << (PTIO& q, triad const &w)
	{
		return q<<w.t1<<w.t2<<w.t3;
	}
	friend PTIO& operator >> (PTIO& q, triad &w)
	{
		return q>>w.t1>>w.t2>>w.t3;
	}
};

void main()
{
    int f, r, s;
    MPI_Initialized(&f);
    MPI_Comm_size(MPI_COMM_WORLD, &s);
    MPI_Comm_rank(MPI_COMM_WORLD, &r);
    MPI_Datatype MPI_triad;
    auto b_lens=new int[COUNT] {1, 1, 1};
    auto triad_d=new MPI_Aint[COUNT]
    {
        offsetof(triad, t1),
        offsetof(triad, t2),
        offsetof(triad, t3)
    };
    auto old_t=new MPI_Datatype[COUNT]{MPI_INT, MPI_INT, MPI_DOUBLE};
    MPI_Type_struct(COUNT, b_lens, triad_d, old_t, &MPI_triad);
    MPI_Type_commit(&MPI_triad);
    auto buf_s=new triad[r];
    for (auto i=0; i<r; i++)
    {
        pt>>buf_s[i];
    }
    auto buf_rs=0;
    triad *buf_r=nullptr;
    int *buf_c=nullptr, *d=nullptr;
    if (!r)
    {
        buf_c=new int[s];
        d=new int[s];
        for (auto i=0; i<s; i++)
        {
            buf_rs+=i;
            buf_c[i]=i;
            d[i]=!i?0 : d[i-1]+buf_c[i-1];
        }
        buf_r=new triad[buf_rs];
    }
    MPI_Gatherv(buf_s, r, MPI_triad, buf_r, buf_c, d, MPI_triad, 0, MPI_COMM_WORLD);
    if (!r)
    {
        for (auto i=0; i<buf_rs; i++)
        {
            pt<<buf_r[i];
        }
    }
    return 0;
}
