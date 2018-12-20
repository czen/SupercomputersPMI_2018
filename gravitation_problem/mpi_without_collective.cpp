#include <iostream>
#include <cmath>
#include <vector>
#include <mpi.h>
#include <unistd.h>

using namespace std;

const int N = 12000, grav = 10, Niter = 100;
const double dt = 0.1, ffmax = 1;

struct Particle
{
    double x, y, vx, vy;
};

struct Force
{
    double x, y;
};

const Force nil{0, 0};

double m[N];
int iter = 0;

void mass_init()
{
    for (int i = 0; i < N; i++)
        m[i] = 100 + i % 100;
}

void mpi_init(int start, int end, vector<Particle> &p)
{
    p.resize(end - start);
    for (int i = start; i < end; ++i)
    {
        int k = i - start;
        p[k].x = 20 * (i / 20 - 20) + 10;
        p[k].y = 20 * (i % 20 - 10) + 10;
        p[k].vx = p[k].y / 15;
        p[k].vy = -p[k].x / 50;
    }
}

void block_CalcForces(vector<vector<Particle>> &p, vector<vector<Force>> &f, vector<int> &bloks, int rank, int k)
{
    for (int i = 0; i < p[0].size() - !k; ++i)
        for (int j = k ? 0 : i + 1; j < p[!!k].size(); ++j)
        {
            double dx = p[!!k][j].x - p[0][i].x, dy = p[!!k][j].y - p[0][i].y,
                   r_2 = 1 / (dx * dx + dy * dy),
                   r_1 = sqrt(r_2),
                   fabs = grav * m[bloks[rank] + i] * m[bloks[rank + k] + j] * r_2;

            if (fabs > ffmax)
                fabs = ffmax;

            f[0][i].x += dx = fabs * dx * r_1;
            f[0][i].y += dy = fabs * dy * r_1;
            f[!!k][j].x -= dx;
            f[!!k][j].y -= dy;
        }
}

void find_bloks(int size, int n, vector<int> &bloks)
{
    int all = n * (n - 1) / 2;
    int sr = all / size;
    int s = 0;
    bloks.reserve(size + 1);
    bloks.push_back(0);
    if (size > 1)
        for (int i = 0; i < n; ++i)
        {
            s += n - i - 1;
            if (s >= sr)
            {
                bloks.push_back(i);
                s = 0;
            }
        }
    bloks.push_back(n);
}

MPI_Datatype mpiParticle()
{
    MPI_Datatype mytype;
    MPI_Datatype types[4] = {MPI_DOUBLE, MPI_DOUBLE, MPI_DOUBLE, MPI_DOUBLE};
    int blocklen[4] = {1, 1, 1, 1};
    MPI_Aint disp[4] = {offsetof(Particle, x), offsetof(Particle, y), offsetof(Particle, vx), offsetof(Particle, vy)};

    MPI_Type_create_struct(4, blocklen, disp, types, &mytype);
    MPI_Type_commit(&mytype);
    return mytype;
}

MPI_Datatype mpiForce()
{
    MPI_Datatype mytype;
    MPI_Datatype types[2] = {MPI_DOUBLE, MPI_DOUBLE};
    int blocklen[2] = {1, 1};
    MPI_Aint disp[2] = {offsetof(Particle, x), offsetof(Particle, y)};

    MPI_Type_create_struct(2, blocklen, disp, types, &mytype);
    MPI_Type_commit(&mytype);
    return mytype;
}

int mpi_func(int argc, char *argv[])
{
    int errCode;
    MPI_Status status;
    if ((errCode = MPI_Init(&argc, &argv)) != 0)
    {
        return errCode;
    }
    int rank, size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    mass_init();

    vector<int> bloks;
    find_bloks(size, N, bloks);

    auto mpiPrt = mpiParticle();
    auto mpiFrc = mpiForce();

    vector<vector<Particle>> p(2);
    vector<vector<Force>> f(2);

    mpi_init(bloks[rank], bloks[rank + 1], p[0]);
    f[0].resize(p[0].size());

    p[1].resize(bloks[bloks.size() - 1] - bloks[bloks.size() - 2]);
    f[1].resize(p[1].size());

    MPI_Request g;
    MPI_Request gg;

    for (int s = 0; s < Niter; ++s)
    {
        for (int q = 0; q < rank; ++q)
        {
            MPI_Isend(&p[0][0], p[0].size(), mpiPrt, q, 0, MPI_COMM_WORLD, &g);
        }

        block_CalcForces(p, f, bloks, rank, 0);

        for (int q = rank + 1; q < size; ++q)
        {
            p[1].resize(bloks[q + 1] - bloks[q]);
            f[1].resize(p[1].size());
            MPI_Recv(&p[1][0], p[1].size(), mpiPrt, q, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            fill(f[1].begin(), f[1].end(), nil);
            block_CalcForces(p, f, bloks, rank, q - rank);
            MPI_Isend(&f[1][0], f[1].size(), mpiFrc, q, 1, MPI_COMM_WORLD, &g);
        }
        f[1].resize(f[0].size());
        for (int q = 0; q < rank; ++q)
        {
            MPI_Recv(&f[1][0], f[1].size(), mpiPrt, q, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            for (int i = 0; i < f[0].size(); ++i)
            {
                f[0][i].x += f[1][i].x;
                f[0][i].y += f[1][i].y;
            }
        }

        for (int i = 0; i < p[0].size(); ++i)
        {
            double dvx = f[0][i].x * dt / m[bloks[rank] + i],
                   dvy = f[0][i].y * dt / m[bloks[rank] + i];
            p[0][i].x += (p[0][i].vx + dvx / 2) * dt;
            p[0][i].y += (p[0][i].vy + dvy / 2) * dt;
            p[0][i].vx += dvx;
            p[0][i].vy += dvy;
            f[0][i].x = 0;
            f[0][i].y = 0;
        }
    }

    if (!rank)
        cout << p[0][0].x << " " << p[0][0].y << endl;

    MPI_Finalize();
    return 0;
}

int main(int argc, char *argv[])
{
    mpi_func(argc, argv);
    return 0;
}
