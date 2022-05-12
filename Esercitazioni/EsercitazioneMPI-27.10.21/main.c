#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "mpi.h"

#define TAG_ARBITRARY 12345
#define VAL_MAX 100

int main(int argc, char **argv) {

    int myRank, size;
    int destinatario = 2;
    int x;
    int cont;

    MPI_Comm communicator;
    MPI_Status status;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    printf("Sono il processo con rango: %d \n", myRank);

    if (myRank == 0) {
        printf("\n Sono il processo %d inserisci un numero intero: \n", myRank);
        scanf("%d", &x);
        // Invio x al processo con rango 1
        MPI_Send(&x, 1, MPI_INT, 1, TAG_ARBITRARY, MPI_COMM_WORLD);
    }

    do {
        MPI_Recv(&x, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        printf("Sono il processo %d e ho ricevuto  %d dal processo %d \n", myRank, x, status.MPI_SOURCE); // status.MPI_SOURCE mi restituisce il rango del processo mittente!
        if (myRank == size - 1) { // Sono l'ultimo e devo quindi inviare al primo
            destinatario = 0;
        } else {
            destinatario = myRank + 1;
        }
        if (myRank == 0) {
            x--;
        }
        MPI_Send(&x, 1, MPI_INT, destinatario, TAG_ARBITRARY, MPI_COMM_WORLD);
    } while(x>0);

    MPI_Finalize();

    return 0;
}
