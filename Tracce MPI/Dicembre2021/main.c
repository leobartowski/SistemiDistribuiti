#include <stdio.h>
#include <mpi.h>


void readMatrixFromFile(char *fileName, int *mat, int dim1, int dim2) {
    FILE *file = fopen(fileName, "r");
    for (int i = 0; i < dim1; ++i) {
        for (int j = 0; j < dim2; ++j) {
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


    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &nProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    MPI_Status status;

    int k = matrixDim / nProc;


    MPI_Datatype rowBlock;
    MPI_Type_vector(k, matrixDim, matrixDim * nProc, MPI_INT, &rowBlock);
    MPI_Type_commit(&rowBlock);

    if (myRank == 0) {
        int matrix[matrixDim][matrixDim];
        readMatrixFromFile("mat.txt", &matrix[0][0], matrixDim, matrixDim);
        printMatrix(&matrix[0][0], matrixDim, matrixDim, myRank);

        for (int i = 0; i < nProc; ++i) {
            MPI_Send(&matrix[i][0], 1, rowBlock, i, 0, MPI_COMM_WORLD);
        }
    }

    int matrixT[k][matrixDim];

    MPI_Recv(&matrixT[0][0], k * matrixDim, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
//    printMatrix(&matrixT[0][0], k, matrixDim, myRank);

    // Devo fare il prodotto riga per colonne
    int result[k];

    for (int i = 0; i < k; ++i) {
        result[i] = 0;
        for (int j = 0; j < matrixDim; ++j) {
            result[i] += matrixT[i][j] * matrixT[0][j];
        }
    }

    printMatrix(&result[0], 1, k, myRank);

    int finalResult[k];

    for (int i = 0; i < k; ++i) {
        MPI_Reduce(&result[i], &finalResult[i], 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
    }

    // Ho fatto una Reduce e non una AllReduce quindi solo il vettore con rank 0 ha il risultato!
    if (myRank == 0) {
        printMatrix(&finalResult[0], 1, k, myRank);
    }

    MPI_Finalize();

    return 0;
}
