#include <stdio.h>
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
    printf("Sono il processo %d e stampo: \n", rank);
    for (int i = 0; i < dim1; ++i) {
        for (int j = 0; j < dim2; ++j) {
            printf("%d ", matrix[i * dim2 + j]);
        }
        printf("\n");
    }
}

void printMatrixFloat(float *matrix, int dim1, int dim2, int rank) {
    printf("Sono il processo %d e stampo: \n", rank);
    for (int i = 0; i < dim1; ++i) {
        for (int j = 0; j < dim2; ++j) {
            printf("%f ", matrix[i * dim2 + j]);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {

    MPI_Init(&argc, &argv);

    int nProc, myRank;
    int dim = 8;

    MPI_Comm_size(MPI_COMM_WORLD, &nProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    int k = dim / nProc;

    if (myRank == 0) {

        int a[dim][dim], b[dim][dim];
        readMatrix("matA.txt", &a[0][0], dim, dim);
        readMatrix("matB.txt", &b[0][0], dim, dim);

        MPI_Datatype columns;
        MPI_Type_vector(dim, k, dim, MPI_INT, &columns);
        MPI_Type_commit(&columns);
        for (int i = 0; i < nProc; ++i) {
            MPI_Send(&a[i * k][0], k * dim, MPI_INT, i, 1, MPI_COMM_WORLD);
            MPI_Send(&b[0][i * k], 1, columns, i, 2, MPI_COMM_WORLD);
        }
    }

    int t1[k][dim], t2[dim][k];
    MPI_Recv(&t1[0][0], k * dim, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&t2[0][0], k * dim, MPI_INT, 0, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//    printMatrix(&t1[0][0], k, dim, myRank);
//    printMatrix(&t2[0][0], dim, k, myRank);

    MPI_Barrier(MPI_COMM_WORLD);
    sleep(1);

    int c[k][k];
    for (int i = 0; i < k; ++i) {
        for (int j = 0; j < k; ++j) {
            c[i][j] = 0;
            for (int l = 0; l < dim; ++l) {
                c[i][j] += t1[i][l] * t2[l][j];
            }
        }
    }

    for (int i = 0; i < k; ++i) {
        for (int j = i + 1; j < k; ++j) {
            if (c[i][0] < c[j][0]) {
                int temp = c[i][0];
                c[i][0] = c[j][0];
                c[j][0] = temp;
            }
        }
    }
    printMatrix(&c[0][0], k, k, myRank);

    MPI_Barrier(MPI_COMM_WORLD);
    sleep(1);

    float c00 = c[0][0];
    float average;
    MPI_Allreduce(&c00, &average, 1, MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);
    average = average / nProc;

    if (myRank != 0) {
        float diff = c00 - average;
        MPI_Send(&diff, 1, MPI_FLOAT, 0, 5, MPI_COMM_WORLD);
    } else {
        float diff[nProc - 1];
        for (int i = 1; i < nProc; ++i) {
            MPI_Recv(&diff[i - 1], 1, MPI_FLOAT, i, 5, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        printMatrixFloat(&diff[0], 1, nProc - 1, myRank);
    }

    MPI_Finalize();
    return 0;
}
