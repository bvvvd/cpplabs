#include <stdio.h>
#include <mpi.h>
#include <iostream>

using namespace std;

int main (int argc, char **argv) {

    MPI_Init(&argc, &argv);
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int dimension = size;
    int matrixSize = dimension * dimension;
   
    int sourceMatrix[matrixSize];
    for (int i = 0; i < dimension; i++)
        for (int j = 0; j < dimension; j++)
            sourceMatrix[i * dimension + j] = i * dimension + j + 1;

    int blockLenght[dimension];
    for (int i = 0; i < dimension; i++) {
        blockLenght[i] = 1;
    }

    //Массив индексов для каждого блока, каждый индекс в этом массиве
    //равен индексу (в исходной матрице) первого элемента соответствующего блока
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int blockIndex[dimension];
    for (int i = 0; i < dimension; i++) {
        blockIndex[i] = i * dimension + rank;
    }

    MPI_Datatype Matrix_Column;
    MPI_Type_indexed(dimension, blockLenght, blockIndex, MPI_INT, &Matrix_Column);
    MPI_Type_commit(&Matrix_Column);

    int bufferMatrix[dimension * dimension * dimension]; 
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Gather(sourceMatrix, 1, Matrix_Column, bufferMatrix, 1, Matrix_Column, 0, MPI_COMM_WORLD);

    if (rank == 0) {
	int destinationMatrix[dimension][dimension];
        int bufferedI = 0;
        int bufferedJ = 0;
        for (int j = 0; j < dimension; j++) {
            for (int i = 0; i < dimension; i++) {
                destinationMatrix[i][j] = bufferMatrix[bufferedI * dimension + bufferedJ];
                bufferedI++;
            }
            bufferedI--;
            bufferedJ++;
        }

        for (int i = 0; i < dimension; i++) {
            for (int j = 0; j < dimension; j++) {
                cout<<destinationMatrix[i][j]<<' ';
            }
            cout<<endl;
        }
    }

    MPI_Finalize();
}
                     
