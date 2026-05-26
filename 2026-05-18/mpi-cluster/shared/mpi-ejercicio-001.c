#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size % 2 != 0 || size < 4) {
        if (rank == 0) {
            printf("Error: Se requiere un número par de procesos >= 4\n");
        }
        MPI_Finalize();
        return 0;
    }

    if (rank == 0) {
        printf("Proceso maestro. Total procesos: %d\n", size);
    }
    else if (rank % 2 == 0) {
        printf("Hola desde el rank par %d\n", rank);
    }
    else {
        printf("Hola desde el rank impar %d\n", rank);
    }

    MPI_Finalize();
    return 0;
}
