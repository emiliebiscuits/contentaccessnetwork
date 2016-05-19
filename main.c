#include <mpi.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "espace.h"
#include "voisin.h"

//Commandes
const int inserer = 1;
const int fininsertion = 2;



int tirage(int rang, int debut, int fin)
{
	srand(time(NULL));
	return ((rand()%(fin-debut+1)+debut) * rang)%(fin-debut+1)+debut; 
}



int main(int argc,char **argv)
{
	int size, rank;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	//printf("Hello world from %d(out of %d procs.!)\n",rank,size);
	
	int commande, i;
	int demande[2];
	int * espaceDistribue;
	int espaceRecu[4];
	int identifiant[2];
	int annonceVoisin[5];
	Espace e;
	Voisins v;
	initVoisins(&v);
	
	if(rank == 0)
	{
		//Envoyer l'invitation d'insertion à chaque processus et attendre la fin de son insertion 
		for(i=1;i<size;i++)
		{
			MPI_Send(&inserer, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
			MPI_Recv(&commande, 1, MPI_INT, i, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		}	
	}
	else
	{
		//Tirage d'identifiant
		identifiant[0] = tirage(rank,0,1000);
		identifiant[1] = tirage(rank+10,0,1000);

		//Attendre la commande du coordinateur 
		MPI_Recv(&commande, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		if(rank == 1)
		{
			affectationEspace(&e,0,1000,0,1000);
		}
		else
		{
			//Demande d'insertion à chaque processus déjà inséré
			for(i=1;i<rank;i++)
			{
				MPI_Send(identifiant, 2, MPI_INT, i, 0, MPI_COMM_WORLD);
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
		MPI_Send(&fininsertion, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
		
		//Ecouter chaque demande d'insertion
		for(i=rank+1;i<size;i++)
		{
			MPI_Recv(demande, 2, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			//si dans l'espace, envoie autorisation d'insertion
			if(estDedans(&e,demande[0],demande[1]))
			{
				espaceDistribue = (int*)malloc(4*sizeof(int)); 
				decouperEspace(&e, identifiant, espaceDistribue);
				MPI_Send(espaceDistribue, 4, MPI_INT, i, 0, MPI_COMM_WORLD);
				free(espaceDistribue);
			}
			
		}
		//Affichage d'espace et d'indentifiant
		printf("Espace de %d : ",rank);
		affichageEspace(&e);
		printf("Identifiant de %d : ( %d, %d )\n",rank, identifiant[0],identifiant[1]);
	}
	//Broadcast pour annoncer son espace à tous les processus, et chaque processus calcul si l'espace reçu est son voisin
	for(i=1;i<size;i++)
	{
		if(rank == i)
		{
			annonceVoisin[0] = e.xdebut;
			annonceVoisin[1] = e.xfin;
			annonceVoisin[2] = e.ydebut;
			annonceVoisin[3] = e.yfin;
			annonceVoisin[4] = rank;
		}
		MPI_Bcast(annonceVoisin, 5, MPI_INT, i, MPI_COMM_WORLD);
		if(rank != 0 && rank != i)
		{
			//printf("%d a reçu [ %d, %d, %d, %d ] du proc %d \n", rank, annonceVoisin[0], annonceVoisin[1], annonceVoisin[2], annonceVoisin[3], annonceVoisin[4]);	
			//S'il est un voisin, on l'ajoute dans la liste de voisins
			if(estVoisinHaut(&e,annonceVoisin[0], annonceVoisin[1], annonceVoisin[2], annonceVoisin[3]))
			{
				ajouterHaut(&v, annonceVoisin[4], annonceVoisin[0], annonceVoisin[1], annonceVoisin[2], annonceVoisin[3]);
			}
			else if(estVoisinBas(&e,annonceVoisin[0], annonceVoisin[1], annonceVoisin[2], annonceVoisin[3]))
			{
				ajouterBas(&v, annonceVoisin[4], annonceVoisin[0], annonceVoisin[1], annonceVoisin[2], annonceVoisin[3]);
			}
			else if(estVoisinGauche(&e,annonceVoisin[0], annonceVoisin[1], annonceVoisin[2], annonceVoisin[3]))
			{
				ajouterGauche(&v, annonceVoisin[4], annonceVoisin[0], annonceVoisin[1], annonceVoisin[2], annonceVoisin[3]);
			}
			else if(estVoisinDroite(&e,annonceVoisin[0], annonceVoisin[1], annonceVoisin[2], annonceVoisin[3]))
			{
				ajouterDroite(&v, annonceVoisin[4], annonceVoisin[0], annonceVoisin[1], annonceVoisin[2], annonceVoisin[3]);
			}
		}
	}
	if(rank != 0 && rank != i)
	{
		afficherVoisins(&v,rank);
	}
	MPI_Finalize();
	return 0;
}

