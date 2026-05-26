#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    int rank, size;
    int testigo;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int destino = (rank + 1) % size;
    int origen = (rank - 1 + size) % size;

    // Proceso 0 inicia el testigo
    if (rank == 0) {
        testigo = 100;

        printf("Proceso 0 inicia con testigo = %d\n", testigo);

        // Envía al proceso 1
        MPI_Send(&testigo, 1, MPI_INT, destino, 0, MPI_COMM_WORLD);

        // Recibe el valor final del último proceso
        MPI_Recv(&testigo, 1, MPI_INT, origen, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        printf("Proceso 0 recibe el valor final = %d\n", testigo);
    }
    else {
        // Recibe del proceso anterior
        MPI_Recv(&testigo, 1, MPI_INT, origen, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        printf("Proceso %d recibió testigo = %d\n", rank, testigo);

        // Suma su rank
        testigo += rank;

        printf("Proceso %d envía testigo = %d\n", rank, testigo);

        // Envía al siguiente proceso
        MPI_Send(&testigo, 1, MPI_INT, destino, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
}
