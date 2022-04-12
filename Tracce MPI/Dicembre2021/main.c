#include <stdio.h>
#include <mpi.h>


void readMatrixFromFile(char *fileName, int *mat, int dim1, int dim2) {
    FILE *file = fopen(fileName, "r");
    for (int i = 0; i < dim1; ++i) {
        for (int j = 0; j < dim2 ; ++j) {
            fscanf(file, "%d ", &mat[i * dim2 + j]);
        }
    }
}

void printMatrix(int *mat, int dim1, int dim2, int myRank) {
    printf("\nsono il processo %d e stampo la matrice \n", myRank);
    for (int i = 0; i < dim1; i++) {
        for (int j = 0; j < dim2; j++) {
            printf("%d ", mat[i * dim2 + j]);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {

    int myRank, nProc;
    int matrixDim = 8; // La matrice deve essere quadrata 8x8
    int matrix[matrixDim][matrixDim];

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &nProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    MPI_Status status;

    int k = matrixDim / nProc;

    MPI_Datatype datatype;

    if (myRank == 0) {
        readMatrixFromFile("mat.txt", &matrix[0][0], matrixDim, matrixDim);
        printMatrix(&matrix[0][0], matrixDim, matrixDim, myRank);

        for (int i = 0; i < nProc; ++i) {
            MPI_Send(&matrix[0][i * k], k, columns, i, MPI_ANY_TAG, MPI_COMM_WORLD);
        }
    }


    printf("Hello, World!\n");
    return 0;
}
