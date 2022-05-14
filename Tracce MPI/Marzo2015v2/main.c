#include <stdio.h>
#include <unistd.h>
#include "mpi.h"
#include "limits.h"

void readVector(char *fileName, int *matrix, int dim) {
    FILE *file = fopen(fileName, "r");
    for (int i = 0; i < dim; ++i) {
        fscanf(file, "%d ", &matrix[i]);
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

    int dim = 16, chunk = 2, myRank, nProc;
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    MPI_Comm_size(MPI_COMM_WORLD, &nProc);

    int k = dim / nProc;

    if (myRank == 0) {
        int a[dim];
        readVector("vector.txt", &a[0], dim);
        MPI_Datatype blocks;
        MPI_Type_vector(k / chunk, chunk, nProc * chunk, MPI_INT, &blocks);
        MPI_Type_commit(&blocks);
        for (int i = 0; i < nProc; ++i) {
            MPI_Send(&a[i * chunk], 1, blocks, i, 5, MPI_COMM_WORLD);
        }
    }

    int t[k];
    MPI_Recv(&t[0], k, MPI_INT, 0, 5, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    // Ordino il vettore in maniera decrescente
    for (int i = 0; i < k; ++i) {
        for (int j = i + 1; j < k; ++j) {
            if (t[i] < t[j]) {
                int temp = t[i];
                t[i] = t[j];
                t[j] = temp;
            }
        }
    }
    printMatrix(&t[0], 1, k, myRank);
    MPI_Barrier(MPI_COMM_WORLD);
    sleep(1);

    int ranking[nProc], myPositionInTheRanking;
    int local[2], global[2];
    local[0] = t[0];
    local[1] = myRank;
    for (int i = 0; i < nProc; ++i) {
        MPI_Allreduce(&local[0], &global[0], 1, MPI_2INT, MPI_MAXLOC, MPI_COMM_WORLD);
        ranking[i] = global[1];
        if (global[1] == myRank) {
            local[0] = INT_MIN;
            myPositionInTheRanking = i;
        }
    }

//    printMatrix(&ranking[0], 1, nProc, myRank);
    MPI_Barrier(MPI_COMM_WORLD);
    sleep(1);

    int dest = ranking[(myPositionInTheRanking + 2) % nProc], newT[k];
    MPI_Sendrecv(&t[0], k, MPI_INT, dest, 1,
                 &newT[0], k, MPI_INT, MPI_ANY_SOURCE, 1,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    printMatrix(&newT[0], 1, k, myRank);
    MPI_Barrier(MPI_COMM_WORLD);
    sleep(1);

    if (myRank != 0) {
        MPI_Send(&newT[0], k, MPI_INT, 0, 10, MPI_COMM_WORLD);
    } else {
        int mat[nProc - 1][k];
        for (int i = 1; i < nProc; ++i) {
            MPI_Recv(&mat[i - 1][0], k, MPI_INT, i, 10, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        printMatrix(&mat[0][0], nProc - 1, k, myRank);

    }

    MPI_Finalize();
    return 0;
}
