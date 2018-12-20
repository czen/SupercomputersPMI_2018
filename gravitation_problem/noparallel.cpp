#include <iostream>
#include <cmath>
#include <iomanip>
#include <omp.h>
#include <vector>
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

Particle p[N];
Force f[N];
double m[N];

void Init()
{
    for (int i = 0; i < N; i++)
    {
        p[i].x = 20 * (i / 20 - 20) + 10;
        p[i].y = 20 * (i % 20 - 10) + 10;
        p[i].vx = p[i].y / 15;
        p[i].vy = -p[i].x / 50;
        m[i] = 100 + i % 100;
        f[i].x = 0;
        f[i].y = 0;
    }
}

void mass_init()
{
    for (int i = 0; i < N; i++)
        m[i] = 100 + i % 100;
}

void CalcForces1()
{

    int kkk = 0;
    for (int i = 0; i < N - 1; i++)
        for (int j = i + 1; j < N; j++)
        {
            double dx = p[j].x - p[i].x, dy = p[j].y - p[i].y,
                   r_2 = 1 / (dx * dx + dy * dy),
                   r_1 = sqrt(r_2),
                   fabs = grav * m[i] * m[j] * r_2;
            if (fabs > ffmax)
                fabs = ffmax;
            f[i].x += dx = fabs * dx * r_1;
            f[i].y += dy = fabs * dy * r_1;
            f[j].x -= dx;
            f[j].y -= dy;
        }
}

void MoveParticlesAndFreeForces()
{
    for (int i = 0; i < N; i++)
    {
        double dvx = f[i].x * dt / m[i],
               dvy = f[i].y * dt / m[i];
        p[i].x += (p[i].vx + dvx / 2) * dt;
        p[i].y += (p[i].vy + dvy / 2) * dt;
        p[i].vx += dvx;
        p[i].vy += dvy;
        f[i].x = 0;
        f[i].y = 0;
    }
}

void info(string s, double time)
{
    cout << setw(30) << left << s << "Time: " << fixed << setprecision(0) << setw(6) << 1000 * time
         << setprecision(12) << "p0: " << setw(12) << p[0].x << ", " << setw(12) << p[0].y << endl;
}

int main(int argc, char *argv[])
{
    Init();
    for (int i = 0; i < Niter; i++)
    {
        CalcForces1();
        MoveParticlesAndFreeForces();
    }
    info("Non-Parallel (N*N)", 0);
    return 0;
}
