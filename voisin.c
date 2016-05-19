#include <stdio.h>
#include <stdlib.h>
#include "voisin.h"

int * allocTab(const int num, const int x1, const int x2, const int y1, const int y2)
{
	int *tab = (int*)malloc(3 * sizeof(int));
	tab[0] = num;
	tab[1] = (x1 + x2)/2;
	tab[2] = (y1 + y2)/2;
	return tab;
}

void initVoisins(Voisins * const v)
{
	v->voisinsHaut = NULL;
	v->voisinsBas = NULL;
	v->voisinsGauche = NULL;
	v->voisinsDroite = NULL;
}

void ajouterHaut(Voisins * const v, const int num, const int x1, const int x2, const int y1, const int y2)
{
	v->voisinsHaut = g_slist_append(v->voisinsHaut, allocTab(num, x1, x2, y1, y2));
}
void ajouterBas(Voisins * const v, const int num, const int x1, const int x2, const int y1, const int y2)
{
	v->voisinsBas = g_slist_append(v->voisinsBas, allocTab(num, x1, x2, y1, y2));
}
void ajouterGauche(Voisins * const v, const int num, const int x1, const int x2, const int y1, const int y2)
{
	v->voisinsGauche = g_slist_append(v->voisinsGauche, allocTab(num, x1, x2, y1, y2));
}
void ajouterDroite(Voisins * const v, const int num, const int x1, const int x2, const int y1, const int y2)
{
	v->voisinsDroite = g_slist_append(v->voisinsDroite, allocTab(num, x1, x2, y1, y2));
}


void supprimerHaut(Voisins * const v, const int num)
{
	GSList *temp = v->voisinsHaut;
	int *pointeur;
	while(temp != NULL && (((int*)temp->data)[0] == num))
	{
		temp = temp->next;
	}
	if(temp != NULL)
	{
		pointeur = temp->data;
		v->voisinsHaut = g_slist_remove(v->voisinsHaut,temp->data);
		free(pointeur);
	}
}


void afficherVoisins(Voisins * const v, const int num)
{	
	GSList *temp = v->voisinsHaut;
	while(temp != NULL)
	{
		printf("%d a un voisin haut : proc %d (%d , %d)\n", num, ((int*)temp->data)[0], ((int*)temp->data)[1], ((int*)temp->data)[2]);
		temp = temp->next;
	}
	
	temp = v->voisinsBas;
	while(temp != NULL)
	{
		printf("%d a un voisin bas : proc %d (%d , %d)\n", num, ((int*)temp->data)[0], ((int*)temp->data)[1], ((int*)temp->data)[2]);
		temp = temp->next;
	}
	
	temp = v->voisinsGauche;
	while(temp != NULL)
	{
		printf("%d a un voisin gauche : proc %d (%d , %d)\n", num, ((int*)temp->data)[0], ((int*)temp->data)[1], ((int*)temp->data)[2]);
		temp = temp->next;
	}
	temp = v->voisinsDroite;
	while(temp != NULL)
	{
		printf("%d a un voisin droite : proc %d (%d , %d)\n", num, ((int*)temp->data)[0], ((int*)temp->data)[1], ((int*)temp->data)[2]);
		temp = temp->next;
	}
}

