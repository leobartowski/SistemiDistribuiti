#include <stdio.h>
#include <unistd.h>
#include "mpi.h"

void readMatrix(FILE *file, int *matrix, int dim1, int dim2) {

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


int main() {

    MPI_Init(NULL, NULL);
    int dim = 8, nProc, myRank;

    MPI_Comm_size(MPI_COMM_WORLD, &nProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    if (myRank == 0) {
        FILE *file = fopen("matrics.txt", "r");
        int matrics[nProc][dim][dim];
        for (int i = 0; i < nProc; ++i) {
            readMatrix(file, &matrics[i][0][0], dim, dim);
            printMatrix(&matrics[i][0][0], dim, dim, myRank);

            MPI_Datatype diagonal;
            MPI_Type_vector(dim, 1, dim + 1, MPI_INT, &diagonal);
            MPI_Type_commit(&diagonal);
            MPI_Send(&matrics[i][0][0], 1, diagonal, i, 0, MPI_COMM_WORLD);
        }
    }

    int v[dim];
    MPI_Recv(&v[0], dim, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//    printMatrix(v[0], 1, dim, myRank);

    for (int i = 0; i < dim; ++i) {
        for (int j = i + 1; j < dim; ++j) {
            if (v[i] < v[j]) {
                int temp = v[i];
                v[i] = v[j];
                v[j] = temp;
            }
        }
    }

    sleep(1);
    MPI_Barrier(MPI_COMM_WORLD);
    printMatrix(&v[0], 1, dim, myRank);

    MPI_Comm newComm;
    int max, min;
    int color = myRank % 2 == 0 ? 0 : 1;
    MPI_Comm_split(MPI_COMM_WORLD, color, 0, &newComm);

    if (color == 0) {
        MPI_Reduce(&v[0], &max, 1, MPI_INT, MPI_MAX, 0, newComm);
    } else {
        // Uso come root 0 poichè so che il processo che ha rango 1 in MPI_COMM_WORLD avrà 0 nel nuovo (essendo il più piccolo)
        MPI_Reduce(&v[dim - 1], &min, 1, MPI_INT, MPI_MIN, 0, newComm);
    }

    if (myRank == 0)
        printf("Sono il processo %d e il massimo è %d \n", myRank, max);
    if (myRank == 1)
        printf("Sono il processo %d e il minimo è %d \n", myRank, min);


    MPI_Finalize();
    return 0;
}
