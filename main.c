#include <mpi.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "espace.h"

//Commandes
const int inserer = 1;
const int fininertion =2;



int tirage(int rang)
{
	srand(time(NULL));
	return ((rand()%1001) * rang)%1001; 
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
		int demande[2];
		for(i=1;i<size;i++)
		{
			MPI_Send(&inserer, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
			MPI_Recv(&commande, 1, MPI_INT, i, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		}	
	}
	else
	{
		int commande;
		int demande[2];
		int j;
		Espace e;
		int cible[2] = {tirage(rank),tirage(rank+10)};
		MPI_Recv(&commande, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		//printf("commande %d \n", commande);
		if(rank == 1)
		{
			affectationEspace(&e,0,1000,0,1000);
			affichageEspace(&e);
		}
		else
		{
			for(j=1;j<rank;j++)
			{
				MPI_Send(cible, 2, MPI_INT, j, 0, MPI_COMM_WORLD);
				printf("proc %d envoie: %d %d  \n",rank,cible[0],cible[1]);
			}
			//attend l'autorisation d'insertion
			//insertion
		}
		MPI_Send(&fininertion, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
		for(j=rank+1;j<size;j++)
		{
			MPI_Recv(demande, 2, MPI_INT, j, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			printf("proc %d reçu: %d %d  du proc %d \n",rank, demande[0],demande[1],j);
		}
		//if dans l'espace, envoie autorisation d'insertion
		printf("fin proc %d \n",rank);
	}
	MPI_Finalize();
	return 0;
}
