#include <stdio.h>
#include <mpi.h>
#include <iostream>

using namespace std;

const int dimension = 5;
const int matrixSize = dimension * dimension;

int main (int argc, char **argv) {

    int source[matrixSize];
    int destination[matrixSize];

    for (int i = 0; i < matrixSize; i++) {
	source[i] = i + 1;
	destination[i] = 0;
    }
    
    int blockLength[dimension];
    for (int i = 0; i < dimension; i++) {
	blockLength[i] = i + 1;
    }
    
    int blockIndex[dimension];
    for (int i = 0; i < dimension; i++) {
	blockIndex[i] = i * dimension;
    }
    
    MPI_Datatype Triangular_Matrix;

    MPI_Init(&argc, &argv);

    int rank;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Type_indexed(dimension, blockLength, blockIndex, MPI_INT, &Triangular_Matrix);

    MPI_Type_commit(&Triangular_Matrix);

    if (rank == 0) {
        //int MPI_Send(void *buf, int count, MPI_Datatype datatype,
        //int dest, int tag, MPI_Comm comm)
        //buf — указатель на начало сообщения длиной в count
        //элементов, каждый из которых имеет тип datatype
        //dest — ранг получателя в коммуникаторе comm
        //tag — положительное целое число, идентифицирующее передачу
        //tag может использоваться чтобы различать сообщение
	
	for (int i = 0; i < dimension; i++) {
	    for (int j = 0; j < dimension; j++) {
		cout<<source[i * dimension + j]<<" ";
	    }
	    cout<<endl;
	}

	MPI_Send(source, 1, Triangular_Matrix, 1, 0, MPI_COMM_WORLD);

	cout<<"--------------------------------"<<endl;
    }

    if (rank == 1) {
        //MPI_Recv(void *buf, int count, MPI_Datatype datatype,
        //int source, int tag, MPI_Comm comm, MPI_Status *status)
        //source — ранг отправителя в коммуникаторе comm
        //Информация о сообщении возвращается в status
	MPI_Recv(destination, 1, Triangular_Matrix, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

	for (int i = 0; i < dimension; i++) {
	    for (int j = 0; j < dimension; j++) {
		cout<<destination[i * dimension + j] << ' ';
	    }
	    cout<<endl;
	}
    }

    MPI_Finalize();
}
