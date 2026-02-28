// cache_demo.c
// Compilar: gcc -O2 -std=c11 cache_demo.c -o cache_demo
// Ejecutar: ./cache_demo filas | ./cache_demo columnas
/**

Que evidencia:
- Por filas: acceso secuencial, aprovecha la localidad espacial y la cache.
- Por columnas: salto de memoria grande, mas fallos de cache y menor rendimiento.

Sugerencias:
1) Compila con optimizacion: `gcc -O2 -std=c11 cache_demo.c -o cache_demo`
2) Prueba distintos tamanos con `-DN=8192` o `-DN=2048` para ver el efecto de la cache.

**/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#ifndef N
#define N 4096 // tamaño de la matriz NxN
#endif

/**
  Obtiene el tiempo actual y lo devuelve en segundos, e.g. 12.345678
  */
static double now_sec(void)
{
    return (double)clock() / CLOCKS_PER_SEC;
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Uso: %s filas|columnas\n", argv[0]);
        return 1;
    }

    int por_filas = 0;
    int por_columnas = 0;
    if (argv[1][0] == 'f' || argv[1][0] == 'r')
    {
        por_filas = 1;
    }
    else if (argv[1][0] == 'c')
    {
        por_columnas = 1;
    }
    else
    {
        fprintf(stderr, "Argumento invalido: %s\n", argv[1]);
        fprintf(stderr, "Uso: %s filas|columnas\n", argv[0]);
        return 1;
    }
    // Reservar matriz contigua
    size_t total = (size_t)N * (size_t)N;
    /**
      total * sizeof(int) debe ser multiplo de alignment (64)
      64 debe ser potencia de 2 y multiplo de sizeof(int)
      */
    int *A = (int *)malloc(total * sizeof(int));
    if (!A)
    {
        perror("aligned_alloc");
        return 1;
    }

    // Inicializar
    for (size_t i = 0; i < total; i++)
    {
        A[i] = (int)(i & 0xFF);
    }

    volatile uint64_t sum = 0; // evitar que el compilador elimine los bucles

    double t1 = now_sec();
    if (por_filas)
    {
        // Acceso por filas (cache-friendly)
        for (int i = 0; i < N; i++)
        {
            size_t base = (size_t)i * (size_t)N;
            for (int j = 0; j < N; j++)
            {
                sum += A[base + (size_t)j];
            }
        }
    }
    else
    {
        // Acceso por columnas (cache-unfriendly)
        for (int j = 0; j < N; j++)
        {
            for (int i = 0; i < N; i++)
            {
                sum += A[(size_t)i * (size_t)N + (size_t)j];
            }
        }
    }
    double t2 = now_sec();

    printf("N = %d\n", N);
    printf("Suma (ignorar): %llu\n", (unsigned long long)sum);
    if (por_filas)
    {
        printf("Por filas:   %.6f s\n", t2 - t1);
    }
    else
    {
        printf("Por columnas: %.6f s\n", t2 - t1);
    }

    free(A);
    return 0;
}
