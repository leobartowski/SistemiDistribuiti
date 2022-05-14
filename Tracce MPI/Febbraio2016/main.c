#include <stdio.h>
#include <unistd.h>
#include "mpi.h"

void readMatrics(FILE *file, int *matrix, int dim1, int dim2) {

    for (int i = 0; i < dim1; ++i) {
        for (int j = 0; j < dim2; ++j) {
            fscanf(file, "%d ", &matrix[i * dim2 + j]);
        }
    }
}

void printMatrix(int *matrix, int dim1, int dim2, int rank) {
    printf("Sono il processo %d e stampo: \n", rank);
    for (int i = 0; i < dim1; ++i) {
        for (int j = 0; j < dim2; ++j) {
            printf("%d ", matrix[i * dim2 + j]);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {

    MPI_Init(&argc, &argv);

    int dim = 8, myRank, nProc;

    MPI_Comm_size(MPI_COMM_WORLD, &nProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    int arrayOfMatrices[nProc][dim][dim];

    if (myRank == 0) {
        FILE *file = fopen("matrixs.txt", "r");
        for (int i = 0; i < nProc; ++i) {
            readMatrics(file, &arrayOfMatrices[i][0][0], dim, dim);
//            printMatrix(&arrayOfMatrices[i][0][0], dim, dim, myRank);
            int diagonal[dim];
            for (int j = 0; j < dim; ++j) {
                diagonal[j] = arrayOfMatrices[i][j][j];
            }
            MPI_Send(&diagonal[0], dim, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    }
    int v[dim];
    MPI_Recv(&v[0], dim, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    sleep(1);
//    printMatrix(&v[0], 1, dim, myRank);

    for (int i = 0; i < dim; ++i) {
        for (int j = i + 1; j < dim; ++j) {
            if (v[i] < v[j]) {
                int temp = v[i];
                v[i] = v[j];
                v[j] = temp;
            }
        }
    }
    MPI_Barrier(MPI_COMM_WORLD);
    sleep(1);
    printMatrix(&v[0], 1, dim, myRank);
    int color = 1, myNewRank;
    if (v[dim - 1] > 0 || myRank == 0) {
        color = 2;
    }
    MPI_Comm newComm;
    MPI_Comm_split(MPI_COMM_WORLD, color, 0, &newComm);
    MPI_Comm_rank(newComm, &myNewRank);

    if (color == 2) {
        int vMax[dim];
        MPI_Reduce(&v[0], &vMax[0], dim, MPI_INT, MPI_MAX, 0, newComm);

        MPI_Barrier(newComm);
        sleep(1);
        if (myRank == 0) {
            printMatrix(&vMax[0], 1, dim, myRank);
        }
    }

    MPI_Finalize();
    return 0;
}
