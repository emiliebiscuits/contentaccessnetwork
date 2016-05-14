#include <mpi.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "espace.h"

const int inserer = 1;
const int fininertion =2;



int tirage()
{
	srand(time(NULL));
	return rand()%1001; 
}



int main(int argc,char **argv)
{
	int size,rank;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	//printf("Hello world from %d(out of %d procs.!)\n",rank,size);
	if(rank == 0)
	{
		int commande, i;
		for(i=1;i<size;i++)
		{
			MPI_Send(&inserer, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
			MPI_Recv(&commande, 1, MPI_INT, i,0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		}	
	}
	else
	{
		int commande;
		Espace e;
		int cibleX = tirage();
		int cibleY  = tirage();
		MPI_Recv(&commande, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("commande %d \n", commande);
		if(rank == 1)
		{
			affectationEspace(&e,0,1000,0,1000);
			affichageEspace(&e);
			//MPI_IRecv
			//if dans l'espace MPI_Send 
		}
		else
		{
			//MPI_Bcast 
			//MPI_Recv
		}
		MPI_Send(&fininertion, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}
	MPI_Finalize();
	return 0;
}
