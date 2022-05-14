#include <stdio.h>
#include <unistd.h>
#include "mpi.h"

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

    int dim = 8, nProc, myRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    MPI_Comm_size(MPI_COMM_WORLD, &nProc);
    int k = dim / nProc;

    if (myRank == 0) {
        int a[dim];
        readVector("vet.txt", &a[0], dim);
        for (int i = 0; i < nProc; ++i) {
            MPI_Send(&a[i * k], k, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    }

    int v[k];
    MPI_Recv(&v[0], k, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    // Ordina in senso crescente
    for (int i = 0; i < k; ++i) {
        for (int j = i + 1; j < k; ++j) {
            if (v[i] > v[j]) {
                int temp = v[j];
                v[j] = v[i];
                v[i] = temp;
            }
        }
    }
    MPI_Barrier(MPI_COMM_WORLD);
    sleep(1);
    printMatrix(&v[0], 1, k, myRank);

    int color = v[k - 1] >= 0 ? 1 : 2;
    MPI_Comm newComm;
    int myNewRank;
    MPI_Comm_split(MPI_COMM_WORLD, color, 0, &newComm);
    MPI_Comm_rank(newComm, &myNewRank);
    // I processi sono ordinati quindi per i positivi il massimo è l'ultimo elemento per i negativi il minimo è il primo elemento
    int valueToCalculate = color == 1 ? v[k - 1] : v[0];
    int result;
    MPI_Reduce(&valueToCalculate, &result, 1, MPI_INT, (color == 1 ? MPI_MAX : MPI_MIN), 0, newComm);
    if (myNewRank == 0) { // Si suppone che ci sia almeno ul processo in entrambi i gruppi altrimenti
        printf("Sono il processo %d e il max/min è %d \n", myRank, result);
        MPI_Send(&result, 1, MPI_INT, 0, color, MPI_COMM_WORLD);
    }

    if (myRank == 0) {
        int max, min;
        MPI_Recv(&max, 1, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&min, 1, MPI_INT, MPI_ANY_SOURCE, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        int sum = max + min;
        printf("Sono il processo %d e il risultato finale è %d \n", myRank, sum);
    }

    MPI_Finalize();
    return 0;
}
