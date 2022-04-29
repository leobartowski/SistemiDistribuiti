#include <stdio.h>
#include "limits.h"
#include "mpi.h"

void readMatrix(char *fileName, int *matrix, int dim1, int dim2) {
    FILE *file = fopen(fileName, "r");

    for (int i = 0; i < dim1; ++i) {
        for (int j = 0; j < dim2; ++j) {
            fscanf(file, "%d ", &matrix[i * dim2 + j]);
        }
    }
}

void printMatrix(int *matrix, int dim1, int dim2, int myRank) {
    printf("\nsono il processo %d e stampo la matrice: \n", myRank);
    for (int i = 0; i < dim1; ++i) {
        for (int j = 0; j < dim2; ++j) {
            printf("%d ", matrix[i * dim2 + j]);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {

    int myRank, nProc;
    int matrixDim = 8;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    MPI_Comm_size(MPI_COMM_WORLD, &nProc);
    MPI_Status status;
    MPI_Aint lb, extent;

    int k = matrixDim / nProc;

    MPI_Datatype column, columnBlock;
    MPI_Type_vector(k * matrixDim, 1, nProc, MPI_INT, &column);
    MPI_Type_create_resized(column, lb, sizeof(int), &columnBlock);
    MPI_Type_commit(&columnBlock);

    int startMatrix[matrixDim][matrixDim];

    if (myRank == 0) {
        readMatrix("mat.txt", &startMatrix[0][0], matrixDim, matrixDim);
        printMatrix(&startMatrix[0][0], matrixDim, matrixDim, myRank);
    }

    int matrixT[matrixDim][k];
    MPI_Scatter(&startMatrix[0][0], 1, columnBlock, &matrixT[0][0], k * matrixDim, MPI_INT, 0, MPI_COMM_WORLD);

//    printMatrix(&matrixT[0][0], matrixDim, k, myRank);

    MPI_Barrier(MPI_COMM_WORLD);

    //! Punto 2
    int maxVector[matrixDim];

    for (int i = 0; i < matrixDim; ++i) {
        maxVector[i] = matrixT[i][0];
        for (int j = 0; j < k; ++j) {

            if (matrixT[i][j] > maxVector[i]) {
                maxVector[i] = matrixT[i][j];
            }
        }
    }
//    printMatrix(&maxVector[0], matrixDim, 1, myRank);

    //! Punto 3
    int v0AndRank[2], globalResult[2]; // Utilizzo un array di dimensione due per avere valore e rango da utilizzare in combinazione con MPI_MAXLOC (vedi docs)
    int color = 0; // mi serve per creare il communicator
    v0AndRank[0] = maxVector[0];
    v0AndRank[1] = myRank;// Deve essere il massimo

    // Questo 2 dovrebbe essere 6 per la traccia ma visto che ho solo 4 processi vado a scegliere solo i 2 max

    int numberOfProcessMax = 2; // Numero di processi che mi servono per la topologia dei massimi
    for (int i = 0; i < numberOfProcessMax; ++i) {
        MPI_Allreduce(&v0AndRank[0], &globalResult[0], 1, MPI_2INT, MPI_MAXLOC, MPI_COMM_WORLD);
        if (globalResult[1] == myRank) { // In questo if entrano solo i processi che hanno il massimo valore
            v0AndRank[0] = INT_MIN;
            color = 1;
        }
    }
    if (color == 1) {
        printf("Solo il processo %d e il mio valore è %d \n", myRank, maxVector[0]);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Comm maxComm; // Communicator che mi serve per far parlare i processi che posseggono i valori massimi

    MPI_Comm_split(MPI_COMM_WORLD, color, 0, &maxComm);

    int dims[2] = { 0 };
    int nDims = 1, periods[nDims];// in this way we can create a topology with preferred dimension
    periods[0] = 0; // serve nel caso in cui volessi creare una topologia chiusa su se stessa (toroide)

    // Topologia
    MPI_Comm topologyComm;

    if (color == 1) {

        MPI_Dims_create(numberOfProcessMax, nDims, dims);
        MPI_Cart_create(maxComm, nDims, dims, periods, 0, &topologyComm);

        int topologyRank;
        MPI_Comm_rank(topologyComm, &topologyRank);

        // need to know the coords in the mesh
        int myCoords;
        int rangoDx, rangoSx;
        MPI_Cart_coords(topologyComm, topologyRank, 1, &myCoords);
        printf("Sono il processo %d topologyRank: %d -- Pos(%d) \n", myRank, topologyRank, myCoords);
    }

    MPI_Finalize();
    return 0;
}