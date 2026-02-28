#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// TAMAÑO DEL ARREGLO
// 1 millón de doubles = 8 MB (Al límite del Stack típico)
// 2 millones de doubles = 16 MB (Seguro explota el Stack)
#define SIZE 2000000

void intento_en_stack()
{
    printf("\n[STACK] Intentando reservar arreglo en Stack...\n");
    // Las variables locales viven en el Stack.
    // El Stack tiene un límite rígido (ulimit -s en Linux suele ser 8192kb)
    double datos[SIZE];

    // Si llegamos aquí, sobrevivimos (pero probablemente no lleguemos)
    datos[0] = 1.0;
    datos[SIZE - 1] = 3.14;
    printf("[STACK] Éxito. Primer elemento: %f\n", datos[0]);
}

void intento_en_heap()
{
    printf("\n[HEAP] Intentando reservar arreglo en Heap (Punteros)...\n");
    // malloc solicita memoria al sistema operativo en el Heap.
    // El Heap puede crecer hasta llenar la RAM disponible.
    double *datos = (double *)malloc(SIZE * sizeof(double));

    if (datos == NULL)
    {
        printf("[HEAP] Falló la asignación (¿Se acabó la RAM del PC?).\n");
        return;
    }

    datos[0] = 1.0;
    datos[SIZE - 1] = 3.14;
    printf("[HEAP] Éxito. Dirección de memoria base: %p\n", (void *)datos);

    // Importante: En C, lo que pides al Heap, debes devolverlo.
    free(datos);
    printf("[HEAP] Memoria liberada.\n");
}

int main()
{
    printf("--- INICIO DEL EXPERIMENTO DE CAPACIDAD ---\n");
    printf("Tamaño a reservar: %lu MB\n", (SIZE * sizeof(double)) / (1024 * 1024));

    // 1. Probamos el Heap primero (Sabemos que funciona)
    intento_en_heap();

    // 2. Probamos el Stack (Probable Crash / Segmentation Fault)
    // PREGUNTA A ESTUDIANTES: ¿Por qué el programa muere aquí sin decir "Error"?
    intento_en_stack();

    printf("--- FIN DEL PROGRAMA (Si lees esto, el Stack aguantó) ---\n");
    return 0;
}
