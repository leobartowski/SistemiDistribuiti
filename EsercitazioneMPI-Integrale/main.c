#include <stdio.h>
#include <math.h>
#include "mpi.h"

#define TAG_ARBITRARY 12345

/*Function definition to perform integration by Trapezoidal Rule */
double trapezoidal(double f(double x), double a, double b, int n) {
    double x, h, sum = 0, integral;
    int i;
    h = fabs(b - a) / n;
    for (i = 1; i < n; i++) {
        x = a + i * h;
        sum = sum + f(x);
    }
    integral = (h / 2) * (f(a) + f(b) + 2 * sum);
    return integral;
}

/* Define the function to be integrated here: */
double f(double x) {
    return x * x;
}

// Variante classica con send e receive
int main(int argc, char *argv[]) {

    int myRank, size;
    MPI_Comm communicator;
    MPI_Status status;
    double x, a, b, stepSize; // n rappresenta il numero di intervalli in cui divido il mio calcolo

    MPI_Init(&argc, &argv);

    // Assegno i valori per calcolare l'integrale
    a = 5; // limite inferiore
    b = 6; // limite superiore

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    // TODO: Secondo me dovrebbe essere size -1 ma funziona con size. capire perchè!
    int subInterval = size; // il numero di sottointervalli da utilizzare è il numero dei processi
//    double values [subInterval];
    stepSize = fabs(b - a) / subInterval; // funzione valore assoluto

    if (myRank != 0) {
        x = a + myRank * stepSize; // ho sostituito i con myRank
        // devo mandare f(x)
        double valueToSend = f(x);
        MPI_Send(&valueToSend, 1, MPI_DOUBLE, 0, TAG_ARBITRARY, MPI_COMM_WORLD);

    } else {

        double sum = 0, integral_result;
        for (int i = 0; i < size - 1; ++i) {
            double partialSum = 0;
            MPI_Recv(&partialSum, 1, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            printf("Sono il processo 0 e ho ricevuto %f dal processo %d \n", partialSum, status.MPI_SOURCE);
            sum = sum + partialSum;
        }

        integral_result = (stepSize / 2) * (f(a) + f(b) + 2 * sum);
        printf("Il risultato dell'integrale è:  %f \n", integral_result);

    }
    MPI_Finalize();
}

// Variante con BCast e Reduce, calcolo collettivo
//int main(int argc, char *argv[]) {
//
//
//
//}

