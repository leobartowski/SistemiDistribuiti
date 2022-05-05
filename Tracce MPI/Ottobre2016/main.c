#include <stdio.h>
#include <unistd.h>
#include "mpi.h"
#include "limits.h"
#include "string.h"

void readVector(char *fileName, int *vector, int dim) {
    FILE *file = fopen(fileName, "r");
    for (int i = 0; i < dim; ++i) {
        fscanf(file, "%d ", &vector[i]);
    }
}

void printMatrix(int *matrix, int dim1, int dim2, int myRank) {
    printf("Sono il processo %d e stampo \n", myRank);
    for (int i = 0; i < dim1; ++i) {
        for (int j = 0; j < dim2; ++j) {
            printf("%d ", matrix[i * dim2 + j]);
        }
        printf("\n");
    }
}


int main(int argc, char *argv[]) {

    MPI_Init(&argc, &argv);

    int myRank, nProc;
    int dim = 16;

    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    MPI_Comm_size(MPI_COMM_WORLD, &nProc);

    int k = dim / nProc;
    int a[dim];

    if (myRank == 0) {

        readVector("vector.txt", &a[0], dim);
//        printMatrix(&a[0], 1, dim, myRank);

        for (int i = 0; i < nProc; ++i) {
            MPI_Send(&a[i * k], k, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    }

    int v[k];

    MPI_Recv(&v[0], k, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//    printMatrix(&v[0], 1, k, myRank);

    //! Punto 2

    for (int i = 0; i < k; ++i) {
        for (int j = i + 1; j < k; ++j) {
            if (v[i] > v[j]) {
                int temp = v[i];
                v[i] = v[j];
                v[j] = temp;
            }
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);
    sleep(1);
//    printMatrix(&v[0], 1, k, myRank);

    //! Punto 3

    int vResult[k];
    int color = (myRank % 2) == 0 ? 0 : 1; // color 0 per i pari e 1 per i dispari
    MPI_Comm newComm;

    MPI_Comm_split(MPI_COMM_WORLD, color, 0, &newComm);
    MPI_Allreduce(&v[0], &vResult[0], k, MPI_INT, (color == 0) ? MPI_SUM : MPI_PROD, newComm);
//    printMatrix(&vResult[0], 1, k, myRank);

    //! Punto 4
    if (myRank == 0 || myRank == 1) {
        MPI_Send(&vResult[0], k, MPI_INT, nProc - 1, 5, MPI_COMM_WORLD);
    }

    if (myRank == (nProc - 1)) {
        int vSum[k], vProd[k];
        MPI_Recv(&vSum[0], k, MPI_INT, 0, 5, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&vProd[0], k, MPI_INT, 1, 5, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

//        printMatrix(&vSum[0], 1, k, myRank);
//        printMatrix(&vProd[0], 1, k, myRank);
    }

    //! Punto 5
    //* RICORDA CHE DEVI SEMPRE CREARE LOCAL QUANDO USI MAXLOC
    int local[2];
    local[0] = v[0];
    local[1] = myRank;
    int global[2] = {0};

    printMatrix(&v[0], 1, k, myRank);
    MPI_Barrier(MPI_COMM_WORLD);
    sleep(1);

    MPI_Allreduce(&local[0], &global[0], 1, MPI_2INT, MPI_MAXLOC, MPI_COMM_WORLD);

    if (myRank == global[1]) {
        printf("Sono il processo %d e posseggo il massimo in v[0] \n", myRank);
    }

    MPI_Finalize();
    return 0;
}
