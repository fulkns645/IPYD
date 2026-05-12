#include <stdio.h>
#include <omp.h>

#define N 100000

int main()
{
    static double a[N], b[N], c[N];

#pragma omp parallel for
    for (int i = 0; i < N; i++)
    {
        a[i] = i * 1.0;
        b[i] = i * 2.0;
    }

#pragma omp parallel for
    for (int i = 0; i < N; i++)
    {
        c[i] = a[i] + b[i];
    }

    printf("c[10] = %f\n", c[10]);
    return 0;
}