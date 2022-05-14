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

    int dim = 8, nProc, myRank;

    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    MPI_Comm_size(MPI_COMM_WORLD, &nProc);

    int k = dim / nProc;

    if (myRank == 0) {
        int a[dim][dim];
        readMatrix("matA.txt", &a[0][0], dim, dim);

        MPI_Datatype column;
        MPI_Type_vector(k * dim, 1, nProc, MPI_INT, &column);
        MPI_Type_commit(&column);

        for (int i = 0; i < nProc; ++i) {
            MPI_Send(&a[0][i], 1, column, i, 0, MPI_COMM_WORLD);
        }
    }

    int tColumn[dim][k], t[k][dim];
    MPI_Recv(&tColumn[0][0], k * dim, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    for (int i = 0; i < k; ++i) {
        for (int j = 0; j < dim; ++j) {
            t[i][j] = tColumn[j][i];
        }
    }

    printMatrix(&t[0][0], k, dim, myRank);
    int max[dim];
    MPI_Allreduce(&t[0][0], &max[0], dim, MPI_INT, MPI_MAX, MPI_COMM_WORLD);
    printMatrix(&max[0], 1, dim, myRank);

    MPI_Finalize();

    return 0;
}
