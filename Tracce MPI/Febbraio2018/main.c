#include <stdio.h>
#include <mpi.h>
//#include <stdlib.h>
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

// Stampa la Matrice - Primo parametro matrice, secondo parametro righe, terzo parametro colonne e quarto parametro rango di chi stampa la matrice
void printMatrix(int *mat, int dim1, int dim2, int myRank) {
    printf("\nsono il processo %d e stampo la matrice \n", myRank);
    for (int i = 0; i < dim1; i++) {
        for (int j = 0; j < dim2; j++) {
            printf("%d ", mat[i * dim2 + j]);
        }
        printf("\n");
    }
}

void printVector(int *vet, int dim, int myRank) {
    printf("\nsono il processo %d e stampo il vettore: \n", myRank);
    for (int i = 0; i < dim; i++) {
        printf("%d ", vet[i]);
    }
    printf("\n");
}

typedef struct {

    int max;
    int rank;
} rankToMax;

// TODO: Usa nProc = 4 poiché dim/nProc deve essere un numero intero
int main(int argc, char *argv[]) {

    int myRank, nProc;
    int matrixDim = 8; // La matrice deve essere quadrata 8x8
    int matrixA[matrixDim][matrixDim];

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &nProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    MPI_Status status;

    int k = matrixDim / nProc; // Rapporto dimensione matrice e numero di processi --> deve essere intero!

    // Creazione DataType per inviare più colonne
    MPI_Datatype column, columns;
    // MPI_Type_vector creates an MPI datatype by replicating an existing MPI_Datatype a certain number of times into a block.
    // The MPI_Datatype created will contain a certain number of such blocks separated with the constant displacement specified.
    MPI_Type_vector(matrixDim, 1, matrixDim, MPI_INT, &column);
    // MPI_Type_create_resized allows to change the lower bound and extent of an MPI_Datatype.
    // This feature finds its use when multiple elements are sent (count > 1); typically to interleave non-contiguous datatypes.
    MPI_Type_create_resized(column, 0, 1 * (sizeof(int)), &columns);
    // MPI_Type_commit must be called on user-defined datatypes before they may be used in communications.
    MPI_Type_commit(&columns);

    // PUNTO 1 e 2
    if (myRank == 0) {
        // Quando passiamo la matrice a questi due metodi dobbiamo specificare il primo elemento [0][0]
        readMatrixFromFile("matb.txt", &matrixA[0][0], matrixDim, matrixDim);
        printMatrix(&matrixA[0][0], matrixDim, matrixDim, myRank);

        for (int i = 0; i < nProc; ++i) {
            MPI_Send(&matrixA[0][i * k], k, columns, i, TAG_ARBITRARY, MPI_COMM_WORLD);
        }
    }

    int matrixT[k][matrixDim];
    MPI_Recv(&matrixT[0][0], k * matrixDim, MPI_INT, 0, TAG_ARBITRARY, MPI_COMM_WORLD, &status);

    MPI_Barrier(MPI_COMM_WORLD);
    sleep(1);
    printMatrix(&matrixT[0][0], k, matrixDim, myRank);


    // PUNTO 3
    int sum_max = 0;
    int max[matrixDim];

    /*
     Creo un nuovo vettore che ha matrixDim elementi e ogni elemento di questo vettore
     è il massimo tra i k elementi della stessa colonna:
     ex:
     30 38 12 94 17 55 33 17
     31 39 22 10 18 26 34 12
    result:
     31 39 22 94 18 55 33 17
     */

    for (int j = 0; j < matrixDim; j++) { // for che scorre le colonne

        max[j] = matrixT[0][j]; // mi prendo il primo elemento della colonna j
        for (int i = 1; i < k; i++) {

            if (matrixT[i][j] > max[j]) { // confronto quell'elemento con tutti quelli della stessa colonna
                max[j] = matrixT[i][j]; // sovrascrivo quando trovo un valore più alto
            }
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);
    sleep(1);
    printVector(&max[0], matrixDim, myRank);

    // Sommo tutti gli elementi del vettore e ottengo un numero
    for (int i = 0; i < matrixDim; i++) {
        sum_max += max[i];
    }

    MPI_Barrier(MPI_COMM_WORLD);
    sleep(1);
    printf("sono: %d e ho %d \n", myRank, sum_max);

    // PUNTO 4
    int M[nProc];
    rankToMax s1, s2;
    s1.max = sum_max;
    s1.rank = myRank;

    MPI_Comm newComm = MPI_COMM_WORLD;
    int conteggio = 0;
    int color = 0;

    do {

        MPI_Allreduce(&s1, &s2, 1, MPI_2INT, MPI_MAXLOC, newComm);

        if (sum_max == s2.max && color != 1) {
            color = 1;
            MPI_Send(&s2.max, 1, MPI_INT, 0, 67, MPI_COMM_WORLD);
        }

        if (myRank == 0) {
            MPI_Recv(&M[conteggio], 1, MPI_INT, MPI_ANY_SOURCE, 67, MPI_COMM_WORLD, &status);
        }

        conteggio++;
        MPI_Comm_split(newComm, color, myRank, &newComm);

    } while (conteggio < nProc);

    if (myRank == 0) {
        sleep(1);
        printVector(&M[0], nProc, myRank);
    }

    MPI_Finalize();
}
