#include <mpi.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "espace.h"

//Commandes
const int inserer = 1;
const int fininertion =2;



int tirage(int rang, int debut, int fin)
{
	srand(time(NULL));
	return ((rand()%(fin-debut+1)+debut) * rang)%(fin-debut+1)+debut; 
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
		int * espaceDistribue;
		int espaceRecu[4];
		int j;
		Espace e;
		//Tirage d'identifiant
		int identifiant[2] = {tirage(rank,0,1000),tirage(rank+10,0,1000)};
		//Attendre la commande du coordinateur 
		MPI_Recv(&commande, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		if(rank == 1)
		{
			affectationEspace(&e,0,1000,0,1000);
		}
		else
		{
			//Demande d'insertion à chaque processus déjà inséré
			for(j=1;j<rank;j++)
			{
				MPI_Send(identifiant, 2, MPI_INT, j, 0, MPI_COMM_WORLD);
			}
			//attend l'autorisation d'insertion
			MPI_Recv(espaceRecu, 4, MPI_INT, MPI_ANY_SOURCE,0,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			//insertion
			affectationEspace(&e,espaceRecu[0],espaceRecu[1],espaceRecu[2],espaceRecu[3]);
			//Si l'identifiant I n'appartient pas à ce sous-espace, I tire aléatoirement un nouvel identifiant dans ce sous-espace.
			if(estDedans(&e,identifiant[0],identifiant[1])==0)
			{
				identifiant[0] = tirage(rank, espaceRecu[0],espaceRecu[1]);
				identifiant[1] = tirage(rank, espaceRecu[2],espaceRecu[3]);
			}
		}
		//Dire au coordinateur que l'insertion est finie
		MPI_Send(&fininertion, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
		
		//Ecouter chaque demande d'insertion
		for(j=rank+1;j<size;j++)
		{
			MPI_Recv(demande, 2, MPI_INT, j, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			//si dans l'espace, envoie autorisation d'insertion
			if(estDedans(&e,demande[0],demande[1]))
			{
				espaceDistribue = decouperEspace(&e);
				MPI_Send(espaceDistribue, 4, MPI_INT, j, 0, MPI_COMM_WORLD);
				free(espaceDistribue);
			}
			//Si l'identifiant I n'appartient pas à ce sous-espace, I tire aléatoirement un nouvel identifiant dans ce sous-espace.
			if(estDedans(&e,identifiant[0],identifiant[1])==0)
			{
				identifiant[0] = tirage(rank, e.xdebut,e.xfin);
				identifiant[1] = tirage(rank, e.ydebut,e.yfin);
			}
		}
		//Affichage d'espace et d'indentifiant
		printf("Espace de %d :\n",rank);
		affichageEspace(&e);
		printf("Identifiant de %d : \n( %d, %d )\n\n\n",rank, identifiant[0],identifiant[1]);
	}
	
	MPI_Finalize();
	return 0;
}
