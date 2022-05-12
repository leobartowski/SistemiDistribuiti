#include <stdio.h>
#include "mpi.h"

void printMatrix(int *matrix, int dim1, int dim2, int myRank) {
    printf("Sono il processo %d e stampo: \n", myRank);
    for (int i = 0; i < dim1; ++i) {
        for (int j = 0; j < dim2; ++j) {
            printf("%d ", matrix[i * dim2 + j]);
        }
        printf("\n");
    }
}

void readMatrix(char *fileName, int *matrix, int dim1, int dim2) {
    FILE *file = fopen(fileName, "r");
    for (int i = 0; i < dim1; ++i) {
        for (int j = 0; j < dim2; ++j) {
            fscanf(file, "%d ", &matrix[i * dim2 + j]);
        }
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
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    MPI_Comm_size(MPI_COMM_WORLD, &nProc);

    // ** MATRICI

    //! Punto 1
    int dim1 = 8;
    int k1 = dim1 / nProc;
    int a1[dim1][dim1];
    int t1[dim1][k1];

    if (myRank == 0) {

        readMatrix("matA.txt", &a1[0][0], dim1, dim1);
//        printMatrix(&a1[0][0], dim1, dim1, myRank);

        MPI_Datatype columnRoundRobin;
        MPI_Type_vector(dim1 * k1, 1, nProc, MPI_INT, &columnRoundRobin);
        MPI_Type_commit(&columnRoundRobin);

        for (int i = 0; i < nProc; ++i) {
            MPI_Send(&a1[0][i], 1, columnRoundRobin, i, 1, MPI_COMM_WORLD);
        }
    }

    MPI_Recv(&t1[0][0], k1 * dim1, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//    printMatrix(&t1[0][0], dim1, k1, myRank);


    //! Punto 2
    int dim2 = 8;
    int k2 = dim2 / nProc;
    int a2[dim2][dim2];
    int t2[k2][dim2];

    if (myRank == 0) {

        readMatrix("matA.txt", &a2[0][0], dim2, dim2);
//        printMatrix(&a2[0][0], dim2, dim2, myRank);

        MPI_Datatype rowRoundRobin;
        MPI_Type_vector(k2, dim2, nProc * dim2, MPI_INT, &rowRoundRobin);
        MPI_Type_commit(&rowRoundRobin);

        for (int i = 0; i < nProc; ++i) {
            MPI_Send(&a2[i][0], 1, rowRoundRobin, i, 2, MPI_COMM_WORLD);
        }
    }

    MPI_Recv(&t2[0][0], k2 * dim2, MPI_INT, 0, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//    printMatrix(&t2[0][0], k2, dim2, myRank);


    //! Punto 3
    int dim3 = 8;
    int k3 = dim3 / nProc;
    int a3[dim3][dim3], b3[dim3][dim3];
    // 31 = punto 3 parte 1, 32 = parte 3 punto 2
    int t31[k3][dim3], t32[dim3][k3];

    if (myRank == 0) {
        readMatrix("matA.txt", &a3[0][0], dim3, dim3);
        readMatrix("matB.txt", &b3[0][0], dim3, dim3);
//        printMatrix(&a3[0][0], dim3, dim3, myRank);
        printMatrix(&b3[0][0], dim3, dim3, myRank);

        MPI_Datatype columnConsecutive;
        MPI_Type_vector(dim3, k3, dim3, MPI_INT, &columnConsecutive);
        MPI_Type_commit(&columnConsecutive);

        for (int i = 0; i < nProc; ++i) {
            //SendRow
            MPI_Send(&a3[i * k3][0], k3 * dim3, MPI_INT, i, 31, MPI_COMM_WORLD);
            // Send Column
            MPI_Send(&b3[0][i * k3], 1, columnConsecutive, i, 32, MPI_COMM_WORLD);
        }
    }

    // Receive Rows
    MPI_Recv(&t31[0][0], k3 * dim3, MPI_INT, 0, 31, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//    printMatrix(&t31[0][0], k3, dim3, myRank);

    // Receive Columns
    MPI_Recv(&t32[0][0], k3 * dim3, MPI_INT, 0, 32, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printMatrix(&t32[0][0], dim3, k3, myRank);

    //! Punto 4
    int k4 = 8; // Numero di riche matrice A
    int a4[k4][nProc];
    int v4[k4];

    if (myRank == 0) {
        readMatrix("matA.txt", &a4[0][0], k4, nProc);
//        printMatrix(&a4[0][0], k4, nProc, myRank);

        MPI_Datatype singleColumn;
        MPI_Type_vector(k4, 1, nProc, MPI_INT, &singleColumn);
        MPI_Type_commit(&singleColumn);

        for (int i = 0; i < nProc; ++i) {
            MPI_Send(&a4[0][i], 1, singleColumn, i, 4, MPI_COMM_WORLD);
        }
    }

    MPI_Recv(&v4[0], k4, MPI_INT, 0, 4, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//    printMatrix(&v4[0], k4, 1, myRank);

    // ** VETTORI

    //! Punto 5
    int dim5 = 16;
    int a5[dim5], b5[dim5];
    int k5 = dim5 / nProc;
    int t51[k5], t52[k5];
    int m5 = 2;

    if (myRank == 0) {

        readVector("vectorA.txt", &a5[0], dim5);
        printMatrix(&a5[0], 1, dim5, myRank);
        readVector("vectorB.txt", &b5[0], dim5);
//        printMatrix(&b5[0], 1, dim5, myRank);

        MPI_Datatype blockVector;
        MPI_Type_vector(k5 / m5, m5, nProc * m5, MPI_INT, &blockVector);
        MPI_Type_commit(&blockVector);

        for (int i = 0; i < nProc; ++i) {
            // Send consecutive
            MPI_Send(&a5[i * k5], k5, MPI_INT, i, 51, MPI_COMM_WORLD);
            // Send round-robin
            MPI_Send(&b5[i * m5], 1, blockVector, i, 52, MPI_COMM_WORLD);
        }
    }

    // Recv parte di A
    MPI_Recv(&t51[0], k5, MPI_INT, 0, 51, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//    printMatrix(&t51[0], 1, k5, myRank);
    // Recv parte di B
    MPI_Recv(&t52[0], k5, MPI_INT, 0, 52, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//    printMatrix(&t52[0], 1, k5, myRank);

    MPI_Finalize();


    return 0;
}
