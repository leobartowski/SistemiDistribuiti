#include <stdio.h>
#include <unistd.h>
#include "mpi.h"

void readVector(char *fileName, int *vector, int dim1) {
    FILE *file = fopen(fileName, "r");
    for (int i = 0; i < dim1; ++i) {
        fscanf(file, "%d ", &vector[i]);
    }
}

void printMatrix(int *matrix, int dim1, int dim2, int myRank) {

    printf("Sono il processo %d e stampo questa matrice: \n", myRank);
    for (int i = 0; i < dim1; ++i) {
        for (int j = 0; j < dim2; ++j) {
            printf("%d", matrix[i * dim2 + j]);
        }
        printf(" \n");
    }
}

int main(int argc, char *argv[]) {

    MPI_Init(&argc, &argv);

    int dim = 16;
    int myRank, nProc, vector[16] = {0};

    MPI_Comm_size(MPI_COMM_WORLD, &nProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    MPI_Status status;
    MPI_Request request;

    int k = dim / nProc;

    if (myRank == 0) {
        readVector("vector.txt", &vector[0], 16);
        printMatrix(&vector[0], 16, 1, myRank);
        for (int i = 0; i < nProc; ++i) {
            MPI_Send(&vector[i * k], k, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    }
    int v[k];
    MPI_Recv(&v, k, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
//    printMatrix(&v[0], k, 1, myRank);

//! Improve sort!!!
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < k; j++) {
            if (v[j] > v[i]) {
                int tmp = v[i];
                v[i] = v[j];
                v[j] = tmp;
            }
        }
    }
//    printMatrix(&v[0], k, 1, myRank);

    MPI_Comm newComm;
    MPI_Group posGroup, negGroup;
    int color =  v[k - 1] >= 0 ? 0 : 1, myRankNewComm;
    MPI_Comm_split(MPI_COMM_WORLD, color, 0, &newComm);
    MPI_Comm_rank(newComm,&myRankNewComm);

    if (color == 0) {
        int max = 0;

        MPI_Comm_group(newComm, &posGroup); // serve a qualcosa? No
        MPI_Reduce(&v[k - 1], &max, 1, MPI_INT, MPI_MAX, 0, newComm);
        if (myRankNewComm == 0) {
            MPI_Send(&max, 1, MPI_INT, 0, 5, MPI_COMM_WORLD);
        }
        printf("Sono il processo %d la reduce positiva mi da %d \n", myRank, max);


    } else {
        int min = 0;

        MPI_Comm_group(newComm, &negGroup); // serve a qualcosa? No

        MPI_Reduce(&v[k - 1], &min, 1, MPI_INT, MPI_MIN, 0, newComm);
        if (myRankNewComm == 0) {
            MPI_Send(&min, 1, MPI_INT, 0, 10, MPI_COMM_WORLD);
        }
        printf("Sono il processo %d la reduce negativa mi da %d \n", myRank, min);
    }
    MPI_Barrier(MPI_COMM_WORLD);

    if (myRank == 0) {
        int maxTot, minTot;

        MPI_Recv(&maxTot, 1, MPI_INT, MPI_ANY_SOURCE, 5, MPI_COMM_WORLD, &status);
        MPI_Recv(&minTot, 1, MPI_INT, MPI_ANY_SOURCE, 10, MPI_COMM_WORLD, &status);
        printf("Sono il processo %d e stampo il massimo %d \n", myRank, maxTot);
        printf("Sono il processo %d e stampo il minimo %d \n", myRank, minTot);
    }

    MPI_Finalize();

    return 0;
}
