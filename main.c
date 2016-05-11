#include <mpi.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int tirage()
{
	srand(time(NULL));
	return rand()%1000; 
}

int main(int argc,char **argv)
{
	int size,rank;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	printf("Hello world from %d(out of %d procs.!)\n",rank,size);
	MPI_Finalize();
	return 0;
}
