#include <stdio.h>
#include <math.h>
#include "mpi.h"

void printMatrix(int *matrix, int dim1, int dim2, int myRank) {
    printf("Sono il processo %d e stampo: \n", myRank);
    for (int i = 0; i < dim1; ++i) {
        for (int j = 0; j < dim2; ++j) {
            printf("%d ", matrix[i * dim2 + j]);
        }
        printf("\n");
    }
}

void readMatrix(char *fileName, int *matrix, int dim1, int dim2) {
    FILE *file = fopen(fileName, "r");
    for (int i = 0; i < dim1; ++i) {
        for (int j = 0; j < dim2; ++j) {
            fscanf(file, "%d ", &matrix[i * dim2 + j]);
        }
    }
}

int maxColumn(int *matrix, int dim, int column) {
    int max = matrix[column];

    for (int i = 0; i < dim; ++i) {
        if (matrix[i * dim + column] > max) {
            max = matrix[i * dim + column];
        }
    }
    return max;
}

int main(int argc, char *argv[]) {


    MPI_Init(&argc, &argv);

    int myRank, nProc, dim = 8;
    MPI_Comm_size(MPI_COMM_WORLD, &nProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    MPI_Datatype column;
    MPI_Type_vector(dim, 1, dim, MPI_INT, &column);
    MPI_Type_commit(&column);

    int a[dim][dim];
    int columnATemp[dim];
    int t[dim][dim];


    if (myRank == 0) {
        readMatrix("matA.txt", &a[0][0], dim, dim);
//        printMatrix(&a[0][0], dim, dim, myRank);
    }

    for (int i = 0; i < dim; ++i) {

        if (myRank == 0) {

            MPI_Send(&a[0][i], 1, column, myRank + 1, i, MPI_COMM_WORLD);

        } else {

            MPI_Recv(&columnATemp[0], dim, MPI_INT, myRank - 1, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if (myRank != nProc - 1) {
                MPI_Send(&columnATemp[0], dim, MPI_INT, myRank + 1, i, MPI_COMM_WORLD);
            }

            for (int j = 0; j < dim; ++j) {
                t[j][i] = columnATemp[j] * (pow(2, myRank) + i);
            }
        }
    }

    int color = 0;
    if (myRank != 0) {
        color = 1;
//        printMatrix(&t[0][0], dim, dim, myRank);
    }


    MPI_Comm everyoneExcludedFirst;
    MPI_Comm_split(MPI_COMM_WORLD, color, 0, &everyoneExcludedFirst);

    if (myRank == 1)
        t[0][3] = 10000;

    //! Punto 4
    for (int j = 0; j < dim; ++j) { // Scorro tra le colonne

        if (myRank != 0) {

            int maxSpecificColumn = maxColumn(&t[0][0], dim, j);
            int local[2], global[2] = {0};
            local[0] = maxSpecificColumn;
            local[1] = myRank;
            MPI_Allreduce(&local, &global, 1, MPI_2INT, MPI_MAXLOC, everyoneExcludedFirst);

            if (global[1] == myRank) {
                printf("Sono il processo %d, ho vinto il gioco con il seguente massimo: %d \n", global[1], global[0]);
                MPI_Send(&t[0][j], 1, column, 0, 10 + j, MPI_COMM_WORLD);
            }

        } else {

            MPI_Recv(&t[0][j], 1, column, MPI_ANY_SOURCE, 10 + j, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    }

    if (myRank == 0) {

        printMatrix(&t[0][0], dim, dim, myRank);
    }

    MPI_Finalize();
    return 0;
}
