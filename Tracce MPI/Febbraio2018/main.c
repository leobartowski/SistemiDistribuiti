#include <stdio.h>
#include <mpi.h>
#include "limits.h"
#include <unistd.h>

#define TAG_ARBITRARY 12345

// Leggi la matrice - Il primo parametro è il nome del file, il secondo è la matrice dove mettere i dati letti, il terzo e il quarto le dim della matrice
void readMatrixFromFile(char *fileName, int *mat, int dim1, int dim2) {
    FILE *fd = fopen(fileName, "r");
    for (int i = 0; i < dim1; i++) {
        for (int j = 0; j < dim2; j++) {
            fscanf(fd, "%d ", &mat[i * dim2 + j]);
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


// TODO: Usa nProc = 4 poiché dim/nProc deve essere un numero intero
int main(int argc, char *argv[]) {

    int myRank, nProc, dim = 8;
    int matrixA[dim][dim];

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &nProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    int k = dim / nProc;

    //! PUNTO 1 e 2
    if (myRank == 0) {
        readMatrixFromFile("mat.txt", &matrixA[0][0], dim, dim);
        printMatrix(&matrixA[0][0], dim, dim, myRank);
        MPI_Datatype column;
        MPI_Type_vector(dim, k, dim, MPI_INT, &column);
        MPI_Type_commit(&column);

        for (int i = 0; i < nProc; ++i) {
            MPI_Send(&matrixA[0][i * k], 1, column, i, 0, MPI_COMM_WORLD);
        }
    }

    int matrixTemp[dim][k];
    int matrixT[k][dim];

    MPI_Recv(&matrixTemp[0][0], k * dim, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    for (int i = 0; i < k; ++i) {
        for (int j = 0; j < dim; ++j) {
            matrixT[i][j] = matrixTemp[j][i];
        }
    }
    printMatrix(&matrixT[0][0], k, dim, myRank);


    //! PUNTO 3
    int sum_max = 0;
    for (int i = 0; i < k; i++) {
        int temp = matrixT[i][0];
        for (int j = 0; j < dim; j++) {
            if (matrixT[i][j] > temp) {
                temp = matrixT[i][j];
            }
        }
        sum_max += temp;
    }

    //! PUNTO 4
    int M[nProc], local[2], global[2];
    local[0] = sum_max;
    local[1] = myRank;

    for (int i = 0; i < nProc; ++i) {
        MPI_Allreduce(&local[0], &global[0], 1, MPI_2INT, MPI_MAXLOC, MPI_COMM_WORLD);
        M[i] = global[0];
        if (global[1] == myRank) {
            local[0] = INT_MIN;
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);
    sleep(1);

    if (myRank == 0) {
        printMatrix(&M[0], 1, nProc, myRank);
    }

    MPI_Finalize();
}

