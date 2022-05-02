#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
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
    printf("Sono il processo %d e stampo la matrice \n", myRank);
    for (int i = 0; i < dim1; ++i) {
        for (int j = 0; j < dim2; ++j) {
            printf("%d ", matrix[i * dim2 + j]);
        }
        printf("\n");
    }
}


int main(int argc, char *argv[]) {

    int myRank, nProc;
    int dim = 16;
    int a[dim][dim], b[dim][dim];

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &nProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    MPI_Status status;

    int k = dim / nProc;

    MPI_Datatype rowDT, columnDT;

    if (myRank == 0) {

        readMatrix("matA.txt", &a[0][0], dim, dim);
        readMatrix("matB.txt", &b[0][0], dim, dim);

//        printMatrix(&a[0][0], dim, dim, myRank);
        printMatrix(&b[0][0], dim, dim, myRank);

        // Row Round Robin DataType
        MPI_Type_vector(k, dim, nProc * dim, MPI_INT, &rowDT);
        MPI_Type_commit(&rowDT);
        // Column Round Robin DataType

        MPI_Type_vector(k * dim, 1, nProc, MPI_INT, &columnDT);
        MPI_Type_commit(&columnDT);

        for (int i = 0; i < nProc; ++i) {
            MPI_Send(&a[i][0], 1, rowDT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&b[0][i], 1, columnDT, i, 1, MPI_COMM_WORLD);
        }
    }

    int tRow[k][dim], tColumn[dim][k];

    MPI_Recv(&tRow[0][0], k * dim, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    MPI_Recv(&tColumn[0][0], k * dim, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);

//    printMatrix(&tRow[0][0], k, dim, myRank);
//    printMatrix(&tColumn[0][0], dim, k, myRank);

    //! Punto 2

    int c[k][k];
    // Prodotto riga per colonne
    for (int i = 0; i < k; ++i) {
        for (int j = 0; j < k; ++j) {
            int sum = 0;
            for (int l = 0; l < dim; ++l) {
                sum += tRow[i][l] * tColumn[l][j];
            }
            c[i][j] = sum;
        }
    }

//    printMatrix(&c[0][0], k, k, myRank);

    MPI_Barrier(MPI_COMM_WORLD);

    //! Punto 3

    MPI_Datatype singleColumn;
    MPI_Type_vector(k, 1, k, MPI_INT, &singleColumn);
    MPI_Type_commit(&singleColumn);


    if (myRank == 0) {
        printf("------------------------------------\n");
        int columnToSend[k];
        for (int i = 0; i < k; ++i) {
            columnToSend[i] = c[i][myRank];
        }
        MPI_Send(&columnToSend[0], 1, singleColumn, myRank + 1, 0, MPI_COMM_WORLD);

    } else {

        int columnReceived[k];
        MPI_Recv(&columnReceived[0], k, MPI_INT, myRank - 1, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
        int count = 0; // contatore che mi serve per contare quanti elementi del vettore ricevuto sono maggiori della mia colonna
        for (int i = 0; i < k; ++i) {
            if (columnReceived[i] > c[i][myRank])
                count++;
        }

        if (count == k) {
            for (int i = 0; i < k; ++i) {
                c[i][myRank] = columnReceived[i];
            }
        }

        int columnToSend[k];
        for (int i = 0; i < k; ++i) {
            columnToSend[i] = c[i][myRank];
        }
        if (myRank != nProc - 1)
            MPI_Send(&columnToSend[0], 1, singleColumn, myRank + 1, 0, MPI_COMM_WORLD);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    sleep(1);
    printMatrix(&c[0][0], k, k, myRank);


    //! Punto 4

    int max[k];
    MPI_Allreduce(&c[0][0], &max[0], k, MPI_INT, MPI_MAX, MPI_COMM_WORLD);

    printMatrix(&max[0], 1, k, myRank);

    //! Punto 5
    if (myRank != 0) {
        int diff[k];
        bool sendDiff = true;
        for (int i = 0; i < k; ++i) {
            diff[i] = max[i] - c[0][i];
            if (diff[i] == 0) {
                sendDiff = false; // i processi che hanno 0 non fanno la send
            }
        }

        if (sendDiff) {
            char *message = "ciao";
            printf("Sono il processo %d e ho effetuato una send"
                   " \n", myRank);

            MPI_Send(message, strlen(message), MPI_CHAR, 0, 10, MPI_COMM_WORLD);
        }
    } else { // sono il processo 0
        char messageRcv[4];

        MPI_Recv(&messageRcv, 4, MPI_CHAR, MPI_ANY_SOURCE, 10, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        printf("Ho ricevuto \n");
        for (int i = 0; i < 4; ++i) {
            printf("%c", messageRcv[i]);
        }
    }

    MPI_Finalize();
    return 0;
}
