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
    MPI_Aint lb, extent;

    int k = matrixDim / nProc;

    MPI_Datatype column, columnBlock;
    MPI_Type_vector(k * matrixDim, 1, nProc, MPI_INT, &column);
    MPI_Type_create_resized(column, lb, sizeof(int), &columnBlock);
    MPI_Type_commit(&columnBlock);

    int startMatrix[matrixDim][matrixDim];

    if (myRank == 0) {
        readMatrix("mat.txt", &startMatrix[0][0], matrixDim, matrixDim);
        printMatrix(&startMatrix[0][0], matrixDim, matrixDim, myRank);
    }

    int matrixT[matrixDim][k];
    MPI_Scatter(&startMatrix[0][0], 1, columnBlock, &matrixT[0][0], k * matrixDim, MPI_INT, 0, MPI_COMM_WORLD);

//    printMatrix(&matrixT[0][0], matrixDim, k, myRank);

    MPI_Barrier(MPI_COMM_WORLD);

    //! Punto 2
    int maxVector[matrixDim];

    for (int i = 0; i < matrixDim; ++i) {
        maxVector[i] = matrixT[i][0];
        for (int j = 0; j < k; ++j) {

            if (matrixT[i][j] > maxVector[i]) {
                maxVector[i] = matrixT[i][j];
            }
        }
    }
//    printMatrix(&maxVector[0], matrixDim, 1, myRank);

    //! Punto 3
    int vectorV0[nProc];

    // Con la Allgather ogni processo invia il suo valore di vi maxVector[0]
    // e ogni processo riceve la
    int valueToSend = maxVector[0];
    MPI_Allgather(&valueToSend, 1, MPI_INT, &vectorV0, 1, MPI_INT, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    printMatrix(&vectorV0[nProc], nProc, 1, myRank);

    MPI_Finalize();
    return 0;
}