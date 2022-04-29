#include <stdio.h>
#include "mpi.h"

void readMatrix(char *fileName, int *matrix, int dim1, int dim2) {

    FILE *file = fopen(fileName, "r");
    for (int i = 0; i < dim1; ++i) {
        for (int j = 0; j < dim2; ++j) {
            fscanf(file, "%d ", &matrix[i * dim2 + j]);
        }
    }
}

void printMatrix(int *matrix, int dim1, int dim2, int myRank) {
    printf("\nSono il processo %d  e stampo la matrice: \n", myRank);
    for (int i = 0; i < dim1; ++i) {
        for (int j = 0; j < dim2; ++j) {
            printf("% d", matrix[i * dim2 + j]);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {

    MPI_Init(&argc, &argv);

    int myRank, nProc, dim = 8;

    MPI_Comm_size(MPI_COMM_WORLD, &nProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    MPI_Status status;

    int k = dim / nProc;

    int matA[dim][dim], matB[dim][dim];


    if (myRank == 0) {
        readMatrix("matA.txt", &matA[0][0], dim, dim);
        readMatrix("matB.txt", &matB[0][0], dim, dim);
        printMatrix(&matA[0][0], dim, dim, myRank);
        printMatrix(&matB[0][0], dim, dim, myRank);
        MPI_Datatype column;
        MPI_Type_vector(dim, k, dim, MPI_INT, &column);
        MPI_Type_commit(&column);

        for (int i = 0; i < nProc; ++i) {
            MPI_Send(&matA[i * k][0], k * dim, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&matB[0][i * k], 1, column, i, 1, MPI_COMM_WORLD);
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);
    int matR[k][dim], matC[dim][k];
    MPI_Recv(&matR[0][0], k * dim, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    MPI_Recv(&matC[0][0], k * dim, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
//    printMatrix(&matR[0][0], k, dim, myRank);
//    printMatrix(&matC[0][0], dim, k, myRank);

    int c[k][k];

    // Prodotto righe per colonne
    for (int i = 0; i < k; ++i) {
        for (int j = 0; j < k; ++j) {
            c[i][j] = 0;
            for (int l = 0; l < dim; ++l) {
                c[i][j] = c[i][j] + matR[i][l] * matC[l][j];
            }
        }
    }

    // Ordinamento prima colonna
    int temp = 0;
    for (int i = 0; i < k; i++) {
        for (int j = i + 1; j < k; j++) {
            if (c[i][0] < c[j][0]) {
                temp = c[i][0];
                c[i][0] = c[j][0];
                c[j][0] = temp;
            }
        }
    }

    printMatrix(&c[0][0], k, k, myRank);

    int average = 0;
    MPI_Allreduce(&c[0][0], &average, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    printf("Sono il processo %d e la somma è %d \n", myRank, average);
    average = average / nProc;
    printf("Sono il processo %d e la media è %d \n", myRank, average);

    int color = c[0][0] >= average ? 1 : 2;

    MPI_Comm newComm;
    MPI_Comm_split(MPI_COMM_WORLD, color, 0, &newComm);

    if (color == 1) {
        int max;
        MPI_Allreduce(&c[0][0], & max, 1, MPI_INT, MPI_MAX, newComm);
        printf("Il massimo è %d \n", max);
    } else {
        int min;
        MPI_Allreduce(&c[0][0], &min, 1, MPI_INT, MPI_MIN, newComm);
        printf("Il minimo è %d \n", min);
    }

    MPI_Finalize();
    return 0;

}
