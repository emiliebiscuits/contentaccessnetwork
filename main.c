#include <mpi.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include "espace.h"
#include "voisin.h"
#include "donnee.h"

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
	
	int  i;
	int demande[2];
	int * espaceDistribue;
	int espaceRecu[4];
	int identifiant[2];
	int annonceVoisin[7];
	Espace e;
	Voisins v;
	initVoisins(&v);
	Donnees d;
	initDonnees(&d);
	int coordonnee[2];
	int sum;
	int arret;
	int num;
	
	
	if(rank == 0)
	{
		//Envoyer l'invitation d'insertion à chaque processus et attendre la fin de son insertion 
		for(i=1;i<size;i++)
		{
			MPI_Send(NULL, 0, MPI_INT, i, 0, MPI_COMM_WORLD);
			MPI_Recv(NULL, 0, MPI_INT, i, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		}	
	}
	else
	{
		//Tirage d'identifiant
		identifiant[0] = tirage(rank,0,1000);
		identifiant[1] = tirage(rank+10,0,1000);

		//Attendre la commande du coordinateur 
		MPI_Recv(NULL, 0, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

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
		MPI_Send(NULL, 0, MPI_INT, 0, 0, MPI_COMM_WORLD);
		
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
			annonceVoisin[5] = identifiant[0];
			annonceVoisin[6] = identifiant[1];
		}
		MPI_Bcast(annonceVoisin, 7, MPI_INT, i, MPI_COMM_WORLD);
		if(rank != 0 && rank != i)
		{
			//printf("%d a reçu [ %d, %d, %d, %d ] du proc %d \n", rank, annonceVoisin[0], annonceVoisin[1], annonceVoisin[2], annonceVoisin[3], annonceVoisin[4]);	
			//S'il est un voisin, on l'ajoute dans la liste de voisins
			if(estVoisinHaut(&e,annonceVoisin[0], annonceVoisin[1], annonceVoisin[2], annonceVoisin[3]))
			{
				ajouterHaut(&v,annonceVoisin[4],annonceVoisin[5],annonceVoisin[6]);
			}
			else if(estVoisinBas(&e,annonceVoisin[0], annonceVoisin[1], annonceVoisin[2], annonceVoisin[3]))
			{
				ajouterBas(&v, annonceVoisin[4],annonceVoisin[5],annonceVoisin[6]);
			}
			else if(estVoisinGauche(&e,annonceVoisin[0], annonceVoisin[1], annonceVoisin[2], annonceVoisin[3]))
			{
				ajouterGauche(&v, annonceVoisin[4],annonceVoisin[5],annonceVoisin[6]);
			}
			else if(estVoisinDroite(&e,annonceVoisin[0], annonceVoisin[1], annonceVoisin[2], annonceVoisin[3]))
			{
				ajouterDroite(&v, annonceVoisin[4],annonceVoisin[5],annonceVoisin[6]);
			}
		}
	}
	//Afficher les voisins
	if(rank != 0)
	{
		afficherVoisins(&v,rank);
	}
	MPI_Barrier(MPI_COMM_WORLD); 
	//Partie 2
	if(rank==0)
	{
		
		//Initialiser et envoyer les données.
		for(i = 0; i < size*10; i++)
		{
			coordonnee[0] = tirage(i,0,1000);
			coordonnee[1] = tirage(i*2,0,1000);
			
			//Envoyer à un processus aléatoire
			MPI_Send(&coordonnee, 2, MPI_INT, tirage(i,1,size-1), 0, MPI_COMM_WORLD);
			//Attendre la demande de recupération de données
			MPI_Recv(&num, 1, MPI_INT, MPI_ANY_SOURCE, 0,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			sum = coordonnee[0] + coordonnee[1];
			//Stocker la donnee si necessaire
			if(i<5 || i<(size*10-5))
			{
				ajouterDonnee(&d,coordonnee[0],coordonnee[1],sum);
			}			
			//Envoyer la donnée
			MPI_Send(&sum, 1,  MPI_INT, num, 0, MPI_COMM_WORLD);
			//Attendre l'aquittement
			MPI_Recv(NULL, 0, MPI_INT, num, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			
		}
		//Annoncer la fin des données
		for(i=1;i<size;i++)
		{
			coordonnee[0] = -1;
			coordonnee[1] = -1;
			MPI_Send(&coordonnee, 2, MPI_INT, i, 0, MPI_COMM_WORLD);
			
		}
	}
	else
	{
		arret = 0;
		//Tant qu'il a pas reçu le message d'arret
		while(arret == 0)
		{
			//Recevoir les coordonnees
			MPI_Recv(&coordonnee, 2, MPI_INT, MPI_ANY_SOURCE, 0,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			printf("%d recu coor [%d,%d]\n",rank,coordonnee[0],coordonnee[1]);
			//Répondre si c'est dans sa zone
			if(estDedans(&e,coordonnee[0],coordonnee[1]))
			{
				printf("dans %d\n",rank);
				MPI_Send(&rank, 1, MPI_INT,0, 0, MPI_COMM_WORLD);
				//Attendre la donnee
				MPI_Recv(&sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				//Stoker la donnee
				ajouterDonnee(&d,coordonnee[0],coordonnee[1],sum);
				//Envoyer l'aquittement
				MPI_Send(NULL, 0,  MPI_INT, 0, 0, MPI_COMM_WORLD);
			}
			//Arreter la boucle si (-1,-1)
			else if(coordonnee[0]==-1 && coordonnee[1]==-1)
			{
				arret = 1;
			}
			//Router à un voisin si c'est pas dans sa zone
			else
			{
				printf("pas dans %d mais envoie à %d\n",rank, trouverProche(&v, coordonnee[0],coordonnee[1], e.xdebut, e.xfin, e.ydebut, e.yfin));
				MPI_Send(&coordonnee, 2, MPI_INT, trouverProche(&v, coordonnee[0],coordonnee[1], e.xdebut, e.xfin, e.ydebut, e.yfin), 0, MPI_COMM_WORLD);
			}			
		}
	}
	MPI_Barrier(MPI_COMM_WORLD);
	afficherDonnees(&d, rank);
	MPI_Barrier(MPI_COMM_WORLD);
	//Partie 3
	//Recherche broadcast
	if(rank == 0)
	{
		//struct timeval stop, start;
		//gettimeofday(&start, NULL);
		//Pour chaque coordonnee
		GSList * temp = d.contenu;
		Donnee * ptr = NULL;
		while(temp != NULL)
		{
			ptr = (Donnee*)temp->data;
			//Pour chaque processus
			for(i=1;i<size;i++)
			{
				MPI_Send(ptr->pos, 2, MPI_INT, i, 0, MPI_COMM_WORLD);
				//printf("send (%d %d)\n",ptr->pos[0],ptr->pos[1]);
			}
			MPI_Recv(&sum, 1, MPI_INT,MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			printf("sum : %d\n", sum);
			temp = temp->next;
		}
		coordonnee[0] = -1;
		coordonnee[1] = -1;
		for(i=1;i<size;i++)
		{
			MPI_Send(&coordonnee, 2, MPI_INT, i, 0, MPI_COMM_WORLD);
		}
		//gettimeofday(&stop, NULL);
		//printf("took %lu\n", (unsigned long)stop.tv_usec - (unsigned long)start.tv_usec);
	}
	else
	{
		arret = 0;
		while(arret == 0)
		{
			MPI_Recv(&coordonnee, 2, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			//printf("rank %d recu (%d %d)\n",rank,coordonnee[0],coordonnee[1]);
			//Si (-1,-1), arreter
			if(coordonnee[0]==-1 && coordonnee[1]==-1)
			{
				arret = 1;
			}
			//Si c'est dans son espace
			else if(estDedans(&e,coordonnee[0],coordonnee[1]))
			{
				//Parcourir les donnees et trouver l'element recherché
				sum = trouverValeur(&d,coordonnee[0],coordonnee[1]);
				//printf("%d trouvé %d\n",rank,sum);
				//Envoyer la reponse
				MPI_Send(&sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
			}
		}
	}
	MPI_Barrier(MPI_COMM_WORLD);
	//Recherche proche en proche
	if(rank == 0)
	{
		//Pour chaque coordonnee
		GSList * temp = d.contenu;
		Donnee * ptr = NULL;
		while(temp != NULL)
		{
			ptr = (Donnee*)temp->data;
			//Pour chaque processus
			//Tirer aleatoirement un processus 
			MPI_Send(ptr->pos, 2, MPI_INT, tirage(0, 1, size-1), 0, MPI_COMM_WORLD);
			//Attendre une reponse 
			MPI_Recv(&sum, 1, MPI_INT,MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			printf("sum : %d\n", sum);
			temp = temp->next;
		}
		coordonnee[0] = -1;
		coordonnee[1] = -1;
		for(i=1;i<size;i++)
		{
			MPI_Send(&coordonnee, 2, MPI_INT, i, 0, MPI_COMM_WORLD);
		}
		
	}
	else
	{
		arret = 0;
		while(arret != 1)
		{
			//Recevoir les coordonnees
			MPI_Recv(&coordonnee, 2, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			//Si (-1,-1), arreter
			if(coordonnee[0]==-1 && coordonnee[1]==-1)
			{
				arret = 1;
			}
			//Si c'est chez lui, repondre
			else if(estDedans(&e,coordonnee[0],coordonnee[1]))
			{
				//Parcourir les donnees et trouver l'element recherché
				sum = trouverValeur(&d,coordonnee[0],coordonnee[1]);
				//printf("%d trouvé %d\n",rank,sum);
				//Envoyer la reponse
				MPI_Send(&sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
			}
			//Sinon trouver un voisin et lui envoyer
			else
			{
				MPI_Send(&coordonnee, 2, MPI_INT, trouverProche(&v, coordonnee[0],coordonnee[1], e.xdebut, e.xfin, e.ydebut, e.yfin), 0, MPI_COMM_WORLD);
			}
		}
	}
	MPI_Barrier(MPI_COMM_WORLD);
	//Partie 4
	/*if(rank==0)
	{
		//Retirer une cle
		coordonnee[0] = tirage(0, 0, 1000);
		coordonnee[1] = tirage(5, 0, 1000);
		//Retirer un numero de processus et lui envoyer les coordonnees
		MPI_Send(coordonnee, 2, MPI_INT, tirage(10, 1, size-1), 0, MPI_COMM_WORLD);
		//Attendre que le noeud recoive la demande de supression
		//MPI_Recv
		//Dire aux noeuds de se preparer à une eventuelle supression d'un voisin
		coordonnee[0] = -1;
		coordonnee[1] = -1;
		for(i=1;i<size;i++)
		{
			MPI_Send(coordonnee, 2, MPI_INT, tirage(10, 1, size-1), 0, MPI_COMM_WORLD);
		}
		//Attendre la fin de la supression
		//MPI_Recv
	}
	else
	{	
		MPI_Recv(&coordonnee, 2, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		arret = 0;
		while(arret != 1)
		{
			//Si (-1,-1), arreter
			if(coordonnee[0]==-1 && coordonnee[1]==-1)
			{
				arret = 1;
			}
			//Si c'est chez lui, repondre
			else if(estDedans(&e,coordonnee[0],coordonnee[1]))
			{
				MPI_Send(&rank, 0, MPI_INT, 0, 0, MPI_COMM_WORLD);
			}
			//Sinon trouver un voisin et lui envoyer
			else
			{
				MPI_Send(&coordonnee, 2, MPI_INT, trouverProche(&v, coordonnee[0],coordonnee[1], e.xdebut, e.xfin, e.ydebut, e.yfin), 0, MPI_COMM_WORLD);
			}
		}
		//Noeud en supression
		//Envoyer son espace aux voisins
		//Recevoir une réponse d'un voisin qui peut prendre la place
		//Donner ses voisins, ses donnees à ce voisins
		//Desallouer ses voisins & donnees
		//Annoncer aux autres voisins le noeud qui prend la place 
		//Envoyer confirmation au coordinateur
		
		//Noeuds voisins
		//Si l'espace reçu correspond, donner une réponse au noeud en suppression
		//Recevoir les voisins et les donnees
		//Mettre à jour ses voisins et ses donnees
		//Sinon attendre l'annonce qui dit un noeud va prendre la place
		//Mettre à jour ses voisins
		
		//Autre noeuds
		//Rien faire
	}*/
	MPI_Barrier(MPI_COMM_WORLD);
	//Desallocations
	viderDonnees(&d);
	viderVoisin(&v);
	MPI_Finalize();
	return 0;
}

