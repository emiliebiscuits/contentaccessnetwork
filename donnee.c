#include "donnee.h"
#include <stdio.h>
#include <stdlib.h>
void initDonnees(Donnees * d)
{
	d->contenu = NULL;
}
Donnee * allocDonnee(const int x, const int y, const int val)
{
	Donnee * d = (Donnee*)malloc(sizeof(Donnee));
	d->pos[0] = x;
	d->pos[1] = y;
	d->valeur = val;
	return d;
}

void ajouterDonnee(Donnees * d, const int x, const int y, const int val)
{
	d->contenu = g_slist_append(d->contenu, allocDonnee(x, y, val));
}

void afficherDonnees(Donnees * const d, const int rank)
{
	GSList * temp = d->contenu;
	Donnee * ptr = NULL;
	while(temp!=NULL)
	{
		ptr = (Donnee*)temp->data;
		printf("Donnee dans processus %d : (( %d, %d ), %d)\n", rank, ptr->pos[0], ptr->pos[1], ptr->valeur);
		temp = temp->next;
	}
}
void viderDonnees(Donnees * d)
{
	void (*pf)(void *);
	pf = free;
	g_slist_free_full(d->contenu, pf);
}
int trouverValeur(Donnees * d, const int x, const int y)
{
	GSList * temp = d->contenu;
	Donnee * ptr = NULL;
	while(temp!=NULL)
	{
		ptr = (Donnee*)temp->data;
		if(ptr->pos[0]==x && ptr->pos[1]==y)
		{
			return ptr->valeur;
		}
		temp = temp->next;
	}
	return -1;
}

int tailleDonnees(Donnees * const d, const int * const esp)
{
	GSList * temp = d->contenu;
	Donnee * ptr = NULL;
	int count = 0;
	while(temp!=NULL)
	{
		ptr = (Donnee*)temp->data;
		if((ptr->pos[0]>=esp[0] && ptr->pos[0]<=esp[1]) || (ptr->pos[1]>=esp[2] && ptr->pos[1]<=esp[3]))
		{
			count+=1;
		} 
		temp = temp->next;
	}
	return 3*count;
}

int *tabTransferD(Donnees * const d, const int * const esp)
{
	int taille = tailleDonnees(d, esp);
	int * tab = (int*)malloc(taille*sizeof(taille));
	GSList * temp = d->contenu;
	Donnee * ptr = NULL;
	int count = 0;
	while(temp!=NULL)
	{
		ptr = (Donnee*)temp->data;
		if((ptr->pos[0]>=esp[0] && ptr->pos[0]<=esp[1]) || (ptr->pos[1]>=esp[2] && ptr->pos[1]<=esp[3]))
		{
			tab[count] = ptr->pos[0];
			tab[count+1] = ptr->pos[1];
			tab[count+2] = ptr->valeur;
			count+=3;
		} 
		temp = temp->next;
	}
	return tab;
}

void ajouterDeupuisTab(Donnees * const d, const int * const tab, const int taille)
{
	int count = 0;
	while(count < taille)
	{
		ajouterDonnee(d, tab[count], tab[count+1], tab[count+2]);
		count+=3;
	}
}
