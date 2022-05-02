#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "mpi.h"

void readMatrix(char *fileName, int *matrix, int dim1, int dim2) {
    FILE *file = fopen(fileName, "r");
    for (int i = 0; i < dim1; ++i) {
        for (int j = 0; j < dim2; ++j) {
            fscanf(file, "%d ", &matrix[i * dim2 + j]);
        }
    }
}

void printMatrix(int *matrix, int dim1, int dim2, int rank) {
    printf("Sono il processo %d e stampo la matrice: \n", rank);
    for (int i = 0; i < dim1; ++i) {
        for (int j = 0; j < dim2; ++j) {
            printf("%d ", matrix[i * dim2 + j]);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {


    MPI_Init(&argc, &argv);

    int nProc, myRank, dim = 8;
    int a[dim][dim];

    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    MPI_Comm_size(MPI_COMM_WORLD, &nProc);
    MPI_Status status;

    int k = dim / nProc;
    int n = nProc / 2;

    if (myRank == 0) {
        readMatrix("mat.txt", &a[0][0], dim, dim);
        printMatrix(&a[0][0], dim, dim, myRank);

        MPI_Datatype newDataType;
        MPI_Type_vector(k, dim, nProc * dim, MPI_INT, &newDataType);
        MPI_Type_commit(&newDataType);

        for (int i = 0; i < nProc; ++i) {
            MPI_Send(&a[i][0], 1, newDataType, i, 0, MPI_COMM_WORLD);
        }
    }

    int t[k][dim];

    MPI_Recv(&t[0][0], k * dim, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    printMatrix(&t[0][0], k, dim, myRank);

    //! Punto 2

    MPI_Comm topologyComm;
    int dims[2] = {n, n};
    int periods[2] = {false, false};
    int reorder = false;

    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, reorder, &topologyComm);
    int coords[2];
    MPI_Cart_coords(topologyComm, myRank, 2, coords);
    // Creo un communicator usando come colore i processi che sono sulla stessa riga
    MPI_Comm rowComm;
    MPI_Comm_split(MPI_COMM_WORLD, coords[0], myRank, &rowComm);
    // Se il mio indice di riga e di colonna è uguale sono sulla diagonale principale
    int t2[k][dim]; // invio una copia di t a tutta la riga
    if (coords[0] == coords[1]) {
        for (int i = 0; i < k; i++) {
            for (int j = 0; j < dim; j++) {
                t2[i][j] = t[i][j];
            }
        }
    }

    // ** Perché il processo root è uguale al numero di riga (coords[0]) ?
    MPI_Bcast(&t2[0][0], k * dim, MPI_INT, coords[0], rowComm);
    printf("Sono il processo %d e il mio coord è %d \n", myRank, coords[0]);

    MPI_Barrier(MPI_COMM_WORLD);
    sleep(1);
    printMatrix(&t2[0][0], k, dim, myRank);

    int t2Transposed[dim][k];
    //trasposta di t2
    for (int i = 0; i < dim; ++i) {
        for (int j = 0; j < k; ++j) {
            t2Transposed[i][j] = t2[j][i];
        }
    }
    // prodotto righe per colonne di t per t2Transposed
    int c[k][k];
    for (int i = 0; i < k; ++i) { // righe della prima
        for (int j = 0; j < k; ++j) { // colonne della seconda
            int sum = 0;
            for (int l = 0; l < dim; ++l) { // colonne della prima e righe della seconda
                sum += t[i][l] * t2Transposed[l][j];
            }
            c[i][j] = sum;
        }
    }

    sleep(1);
    MPI_Barrier(MPI_COMM_WORLD);
    printMatrix(&c[0][0], k, k, myRank);

    int vectorDiagonal[k];
    for (int i = 0; i < k; ++i) {
        for (int j = 0; j < k; ++j) {
            if (i == j)
                vectorDiagonal[i] = c[i][j];
        }
    }

    //! PUNTO 4 (???)

    int diag[k], result[k];

    for (int i = 0; i < k; ++i) {
            diag[i] = c[i][i];
    }

    MPI_Barrier(MPI_COMM_WORLD);
    sleep(1);

    MPI_Allreduce(&diag[0], &result[0], k, MPI_INT, MPI_MAX, MPI_COMM_WORLD);
    printMatrix(&result[0], 1, k, myRank);

    //! PUNTO 5 (NON CAPISCO)






    MPI_Finalize();
    return 0;
}
