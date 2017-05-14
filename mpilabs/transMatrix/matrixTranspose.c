#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#define N 9
#define MAXPROC 64
int main(int argc, char **argv)
{
   int rank, size, nl, i, j, ii, jj, ir, irr;
   double c;
   MPI_Status status;
   MPI_Request req[MAXPROC*2];
   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &size);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   nl = (N-1)/size+1;
   double *a = (double *) calloc(size*nl*nl, sizeof(double));
   double *b = (double *) calloc(size*nl*nl, sizeof(double));
   for(i = 0; i<nl; i++){
      ii = i+rank*nl;
      if(ii<N)
         for(j = 0; j<N; j++)
            *(a+j*nl+i)=100*ii+j;
   }
   for(ir = 0; ir<size; ir++){
      MPI_Irecv(b+ir*nl*nl, nl*nl, MPI_DOUBLE, ir, 1, MPI_COMM_WORLD, &req[ir]);
      MPI_Isend(a+ir*nl*nl, nl*nl, MPI_DOUBLE, ir, 1, MPI_COMM_WORLD, &req[ir+size]);
   }
   for(irr = 0; irr<size; irr++){
      MPI_Waitany(size, req, &ir, &status);
      ir = status.MPI_SOURCE;
      for(i = 0; i<nl; i++){
         for(j = i+1; j<nl; j++){
            c = *(b+i*nl+j+ir*nl*nl);
            *(b+i*nl+j+ir*nl*nl) = *(b+j*nl+i+ir*nl*nl);
            *(b+j*nl+i+ir*nl*nl) = c;
         }
      }
   }
   for(i = 0; i<nl; i++){
      ii = i+rank*nl;
      if(ii<N)
         for(j = 0; j<N; j++)
            printf("process %d : a[%d][%d] = %lf b[%d][%d] = %lf\n", rank, ii, j, *(a+j*nl+i), ii, j, *(b+j*nl+i));
   }
   free(a);
   free(b);
   MPI_Finalize();
}
