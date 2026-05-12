#include <stdio.h>
#include <omp.h>

#define N 10000000

int main()
{
    double suma = 0.0;
    double inicio = omp_get_wtime();

#pragma omp parallel for reduction(+ : suma)
    for (int i = 0; i < N; i++)

    {
        suma += i * 0.000001;
    }

    double fin = omp_get_wtime();

    printf("Suma: %f\n", suma);
    printf("Tiempo: %f segundos\n", fin - inicio);

    return 0;
}