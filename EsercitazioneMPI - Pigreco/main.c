#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mpi.h"

#define sqr(x)    ((x)*(x))
#define TAG_ARBITRARY 12345

double boardDarts(int darts);

int main(int argc, char *argv[]) {


    int myRank, size;
    MPI_Comm communicator;
    MPI_Status status;

    // La logica che andremo a usare per creare il sistema delle freccette è, ogni processo lancia un tot numero di freccette
    // e invia il risultato al processo 0
    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    if (myRank == 0) {
        printf("\n Sono il processo %d \n", myRank);
        double pigrecoArray[size - 1];

        for (int i = 0; i < size - 1; ++i) {
            MPI_Recv(&pigrecoArray[i], 1, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            printf("Sono il processo 0 e ho ricevuto %f dal processo %d \n", pigrecoArray[i],
                   status.MPI_SOURCE); // status.MPI_SOURCE mi restituisce il rango del processo mittente
        }

        double pigrecoFinal;
        for (int i = 0; i < size - 1; ++i) {
            pigrecoFinal = pigrecoFinal + pigrecoArray[i];
        }
        pigrecoFinal = pigrecoFinal / (size - 1);
        printf("Il mio pigreco finale è: %f", pigrecoFinal);

    } else {
        srand(myRank); // Dobbiamo andare a dare seed al generatore di numeri casuali e per farlo ci serve un numero sempre diverso, ho scelto myRank
        int darts = rand() % 100000;// Calcola un numero casuale tra 0 e 100000
        printf("Sono il processo %d e voglio tirare %d frecce\n ", myRank, darts);
        double pigreco = boardDarts(darts);
        MPI_Send(&pigreco, 1, MPI_DOUBLE, 0, TAG_ARBITRARY, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;

}

double boardDarts(int darts) {
    double x_coord,       /* x coordinate, between -1 and 1  */
    y_coord,       /* y coordinate, between -1 and 1  */
    pi,            /* pi  */
    r;             /* random number between 0 and 1  */
    int score,            /* number of darts that hit circle */
    n;
    unsigned long cconst; /* used to convert integer random number */
    /* between 0 and 2^31 to double random number */
    /* between 0 and 1  */

    cconst = 2 << (15 - 1);
    score = 0;

    /* "throw darts at board" */
    for (n = 1; n <= darts; n++) {
        /* generate random numbers for x and y coordinates */
        r = (double) rand() / RAND_MAX;
        x_coord = (2.0 * r) - 1.0;
        r = (double) rand() / RAND_MAX;
        y_coord = (2.0 * r) - 1.0;

        /* if dart lands in circle, increment score */
        if ((sqr(x_coord) + sqr(y_coord)) <= 1.0)
            score++;
    }

    /* calculate pi */
    pi = 4.0 * (double) score / (double) darts;
    return (pi);
}