#include <stdio.h>
#include <mpi.h>
#include <iostream>
#include <cassert>

using namespace std;

int main (int argc, char **argv) {
    
    MPI_Init(&argc, &argv);

    int rank;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int size;

    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int receiver[size];
    //MPI_Gather(void* sendbuf, int sendcount, MPI_Datatype sendtype,void* recvbuf, int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm)
    MPI_Gather(&rank, 1, MPI_INT, &receiver, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
	
	int sum = 0;
	int etalonSum = 0;

	for (int i = 0; i < size; i++) {
	    sum += receiver[i];
	    etalonSum += i;
        }

        cout<<"RankSum is: "<<sum<<endl;
	assert(sum == etalonSum);
    }

    MPI_Finalize();
}

