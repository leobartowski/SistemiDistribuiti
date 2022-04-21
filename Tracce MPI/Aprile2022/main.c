#include <stdio.h>
#include "mpi.h"

void readMatrix(char *fileName, int *matrix, int dim1, int dim2) {
    FILE *file = fopen(fileName, "r");

    for (int i = 0; i < dim1 ; ++i) {
        for (int j = 0; j < dim2 ; ++j) {
            fscanf(file, "%d ", &matrix[i * dim2 + j]);
        }
    }
}

void printMatrix(int *matrix, int dim1, int dim2, int myRank) {
    printf("\nsono il processo %d e stampo la matrice: \n", myRank);
    for (int i = 0; i < dim1; ++i) {
        for (int j = 0; j < dim2; ++j) {
            printf("%d ", matrix[i * dim2 + j]);
        }
        printf("\n");
    }
}


int main(int argc, char *argv[]) {

    int myRank, nProc;
    int matrixDim = 8;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    MPI_Comm_size(MPI_COMM_WORLD, &nProc);
    MPI_Status status;

    int k = matrixDim / nProc;

    MPI_Datatype column, columnBlock;
    MPI_Type_vector(matrixDim, k, matrixDim, MPI_INT, &column);
    MPI_Type_create_resized(column, 0, k * (sizeof(int)) * nProc, &columnBlock);
    MPI_Type_commit(&columnBlock);

    if (myRank == 0) {
        int startMatrix[matrixDim][matrixDim];
        readMatrix("mat.txt", &startMatrix[0][0], matrixDim, matrixDim);
        printMatrix(&startMatrix[0][0], matrixDim, matrixDim, myRank);
        for (int i = 0; i < nProc; ++i) {
            MPI_Send(&startMatrix[0][0], 1, columnBlock, i, 0, MPI_COMM_WORLD);
        }
    }

    int matrixT[matrixDim][k];

    MPI_Recv(&matrixT[0][0], k * matrixDim, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

    printMatrix(&matrixT[0][0], matrixDim, k, myRank);

    MPI_Finalize();
    return 0;
}