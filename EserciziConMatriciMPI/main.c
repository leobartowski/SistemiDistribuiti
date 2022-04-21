#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <unistd.h>

#define TAG_FOR_COLUMNS 67890
#define TAG_FOR_ROWS 12345

// Il primo parametro è il nome del file, il secondo è la matrice dove mettere i dati letti, il terzo e il quarto le dim della matrice
void readMatrixFromFile(char *fileName, int *mat, int dim1, int dim2) {
    FILE *fd = fopen(fileName, "r");
    for (int i = 0; i < dim1; i++) {
        for (int j = 0; j < dim2; j++) {
            fscanf(fd, "%d ", &mat[i * dim2 + j]);
        }
    }
}


void printMatrix(int *mat, int dim1, int dim2, int myRank) {
    printf("\nsono il processo %d e stampo la matrice \n", myRank);
    for (int i = 0; i < dim1; i++) {
        for (int j = 0; j < dim2; j++) {
            printf("%d ", mat[i * dim2 + j]);
        }
        printf("\n");
    }
}

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

    // TODO : Differenza invio righe e colonne
    // http://personalpages.to.infn.it/~mignone/MPI/lecture4.pdf
    /*
        C salva le matrici per riga quindi:
            - l'invio di righe: MPI_Type_contiguous
            - l'invio di colonne: MPI_Type_vector
     */

    // TODO: DATATYPE COLONNE
    /*

    MPI_Type_vector(blockCount, blockLength, M, MPI_DOUBLE, &dt_columns) creates a new MPI datatype that consists of M blocks of rows elements
    of MPI_DOUBLE each with the heads of the consecutive blocks M elements apart stride M. Something like this:
                        |<------------   stride   ------------->|
                        |<-blockLength->|                       |
                        +---+---+---+---+---+---+---+---+---+---+--
                        | x | x | x | x |   |   |   |   |   |   | ^
                        +---+---+---+---+---+---+---+---+---+---+ |
                        | x | x | x | x |   |   |   |   |   |   | |
                        +---+---+---+---+---+---+---+---+---+---+
                        .   .   .   .   .   .   .   .   .   .   .   blockCount
                        +---+---+---+---+---+---+---+---+---+---+
                        | x | x | x | x |   |   |   |   |   |   | |
                        +---+---+---+---+---+---+---+---+---+---+ |
                        | x | x | x | x |   |   |   |   |   |   | v
                        +---+---+---+---+---+---+---+---+---+---+--

    Cioè, MPI_Type_Vector mi dà la possibilità di creare colonne fatte da:
     blockCount  = elementi (numero di righe -> quanti elementi ha una colonna),
     blockLenght =  quanto sono lunghi i blocchi -> se devo inviare colonne metto sempre 1!
     stride = distanza dall'inizio di ogni blocco, espressa in numero di elementi -> metto sempre uguale a blockCount matrici quadrate (colonne)
     4 parametro = Datatype del singolo blocco
     5 parametro = Datatype che sto creando

     Se volessi  inviare una colonna avrei finito qua, ma visto che voglio inviare più colonne
     non posso aumentare il numero di blocchi poiché MPI li andrebbe a cercare in verticale
     quindi devo dirgli com'è fatta la colonna con MPI_Type_vector e utilizzando
     MPI_Type_create_resized gli dico quante colonne deve prendere partendo dalla prima
     Firma:
     1 parametro: datatype custom creato con vector per identificare la colonna
     2 parametro: estremo inferiore (0 nel nostro caso)
     3 parametro: estremo superiore nel nostro caso è: numero di colonne * sizeof(elemento singolo della matrice (int) )
     4 parametro: datatype che sto creando

     */
    MPI_Datatype columnDT, columnsDT;
    MPI_Type_vector(matrixDim, k, matrixDim, MPI_INT, &columnDT);
    MPI_Type_create_resized(columnDT, 0, k * (sizeof(int)), &columnsDT);
    MPI_Type_commit(&columnsDT);

    // TODO: DATATYPE RIGHE (POCO UTILE)
    /*
     Il datatype per inviare righe potremmo anche non utilizzarlo, visto che C invia considera gli elementi per riga
     quindi basta dire invia x elementi e lui invierà gli elementi scorrendo la matrice per riga.
     Utilizzando il datatype invece di inviare x elementi di tipo MPI_INT, inviamo un blocco continuo
     di elementi quindi dobbiamo stare attenti a quanti elementi ricevere nella send
     */
    MPI_Datatype rowDT;
    MPI_Type_contiguous(matrixDim, MPI_INT, &rowDT);
    MPI_Type_commit(&rowDT);

    if (myRank == 0) {
        // Quando passiamo la matrice a questi due metodi dobbiamo specificare il primo elemento [0][0]
        readMatrixFromFile("matrice.txt", &matrixA[0][0], matrixDim, matrixDim);
        printMatrix(&matrixA[0][0], matrixDim, matrixDim, myRank);

        for (int i = 0; i < nProc; ++i) {
            // TODO: INVIO LA MATRICE PER RIGHE CON DATATYPE MPI_INT
            MPI_Send(&matrixA[i * k][0], k * matrixDim, MPI_INT, i, TAG_FOR_ROWS, MPI_COMM_WORLD);

            //INVIO LA MATRICE PER RIGHE UTILIZZANDO IL DATATYPE CUSTOM
//            MPI_Send(&matrixA[i * k][0], k, rowDT, i, TAG_FOR_ROWS, MPI_COMM_WORLD);

            // TODO: INVIO LA MATRICE PER COLONNE
            MPI_Send(&matrixA[0][i * k], 1, columnsDT, i, TAG_FOR_COLUMNS, MPI_COMM_WORLD);
        }
    }

    /*
       Una volta inviati gli elementi per righe o per colonne possiamo leggerli come vogliamo,
       infatti come li andiamo a disporre nella ricezione poco conta.
       É importante ricordare che in C le matrici sono viste come un vettore di allocazioni di memoria
       quindi sta a noi dire in che forma li vogliamo mettere.
       Inoltre quello che è molto importante è controllare anche la printMatrix poiché se noi diamo valori
       diversi da quelli della definizione della matrice che vogliamo stampare essa stamperà in ordine diverso
       ma non darà errori (sempre perché per C la matrice è un vettore quindi invertendo righe e colonne avrò sempre gli stessi elementi
       e non avrò un index out of range)!
     */

    /*
     Nella ricezione non mi interessa specificare un particolare datatype
     */

    // TODO: RICEZIONE PER RIGHE
    int matrixRows[k][matrixDim];
    MPI_Recv(&matrixRows[0][0], k * matrixDim, MPI_INT, 0, TAG_FOR_ROWS, MPI_COMM_WORLD, &status);
    MPI_Barrier(MPI_COMM_WORLD);
    printMatrix(&matrixRows[0][0], k, matrixDim, myRank); // k righe e matrixDim colonne

    sleep(2);

    // TODO: RICEZIONE PER COLONNE
    int matrixColumns[matrixDim][k];
    MPI_Recv(&matrixColumns[0][0], k * matrixDim, MPI_INT, 0, TAG_FOR_COLUMNS, MPI_COMM_WORLD, &status);
    MPI_Barrier(MPI_COMM_WORLD);
    printMatrix(&matrixColumns[0][0], matrixDim, k, myRank); // k righe e matrixDim colonne

    MPI_Finalize();
}

