#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include "time.h"

void readMatrix(char *fileName, int *matrix, int dim1, int dim2) {
    FILE *file = fopen(fileName, "r");
    for (int i = 0; i < dim1; ++i) {
        for (int j = 0; j < dim2; ++j) {
            fscanf(file, "%d ", &matrix[i * dim2 + j]);
        }
    }
}

void printMatrix(int *matrix, int dim1, int dim2, int myRank) {
    printf("Sono il processo %d e stampo \n", myRank);
    for (int i = 0; i < dim1; ++i) {
        for (int j = 0; j < dim2; ++j) {
            printf("%d ", matrix[i * dim2 + j]);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {

    MPI_Init(&argc, &argv);

    int nProc, myRank, k = 8;

    MPI_Comm_size(MPI_COMM_WORLD, &nProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    int a[k][nProc];

    srand(myRank * time(0));
    int randomNumber = rand() % 100;
    int local[2], global[2];
    local[0] = randomNumber;
    local[1] = myRank;
    printf("Sono il processo %d e ho estratto %d \n", myRank, randomNumber);

    MPI_Allreduce(&local[0], &global[0], 1, MPI_2INT, MPI_MAXLOC, MPI_COMM_WORLD);

    if (global[1] == myRank) { // Sono il processo che ha estratto il massimo -> PIVOT

        readMatrix("matA.txt", &a[0][0], k, nProc);
//        printMatrix(&a[0][0], k, nProc, myRank);

        MPI_Datatype column;
        MPI_Type_vector(k, 1, nProc, MPI_INT, &column);
        MPI_Type_commit(&column);

        for (int i = 0; i < nProc; ++i) {

            int dest = (myRank + i < nProc) ? myRank + i : myRank + i - nProc;
            MPI_Send(&a[0][i], 1, column, dest, 0, MPI_COMM_WORLD);
        }
    }

    int v[k];

    MPI_Recv(&v[0], k, MPI_INT, global[1], 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//    printMatrix(&v[0], k, 1, myRank);

    //! Punto 4
    int color = (myRank > global[1]) ? 1 : 0;
    MPI_Comm newComm;
    MPI_Comm_split(MPI_COMM_WORLD, color, 0, &newComm);

    int result[k];


    if (myRank > global[1]) {

        // Ordino in senso crescente
        for (int i = 0; i < k; ++i) {
            for (int j = i + 1; j < k; ++j) {
                if (v[i] > v[j]) {
                    int temp = v[i];
                    v[i] = v[j];
                    v[j] = temp;
                }
            }
        }
        printMatrix(&v[0], k, 1, myRank);

        MPI_Allreduce(&v[0], &result[0], k, MPI_INT, MPI_SUM, newComm);
        MPI_Barrier(newComm);
        MPI_Send(&result[0], k, MPI_INT, global[1], 5, MPI_COMM_WORLD);

    } else {
        // Ordino in senso de-crescente
        for (int i = 0; i < k; ++i) {
            for (int j = i + 1; j < k; ++j) {
                if (v[i] < v[j]) {
                    int temp = v[i];
                    v[i] = v[j];
                    v[j] = temp;
                }
            }
        }
        printMatrix(&v[0], k, 1, myRank);

        MPI_Allreduce(&v[0], &result[0], k, MPI_INT, MPI_PROD, newComm);
        MPI_Barrier(newComm);
        MPI_Send(&result[0], k, MPI_INT, global[1], 10, MPI_COMM_WORLD);
    }

    if (myRank == global[1]) {
        int sum[k];
        int prod[k];
        MPI_Request requestSum, requestProd;
        int readySum, readyProd;

        MPI_Irecv(&sum[0], k, MPI_INT, MPI_ANY_SOURCE, 5, MPI_COMM_WORLD, &requestSum);
        MPI_Irecv(&prod[0], k, MPI_INT, MPI_ANY_SOURCE, 10, MPI_COMM_WORLD, &requestProd);

        MPI_Test(&requestSum, &readySum, MPI_STATUS_IGNORE);
        if(readySum)
            printMatrix(&sum[0], k, 1, myRank);

        MPI_Test(&requestProd, &readyProd, MPI_STATUS_IGNORE);
        if(readyProd)
            printMatrix(&prod[0], k, 1, myRank);

    }


    MPI_Finalize();
    return 0;
}
