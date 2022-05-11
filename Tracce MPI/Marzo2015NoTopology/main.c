#include <stdio.h>
#include <unistd.h>
#include "mpi.h"
#include "limits.h"

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

    int myRank, nProc, dim = 16, chunk = 2;
    int v[dim];

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &nProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    int k = dim / nProc;


    if (myRank == 0) {

        readVector("vector.txt", &v[0], dim);
        MPI_Datatype block;
        MPI_Type_vector(dim / (chunk * nProc), chunk, dim / chunk, MPI_INT, &block);
        MPI_Type_commit(&block);

        for (int i = 0; i < nProc; ++i) {
            MPI_Send(&v[i * chunk], 1, block, i, 0, MPI_COMM_WORLD);
        }
    }

    int v1[k];
    MPI_Recv(&v1[0], dim / nProc, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//    printMatrix(&v1[0], 1, k, myRank);

    for (int i = 0; i < k; ++i) {
        for (int j = i + 1; j < k; ++j) {
            if (v1[i] < v1[j]) {
                int temp = v1[i];
                v1[i] = v1[j];
                v1[j] = temp;
            }
        }
    }
//    printMatrix(&v1[0], 1, k, myRank);

    //! Punto 3

    int ranking[nProc];
    int local[2], global[2];
    local[0] = v1[0];
    local[1] = myRank;
    for (int i = 0; i < nProc; ++i) {
        MPI_Allreduce(&local[0], &global[0], 1, MPI_2INT, MPI_MAXLOC, MPI_COMM_WORLD);
        ranking[i] = global[1];
        if (global[1] == myRank) {
            local[0] = INT_MIN;
        }
    }

    sleep(1);
    MPI_Barrier(MPI_COMM_WORLD);
//    printMatrix(&ranking[0], 1, k, myRank);

    //! Punto 4
    for (int i = 0; i < nProc; ++i) {
        if (ranking[i] == myRank) {
            int dest = ranking[i + 2];
            if (dest > (nProc - 1)) {
                dest = ranking[i + 2 - nProc];
            }
            MPI_Send(&v1[0], k, MPI_INT, dest, 0, MPI_COMM_WORLD);
        }
    }

    sleep(1);
    MPI_Barrier(MPI_COMM_WORLD);

    int v2[k];
    MPI_Recv(&v2[0], k, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printMatrix(&v2[0], 1, k, myRank);

    //! Punto 5
    MPI_Send(&v2[0], k, MPI_INT, 0, 1, MPI_COMM_WORLD);

    if (myRank == 0) {
        int c[nProc][k];
        for (int i = 0; i < nProc; ++i) {
            MPI_Recv(&c[i][0], k, MPI_INT, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        printMatrix(&c[0][0], nProc, k, myRank);
    }
    MPI_Finalize();

    return 0;
}
