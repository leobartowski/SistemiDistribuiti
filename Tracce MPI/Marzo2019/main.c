#include <stdio.h>
#include <math.h>
#include "mpi.h"
#include "limits.h"
#include "stdbool.h"

void readMatrix(char *fileName, int *matrix, int dim1, int dim2) {
    FILE *file = fopen(fileName, "r");
    for (int i = 0; i < dim1; ++i) {
        for (int j = 0; j < dim2; ++j) {
            fscanf(file, "%d ", &matrix[i * dim2 + j]);
        }
    }
}

void printMatrix(int *matrix, int dim1, int dim2, int myRank) {
    printf("Sono il processo %d \n", myRank);
    for (int i = 0; i < dim1; ++i) {
        for (int j = 0; j < dim2; ++j) {
            printf("%d ", matrix[i * dim2 + j]);
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

    int n = sqrt(nProc);
    int k = dim / n;
    int a[dim][dim], b[dim][dim];

    MPI_Comm topologyComm;
    int dims[2] = {n, n};
    int periods[2] = {false, false};

    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, false, &topologyComm);
    int myRankTopology;
    MPI_Comm_rank(topologyComm, &myRankTopology);
    int coords[2];
    MPI_Cart_coords(topologyComm, myRankTopology, n, coords);
//    printf("Sono il processo %d e le mie coordinate sono (%d, %d) \n", myRankTopology, coords[0], coords[1]);

    //! Punto 2 e 3
    if (myRank == 0) {

        MPI_Datatype matrixBlock, bigColumn;
        readMatrix("matA.txt", &a[0][0], dim, dim);
        readMatrix("matB.txt", &b[0][0], dim, dim);

//        printMatrix(&a[0][0], dim, dim, myRankTopology);
//        printMatrix(&b[0][0], dim, dim, myRankTopology);

        MPI_Type_vector(k, k, dim, MPI_INT, &matrixBlock);
        MPI_Type_commit(&matrixBlock);

        MPI_Type_vector(dim, k, dim, MPI_INT, &bigColumn);
        MPI_Type_commit(&bigColumn);

        for (int i = 0; i < nProc; ++i) {
            MPI_Send(&a[(i / n) * k][(i % n) * k], 1, matrixBlock, i, 10, topologyComm);
        }

        int currentRow = 0;
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                MPI_Send(&b[0][i * k], 1, bigColumn, currentRow + j, 15, topologyComm);
            }
            currentRow += n;
        }
    }

    int t[k][k], p[dim][k];

    MPI_Recv(&t[0][0], k * k, MPI_INT, 0, 10, topologyComm, MPI_STATUS_IGNORE);
//    printMatrix(&t[0][0], k, k, myRankTopology);

    MPI_Recv(&p[0][0], dim * k, MPI_INT, 0, 15, topologyComm, MPI_STATUS_IGNORE);
//    printMatrix(&p[0][0], dim, k, myRankTopology);

    //! Punto 4
    // ** La prova è errata, forse di doveva trasporre P ???


    MPI_Finalize();


    return 0;
}
