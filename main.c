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
		MPI_Send(&inserer, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
	}
	else if(rank == 1)
	{
		int commande;
		Espace e;
		MPI_Recv(&commande, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("commande %d \n", commande);
		//affectationEspace(&e,0,1000,0,1000);
		//affichageEspace(&e);
		//MPI_Send(&fininertion, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}
	else
	{
		/*int commande;
		int xcible = tirage();
		int ycible = tirage();
		Espace e;
		MPI_Recv(&commande, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("commande %d \n", commande);
		e.xdebut = 0;
		e.ydebut = 0;
		e.xfin = 1000;
		e.yfin = 1000;*/
	}
	MPI_Finalize();
	return 0;
}
