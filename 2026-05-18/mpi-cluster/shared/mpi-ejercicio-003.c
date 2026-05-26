#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        int alerta_recibida;
        for (int i = 1; i < size; i++) {
            MPI_Recv(&alerta_recibida, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("Master: Alerta confirmada desde el nodo %d con código %d\n", i, alerta_recibida);
        }
    } else {
        int codigo_alerta = rank * 10;
        MPI_Request request;
        MPI_Isend(&codigo_alerta, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &request);

        double calculo_local = 0.0;
        for (int i = 0; i < 1000000; i++) {
            calculo_local += 3.1415 * i;
        }

        MPI_Wait(&request, MPI_STATUS_IGNORE);
        printf("Trabajador %d: Mensaje liberado con éxito tras cómputo local.\n", rank);
    }

    MPI_Finalize();
    return 0;
}
