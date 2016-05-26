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
		printf("rank %d: ( %d, %d ) val: %d\n", rank, ptr->pos[0], ptr->pos[1], ptr->valeur);
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
