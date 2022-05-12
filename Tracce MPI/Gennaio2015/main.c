#include <stdio.h>
#include <math.h>
#include "mpi.h"
#include "stdbool.h"

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

    int myRank, nProc, dim = 8;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    MPI_Comm_size(MPI_COMM_WORLD, &nProc);

    int q = sqrt(nProc);
    int m[dim][dim];
    int k = dim / q;

    //! PUNTO 1
    MPI_Comm topologyComm;
    int myRankTopology;
    int nDims = 2;
    int dims[2] = {q, q};
    int periods[2] = {false, false};
    MPI_Cart_create(MPI_COMM_WORLD, nDims, dims, periods, false, &topologyComm);
    MPI_Comm_rank(topologyComm, &myRankTopology);
    int coords[2];
    MPI_Cart_coords(topologyComm, myRankTopology, nDims, &coords[0]);
//    printf("Sono il processo %d  e le mie coords sono (%d, %d) \n", myRank, coords[0], coords[1]);

    //! PUNTO 2
    if (myRank == 0) {
        readMatrix("mat.txt", &m[0][0], dim, dim);
        MPI_Datatype matrixBlock;
        MPI_Type_vector(k, k, dim, MPI_INT, &matrixBlock);
        MPI_Type_commit(&matrixBlock);

        for (int i = 0; i < nProc; ++i) {
            MPI_Send(&m[(i / q) * k][(i % q) * k], 1, matrixBlock, i, 0, MPI_COMM_WORLD);
        }
    }

    int a[k][k];
    MPI_Recv(&a[0][0], k * k, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printMatrix(&a[0][0], k, k, myRankTopology);


    //! PUNTO 3
    int aTemp[k][k];
    for (int i = 0; i < k; ++i) {
        for (int j = 0; j < k; ++j) {
            aTemp[i][j] = a[i][j];
        }

    }

    for (int i = 0; i < k; ++i) {
        for (int j = 0; j < k; ++j) {
            a[k - i][j] = aTemp[i][j];
        }
    }

    printMatrix(&a[0][0], k, k, myRankTopology);

    // !PUNTO 4

    // NON LO FARÒ



    MPI_Finalize();
    return 0;
}
