#include <stdio.h>
#include <mpi.h>
#include <unistd.h>
#include "limits.h"

void readVector(char *fileName, int *vector, int dim) {

    FILE *file = fopen(fileName, "r");
    for (int i = 0; i < dim; ++i) {
        fscanf(file, "%d ", &vector[i]);
    }
}

void printVector(int *vector, int dim, int myRank) {
    printf("Sono il processo %d e stampo \n", myRank);
    for (int i = 0; i < dim; ++i) {
        printf("%d ", vector[i]);
    }
    printf("\n");
}


int main(int argc, char *argv[]) {

    MPI_Init(&argc, &argv);

    int nProc, myRank;
    int dim = 8;
    int a[dim], b[dim];

    MPI_Comm_size(MPI_COMM_WORLD, &nProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    MPI_Status status;

    int k = dim / nProc;

    //! Punto 1,2 e 3

    if (myRank == 0) {
        readVector("a.txt", &a[0], dim);
        readVector("b.txt", &b[0], dim);

//        printVector(&a[0], dim, myRank);
//        printVector(&b[0], dim, myRank);

        int m = 1; // vogliamo mandare un numero ad ogni processo
        MPI_Datatype block;
        MPI_Type_vector(k / m, m, nProc * m, MPI_INT, &block);
        MPI_Type_commit(&block);
        for (int i = 0; i < nProc; ++i) {
            MPI_Send(&a[i * k], k, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&b[i * m], 1, block, i, 1, MPI_COMM_WORLD);
        }
    }

    int a1[k], b1[k];
    MPI_Recv(&a1, k, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    MPI_Recv(&b1, k, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);

    if (myRank == 0)
        printf("\n------------- stampo a1 e b1 per ogni processo -------------\n");
    printVector(&a1[0], k, myRank);
    printVector(&b1[0], k, myRank);

    MPI_Barrier(MPI_COMM_WORLD);
    sleep(1);

    //! Punto 4

    int scalarProduct = 0;
    for (int i = 0; i < k; ++i) {
        scalarProduct += a1[i] * b1[i];
    }

    int t[nProc];

    MPI_Allgather(&scalarProduct, 1, MPI_INT, &t, 1, MPI_INT, MPI_COMM_WORLD);


    MPI_Barrier(MPI_COMM_WORLD);

    if (myRank == 0)
        printf("Stampo i risultati dei prodotti scalari dopo allGather  \n"); // gli altri
    printVector(&t[0], nProc, myRank);
    sleep(1);


    //! Punto 5 e 6

    int orderedRank[nProc]; // mi creo un vettore dei rank che ha la nella posizione 0 il primo massimo e nella posizione nProc - 1 il minimo// 0 == value e 1 == indice
    int tCopy[nProc];

    // creo una copia di t
    for (int i = 0; i < nProc; ++i) {
        tCopy[i] = t[i];
    }

    for (int j = 0; j < nProc; ++j) {
        orderedRank[j] = 0; // metto tutti a zero
        int tempMax = tCopy[0];
        int tempMaxIndex = 0;
        for (int i = 0; i < nProc; ++i) {
            if (tCopy[i] > tempMax) {
                orderedRank[j] = i; // Copio l'indice in cui si trova il massimo
                tempMaxIndex = i;
                tempMax = tCopy[i];
            }
        }
        tCopy[tempMaxIndex] = INT_MIN; // il minimo lo devo fare fuori dal for con indice i altrimenti non riesco a calcolare il massimo alle iterazioni successive
    }

    if (myRank == 0)
        printf("\n------------- stampo matrici ordinate  -------------\n");
    printVector(&orderedRank[0], nProc, myRank);

    MPI_Barrier(MPI_COMM_WORLD);
    sleep(1);

    int tempSum[k];

    for (int i = 0; i < k; ++i) { // tutti deveno avere il coso inizializzato
        tempSum[i] = 0;
    }

    if (myRank != orderedRank[0]) { // L'elemento che ha il massimo (cioè elemento 0 di orderedRank) non deve fare Recv)

        MPI_Recv(&tempSum[0], k, MPI_INT, MPI_ANY_SOURCE, 5, MPI_COMM_WORLD, &status);

    } else { // Solo il processo col massimo deve sommare a1
        for (int j = 0; j < k; ++j) {
            tempSum[k] = a1[k];
        }
    }
    // TUTI devono sommare b1 al tempSum
    for (int j = 0; j < k; ++j) {
        tempSum[j] += b1[j];
    }
    // Tutti effettuano la send tranne l'ultimo
    if (myRank != orderedRank[nProc - 1]) {
        int myDest = -1;
        for (int i = 0; i < nProc; ++i) {
            if (myRank == orderedRank[i]) {
                myDest = orderedRank[i + 1];
            }
        }
        MPI_Send(&tempSum[0], k, MPI_INT, myDest, 5, MPI_COMM_WORLD);
    }

    if (myRank == 0)
        printf("\n------------- stampo valori pipe per ogni processo -------------\n");
    printVector(&tempSum[0], k, myRank); // stampa dei vettori che hanno inviato


    MPI_Barrier(MPI_COMM_WORLD);
    sleep(1);

    MPI_Bcast(&tempSum, k, MPI_INT,orderedRank[nProc - 1],MPI_COMM_WORLD);

//    if (myRank == 0)
//        printf("\n------------- stampo risultato del broadcast (solo processo 0) -------------\n");
//        printVector(&tempSum[0], k, myRank);

    MPI_Finalize();

    return 0;
}
