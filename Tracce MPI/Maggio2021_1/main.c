#include <stdio.h>
#include <unistd.h>
#include "mpi.h"
#include "math.h"
#include "stdbool.h"

void readVector(char *fileName, int *vector, int row, int dim1, int dim2) {
    FILE *file = fopen(fileName, "r");
    for (int i = 0; i < dim1; ++i) {
        for (int j = 0; j < dim2; ++j) {
            if (row == i)
                fscanf(file, "%d ", &vector[j]);
            else {
                int temp;
                fscanf(file, "%d ", &temp);
            }
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

    int nProc, myRank, dim = 8;
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    MPI_Comm_size(MPI_COMM_WORLD, &nProc);

    int n = sqrt(nProc);

    int nDims = 2, myRankTopology, coords[2];
    int dims[2] = {n, n};
    int periods[2] = {true, true};
    MPI_Comm topologyComm;
    MPI_Cart_create(MPI_COMM_WORLD, nDims, dims, periods, false, &topologyComm);
    MPI_Comm_rank(topologyComm, &myRankTopology);
    MPI_Cart_coords(topologyComm, myRankTopology, nDims, &coords[0]);
    printf("Sono il processo %d e le mie coordinate sono (%d, %d)\n", myRank, coords[0], coords[1]);
    int v[dim];
    if (coords[0] == coords[1]) {
        readVector("vectors.txt", &v[0], coords[0], n, dim);
//        printMatrix(&v[0], 1, dim, myRankTopology);
    }

    int colorRow = coords[0];
    MPI_Comm rowComm;
    MPI_Comm_split(topologyComm, colorRow, 0, &rowComm);
    MPI_Bcast(&v[0], dim, MPI_INT, coords[0], rowComm);
    printMatrix(&v[0], 1, dim, myRank);

    MPI_Comm columnComm;
    int colorColumn = coords[1];
    MPI_Comm_split(topologyComm, colorColumn, 0, &columnComm);
    int dest, source;
    MPI_Cart_shift(topologyComm, 0, 1, &source, &dest);
    printf("Sono il processo %d e stampo %d processOnBottom e %d processOnTop \n", myRank, source, dest);
    int newV[dim];
    MPI_Sendrecv(&v[0], dim, MPI_INT, dest, 0,
                 &newV[0], dim, MPI_INT, MPI_ANY_SOURCE, 0,
                 topologyComm, MPI_STATUS_IGNORE);

    if (myRank == 0)
        printf("\n---------------------------------\n");

    sleep(1);
    MPI_Barrier(MPI_COMM_WORLD);
    printMatrix(&newV[0], 1, dim, myRankTopology);

    MPI_Finalize();

    return 0;
}
