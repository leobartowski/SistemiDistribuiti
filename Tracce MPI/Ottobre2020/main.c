#include <stdio.h>
#include "mpi.h"

void readMatrix(FILE *file, int *matrix, int dim1, int dim2) {
    for (int i = 0; i < dim1; ++i) {
        for (int j = 0; j < dim2; ++j) {
            fscanf(file, "%d", &matrix[i * dim2 + j]);
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

    int myRank, nProc, dim = 8;

    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    MPI_Comm_size(MPI_COMM_WORLD, &nProc);

    if (myRank == 0) {
        FILE *file = fopen("matrixs.txt", "r");
        for (int i = 0; i < nProc; ++i) {
            int a[dim][dim];
            readMatrix(file, &a[0][0], dim, dim);
            MPI_Datatype diagonal;
            MPI_Type_vector(dim, 1, dim + 1, MPI_INT, &diagonal);
            MPI_Type_commit(&diagonal);
            MPI_Send(&a[0][0], 1, diagonal, i, 0, MPI_COMM_WORLD);
        }
        fclose(file);
    }
    int v[dim];
    MPI_Recv(&v[0], dim, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
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
    printMatrix(&v[0], 1, dim, myRank);

    int vMax[dim];
    MPI_Reduce(&v[0], &vMax[0], dim, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
    if (myRank == 0)
        printMatrix(&vMax[0], 1, dim, myRank);

    MPI_Finalize();

    return 0;
}
