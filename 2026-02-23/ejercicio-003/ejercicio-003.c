#include <stdio.h>
#include <time.h>

// Estructura "Pesada"
// 50,000 enteros = 200 KB aprox.
// Imaginemos que esto se llama miles de veces por segundo.
typedef struct
{
    int data[50000];
} EstructuraPesada;

// OPCIÓN A: Pasando por VALOR (Ineficiente)
// Se crea una copia completa de la estructura cada vez que se llama.
void procesar_por_valor(EstructuraPesada s)
{
    int x = s.data[0]; // Solo leemos un dato
}

// OPCIÓN B: Pasando por REFERENCIA (Puntero - Eficiente)
// Solo se copia la dirección de memoria (8 bytes).
void procesar_por_puntero(EstructuraPesada *s)
{
    int x = s->data[0]; // Leemos el mismo dato
}

int main()
{
    EstructuraPesada objeto;
    objeto.data[0] = 42;

    clock_t start, end;
    long iteraciones = 100000; // 100 mil llamadas

    // 1. Prueba por VALOR
    start = clock();
    for (int i = 0; i < iteraciones; i++)
    {
        procesar_por_valor(objeto);
    }
    end = clock();
    double tiempo_valor = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Tiempo por VALOR (Copias): %f segundos\n", tiempo_valor);

    // 2. Prueba por PUNTERO
    start = clock();
    for (int i = 0; i < iteraciones; i++)
    {
        procesar_por_puntero(&objeto);
    }
    end = clock();
    double tiempo_ref = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Tiempo por PUNTERO (Ref):  %f segundos\n", tiempo_ref);

    printf("\nCONCLUSIÓN: Usar punteros fue %.2fx veces más rápido.\n", tiempo_valor / tiempo_ref);

    return 0;
}
