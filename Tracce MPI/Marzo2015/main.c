#include <stdio.h>
#include <unistd.h>
#include "limits.h"
#include "mpi.h"
#include "stdbool.h"

void printMatrix(int *matrix, int dim1, int dim2, int myRank) {
    printf("Sono il processo %d e stampo la matrice: \n", myRank);
    for (int i = 0; i < dim1; ++i) {
        for (int j = 0; j < dim2; ++j) {
            printf("%d ", matrix[i * dim2 + j]);
        }
        printf("\n");
    }
}

void readVector(char *fileName, int *vector, int dim) {
    FILE *file = fopen(fileName, "r");
    for (int i = 0; i < dim; ++i) {
        fscanf(file, "%d ", &vector[i]);
    }
}


int main(int argc, char *argv[]) {

    int myRank, nProc;
    int dim = 16;
    int chuck = 2;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &nProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    int k = dim / nProc;
    int vector[dim];
    int v1[k];

    if (myRank == 0) {
        readVector("vector.txt", &vector[0], dim);
//        printMatrix(&vector[0], 1, dim, myRank);

        MPI_Datatype block;
        MPI_Type_vector(k / chuck, chuck, nProc * chuck, MPI_INT, &block);
        MPI_Type_commit(&block);

        for (int i = 0; i < nProc; ++i) {
            MPI_Send(&vector[i * chuck], 1, block, i, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Recv(&v1[0], k, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//    printMatrix(&v1[0], 1, k, myRank);
//    sleep(1);

    //! Punto 2
    int temp;
    for (int i = 0; i < k; ++i) {
        for (int j = i + 1; j < k; ++j) {
            if (v1[i] < v1[j]) {
                temp = v1[i];
                v1[i] = v1[j];
                v1[j] = temp;
            }
        }
    }
    printMatrix(&v1[0], 1, k, myRank);

    // !Punto 3

    int local[2], global[2], rankings[nProc];
    local[0] = v1[0];
    local[1] = myRank;

    for (int i = 0; i < nProc; ++i) {
        MPI_Allreduce(&local, &global, 1, MPI_2INT, MPI_MAXLOC, MPI_COMM_WORLD);
        rankings[i] = global[1];
        if (myRank == global[1]) {
            local[0] = INT_MIN;
        }
    }
//    printMatrix(&rankings[0], 1, nProc, myRank);

    //! Punto 4

    MPI_Group worldGroup, orderedGroup;
    MPI_Comm orderedComm, topologyComm;
    MPI_Comm_group(MPI_COMM_WORLD, &worldGroup);
    MPI_Group_incl(worldGroup, nProc, rankings, &orderedGroup);
    MPI_Comm_create_group(MPI_COMM_WORLD, orderedGroup, 0, &orderedComm);

    int myRankOrdered;
    MPI_Comm_rank(orderedComm, &myRankOrdered);
    printf("Rank originale: %d e nuovo Rank %d \n", myRank, myRankOrdered);

    int dims[4] = {0};
    int periods[1] = {true};

    MPI_Dims_create(nProc, 1, dims);
    MPI_Cart_create(orderedComm, 1, dims, periods, false, &topologyComm);

    int myRankTopology;
    MPI_Comm_rank(topologyComm, &myRankTopology);

    int coords[1];
    MPI_Cart_coords(topologyComm, myRankTopology, 1, coords);
    int leftRank, rightRank;
    MPI_Cart_shift(topologyComm, 0, 2, &leftRank, &rightRank);

    int v2[k];
    MPI_Sendrecv(&v1[0], k, MPI_INT, leftRank, 0, &v2[0], k, MPI_INT, MPI_ANY_SOURCE, 0, topologyComm,
                 MPI_STATUS_IGNORE);

    printMatrix(&v2[0], 1, k, myRankTopology);

    //! Punto 5
    sleep(1);
    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Send(&v2[0], k, MPI_INT, 0, 15, MPI_COMM_WORLD);

    int c[nProc][k];

    if (myRank == 0) {
        for (int i = 0; i < nProc; ++i) {
            MPI_Recv(&c[i][0], k, MPI_INT, i, 15, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        printMatrix(&c[0][0], nProc, k, myRank);
    }

    MPI_Finalize();
    return 0;
}
