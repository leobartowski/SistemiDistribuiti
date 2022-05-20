#include <stdio.h>
#include <mpi.h>
#include <unistd.h>

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
    int dim = 20, nProc, myRank;

    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    MPI_Comm_size(MPI_COMM_WORLD, &nProc);

    int k = dim / nProc;

    if (myRank == 0) {
        int v[dim];
        readVector("vector.txt", &v[0], dim);
        MPI_Datatype block;
        MPI_Type_vector(2, k / 2, (k / 2) * nProc, MPI_INT, &block);
        MPI_Type_commit(&block);

        for (int i = 0; i < nProc; ++i) {

            MPI_Send(&v[i * (k / 2)], 1, block, i, 0, MPI_COMM_WORLD);
        }
    }
    int aTemp[2][k / 2], a[k / 2][2];

    MPI_Recv(&aTemp[0][0], k, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    for (int i = 0; i < (k / 2); ++i) {
        for (int j = 0; j < 2; ++j) {
            a[i][j] = aTemp[j][i];
        }
    }

    printMatrix(&a[0][0], k / 2, 2, myRank);
    int color = 0;
    if (nProc % 2 == 1) {
        int local[2], global[2];
        local[0] = a[0][0];
        local[1] = myRank;
        MPI_Allreduce(&local[0], &global[0], 1, MPI_2INT, MPI_MINLOC, MPI_COMM_WORLD);
        if (global[1] == myRank)
            color = 1;

        MPI_Comm newComm;
        MPI_Comm_split(MPI_COMM_WORLD, color, 0, &newComm);
    }
    MPI_Comm newComm;
    MPI_Comm_split(MPI_COMM_WORLD, color, 0, &newComm);

    if (color == 0) {
        MPI_Comm topologyComm;
        int nDims = 2;
        int dims[2] = {nProc / 2, 2};
        int periods[2] = {0, 0};
        int reorder = 0;
        MPI_Cart_create(newComm, nDims, dims, periods, reorder, &topologyComm);
        int myTopologyRank;
        MPI_Comm_rank(topologyComm, &myTopologyRank);
        int coords[2];
        MPI_Cart_coords(topologyComm, myTopologyRank, nDims, coords);

        if (coords[0] == 0) {
//            int destCoords[2] = {coords[0] + 1, coords[1]};
//            int destRank;
//            MPI_Cart_rank(topologyComm, destV2, &destRank);
            int destV2 = myTopologyRank + (nProc / 2);
            MPI_Send(&a[0][0], 1, MPI_INT, destV2, 5, topologyComm);
        } else {
            int temp;
            MPI_Recv(&temp, 1, MPI_INT, MPI_ANY_SOURCE, 5, topologyComm, MPI_STATUS_IGNORE);
            a[0][0] = temp;
        }

        if (myTopologyRank == 0)
            printf("\n-------------------------------\n");

        MPI_Barrier(topologyComm);
        sleep(1);
        printMatrix(&a[0][0], k / 2, 2, myRank);
    }

    MPI_Finalize();
    return 0;
}
