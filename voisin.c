#include <stdio.h>
#include <stdlib.h>
#include "voisin.h"
#include <string.h>

int * allocTab(const int num, const int idx, const int idy)
{
	int *tab = (int*)malloc(3 * sizeof(int));
	tab[0] = num;
	tab[1] = idx;
	tab[2] = idy;
	return tab;
}

void initVoisins(Voisins * v)
{
	v->voisinsHaut = NULL;
	v->voisinsBas = NULL;
	v->voisinsGauche = NULL;
	v->voisinsDroite = NULL;
}

void ajouterHaut(Voisins * const v, const int num, const int idx, const int idy)
{
	int *ptr = allocTab(num, idx, idy);
    v->voisinsHaut = g_slist_insert_sorted(v->voisinsHaut,ptr,comparerX);
	//v->voisinsHaut = g_slist_append(v->voisinsHaut, ptr);
}
void ajouterBas(Voisins * const v, const int num, const int idx, const int idy)
{
	int *ptr = allocTab(num, idx, idy);
    v->voisinsBas = g_slist_insert_sorted(v->voisinsBas, ptr,comparerX);
	//v->voisinsBas = g_slist_append(v->voisinsBas, allocTab(num, idx, idy));
}
void ajouterGauche(Voisins * const v, const int num, const int idx, const int idy)
{
	int *ptr = allocTab(num, idx, idy);
    v->voisinsGauche = g_slist_insert_sorted(v->voisinsGauche, ptr,comparerY);
	//v->voisinsGauche = g_slist_append(v->voisinsGauche, allocTab(num, idx, idy));
}
void ajouterDroite(Voisins * const v, const int num, const int idx, const int idy)
{
	int *ptr = allocTab(num, idx, idy);
    v->voisinsDroite = g_slist_insert_sorted(v->voisinsDroite, ptr, comparerY);
	//v->voisinsDroite = g_slist_append(v->voisinsDroite, allocTab(num, idx, idy));
}

void supprimerUn(GSList * l, const int num)
{
	GSList *temp = l;
	int *pointeur;
	while(temp != NULL && (((int*)temp->data)[0] == num))
	{
		temp = temp->next;
	}
	if(temp != NULL)
	{
		pointeur = temp->data;
		l = g_slist_remove(l,temp->data);
		free(pointeur);
	}
}

void supprimerHaut(Voisins * const v, const int num)
{
	supprimerUn(v->voisinsHaut, num);
}

void supprimerBas(Voisins * const v, const int num)
{
	supprimerUn(v->voisinsBas, num);
}

void supprimerGauche(Voisins * const v, const int num)
{
	supprimerUn(v->voisinsGauche, num);
}

void supprimerDroite(Voisins * const v, const int num)
{
	supprimerUn(v->voisinsDroite, num);
}



void afficherGSList(GSList * const l, const int num, const int direction)
{
	GSList *temp = l;
	char *c;
	switch(direction)
	{
		case 1: 
			c = "haut";
			break;
		case 2:
			c = "bas";
			break;
		case 3:
			c = "gauche";
			break;
		case 4:
			c = "droite";
			break;
	}
	while(temp != NULL)
	{
		printf("Processus %d a un voisin %s : proc %d (%d , %d)\n", num, c, ((int*)temp->data)[0], ((int*)temp->data)[1], ((int*)temp->data)[2]);
		temp = temp->next;
	}
}


void afficherVoisins(Voisins * const v, const int num)
{	
	afficherGSList(v->voisinsHaut, num, 1);
	afficherGSList(v->voisinsBas, num, 2);
	afficherGSList(v->voisinsGauche, num, 3);
	afficherGSList(v->voisinsDroite, num, 4);
}

void viderVoisin(Voisins * v)
{
	void (*pf)(void *);
	pf = free;
	g_slist_free_full(v->voisinsHaut, pf);
	g_slist_free_full(v->voisinsBas, pf);
	g_slist_free_full(v->voisinsGauche, pf);
	g_slist_free_full(v->voisinsDroite, pf);
}

int trouverProche(Voisins * const v, const int x, const int y, const int xdebut, const int xfin, const int ydebut, const int yfin)
{
	
	if(y<ydebut)
	{
		return ((int*)v->voisinsHaut->data)[0];
	}
	else if(y>yfin)
	{
		return ((int*)v->voisinsBas->data)[0];
	}
	else if(x<xdebut)
	{
		return ((int*)v->voisinsGauche->data)[0];
	}
	else
	{
		return ((int*)v->voisinsDroite->data)[0];
	}
}

int comparerX(gconstpointer a, gconstpointer b)
{
	int grand = ((int*)a)[1];
	int petit = ((int*)b)[1];
	return (grand>petit);
}
int comparerY(gconstpointer a, gconstpointer b)
{
	int grand = ((int*)a)[2];
	int petit = ((int*)b)[2];
	return (grand>petit);
}

/*void trierVoisin(Voisins * const v)
{
	v->voisinsHaut = g_slist_sort(v->voisinsHaut,comparerX);
	v->voisinsBas = g_slist_sort(v->voisinsBas,comparerX);
	v->voisinsGauche = g_slist_sort(v->voisinsGauche,comparerY);
	v->voisinsDroite = g_slist_sort(v->voisinsDroite,comparerY);
}*/

int tailleVoisins(const Voisins * const v)
{
	int h = g_slist_length (v->voisinsHaut);
	int b = g_slist_length (v->voisinsBas);
	int g = g_slist_length (v->voisinsGauche);
	int d = g_slist_length (v->voisinsDroite);
	return (h + b + g + d + 3);
}

int *tabTransferV(const Voisins * const v)
{
	int taille = tailleVoisins(v);
	int *tab = (int*)malloc(taille * sizeof(int));
	GSList * temp = v->voisinsHaut;
	int * ptr = NULL;
	int count = 0;
	while(temp!=NULL)
	{
		ptr = (int*)temp->data;
		tab[count] = ptr[0];
		tab[count+1] = ptr[1];
		tab[count+2] = ptr[2];
		count+=3;
		temp = temp->next;
	}
	tab[count] = -1;
	count+=1;
	
	temp = v->voisinsBas;
	ptr = NULL;
	while(temp!=NULL)
	{
		ptr = (int*)temp->data;
		tab[count] = ptr[0];
		tab[count+1] = ptr[1];
		tab[count+2] = ptr[2];
		count+=3;
		temp = temp->next;
	}
	tab[count] = -1;
	count+=1;
	temp = v->voisinsGauche;
	ptr = NULL;
	while(temp!=NULL)
	{
		ptr = (int*)temp->data;
		tab[count] = ptr[0];
		tab[count+1] = ptr[1];
		tab[count+2] = ptr[2];
		count+=3;
		temp = temp->next;
	}
	tab[count] = -1;
	count+=1;
	temp = v->voisinsDroite;
	ptr = NULL;
	while(temp!=NULL)
	{
		ptr = (int*)temp->data;
		tab[count] = ptr[0];
		tab[count+1] = ptr[1];
		tab[count+2] = ptr[2];
		count+=3;
		temp = temp->next;
	}
	
	return tab;
}
