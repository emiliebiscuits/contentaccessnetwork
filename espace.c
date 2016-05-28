#include "espace.h"
#include <stdio.h>
#include <stdlib.h>
void affectationEspace(Espace * const e,const int x1, const int x2, const int y1, const int y2)
{
	e->xdebut = x1;
	e->xfin = x2;
	e->ydebut = y1;
	e->yfin = y2;
}

void affichageEspace(const Espace * const e)
{
	printf("[xdebut : %d  xfin : %d  ydebut : %d  yfin : %d]\n",e->xdebut, e->xfin, e->ydebut, e->yfin);
}

int estDedans(const Espace * const e, const int x, const int y)
{
	if(x>=e->xdebut && x<=e->xfin && y>=e->ydebut && y<=e->yfin)
	{
		return 1;
	}
	return 0;
}

void decouperEspaceEnDeux(Espace * const e, int * const identifiant, int * const retour)
{
	int temp;
	if(e->xfin - e->xdebut >= e->yfin - e->ydebut)
	{	temp = e->xfin;
		e->xfin = (e->xfin + e->xdebut)/2;
		retour[2] = e->ydebut;
		retour[3] = e->yfin;

		if(estDedans(e, identifiant[0], identifiant[1])){
			retour[1] = temp; 
			retour[0] = e->xfin + 1;
		}
		else{
			retour[1] = e->xfin; 
			retour[0] = e->xdebut;
			e->xdebut = e->xfin +1;
			e->xfin = temp;
		}
	}
	else
	{	temp = e->yfin;
		e->yfin = (e->yfin + e->ydebut)/2;
		retour[0] = e->xdebut;
		retour[1] = e->xfin;

		if(estDedans(e, identifiant[0], identifiant[1])){
			retour[2] = e->yfin+1;
			retour[3] = temp;
		}
		else{
			retour[2] = e->ydebut;
			retour[3] = e->yfin;
			e->ydebut = e->yfin +1;
			e->yfin = temp;
		}
	}
}

int estVoisinHaut(const Espace * const e, const int x1, const int x2, const int y1, const int y2)
{
	if (y2 == e->ydebut-1)
	{
		if(x1>=e->xdebut && x1<e->xfin)
		{
			return 1;
		}
		else if(x2>e->xdebut && x2<=e->xfin)
		{
			return 1;
		}
		else if(x1<=e->xdebut && x2>=e->xfin)
		{
			return 1;
		}
		return 0;
	}
	return 0;
}


int estVoisinBas(const Espace * const e, const int x1, const int x2, const int y1, const int y2)
{
	if (y1 == e->yfin+1)
	{
		if(x1>=e->xdebut && x1<e->xfin)
		{
			return 1;
		}
		else if(x2>e->xdebut && x2<=e->xfin)
		{
			return 1;
		}
		else if(x1<=e->xdebut && x2>=e->xfin)
		{
			return 1;
		}
		return 0;
	}
	return 0;
}

int estVoisinGauche(const Espace * const e, const int x1, const int x2, const int y1, const int y2)
{
	if (x2 == e->xdebut-1)
	{
		if(y1>=e->ydebut && y1<e->yfin)
		{
			return 1;
		}
		else if(y2>e->ydebut && y2<=e->yfin)
		{
			return 1;
		}
		else if(y1<=e->ydebut && y2>=e->yfin)
		{
			return 1;
		}
		return 0;
	}
	return 0;
}

int estVoisinDroite(const Espace * const e, const int x1, const int x2, const int y1, const int y2)
{
	if (x1 == e->xfin+1)
	{
		if(y1>=e->ydebut && y1<e->yfin)
		{
			return 1;
		}
		else if(y2>e->ydebut && y2<=e->yfin)
		{
			return 1;
		}
		else if(y1<=e->ydebut && y2>=e->yfin)
		{
			return 1;
		}
		return 0;
	}
	return 0;
}



int estVoisin(const Espace * const e, const int x1, const int x2, const int y1, const int y2)
{
	if(estVoisinHaut(e,x1,x2,y1,y2) || estVoisinBas(e,x1,x2,y1,y2) || estVoisinGauche(e,x1,x2,y1,y2) || estVoisinDroite(e,x1,x2,y1,y2))
	{
		return 1;
	}
	return 0;
}

void logEspace(const Espace * const e, const int rank)
{
	FILE* fichier = NULL;
    fichier = fopen("test.txt", "w");
    if (fichier != NULL)
    {
        fprintf(fichier,"%d [xdebut : %d  xfin : %d  ydebut : %d  yfin : %d]\n", rank, e->xdebut, e->xfin, e->ydebut, e->yfin);
        fclose(fichier);
    }
}


int * decouperEspace(Espace * const e, const int * const recu)
{
	int * retour = NULL;
	if(recu[0]==e->xdebut && recu[1]<=e->xfin)
	{
		retour = (int*)malloc(4*sizeof(int));
		retour[0] = recu[0];
		retour[1] = recu[1];
		retour[2] = e->ydebut;
		retour[3] = e->yfin;
		e->xdebut = retour[1] + 1;		
	}
	else if(recu[2]==e->ydebut && recu[3]<=e->yfin)
	{
		retour = (int*)malloc(4*sizeof(int));
		retour[0] = e->xdebut;
		retour[1] = e->xfin;
		retour[2] = recu[2];
		retour[3] = recu[3];
		e->ydebut = retour[3] + 1;	
	}
	return retour;
}

void remplirTabParEspace(const Espace * const e, int * const tab)
{
	tab[0] = e->xdebut;
	tab[1] = e->xfin;
	tab[2] = e->ydebut;
	tab[3] = e->yfin;
}

int estValide(const Espace * const e)
{
	return ((e->xdebut<e->xfin)&&(e->ydebut<e->yfin));
}

void mettreAJourEspace(Espace * const e,const int * const tab)
{
	if(e->xdebut==tab[0] && e->xfin==tab[1])
	{
		e->ydebut = min(e->ydebut, tab[2]);
		e->yfin = max(e->yfin, tab[3]);
	}
	else if(e->ydebut==tab[2] && e->yfin==tab[3])
	{
		e->xdebut = min(e->xdebut, tab[0]);
		e->xfin = max(e->yfin, tab[1]);
	}
}

int max(const int a, const int b)
{
	if(a>b)
	{
		return a;
	}
	return b;
}
int min(const int a, const int b)
{
	if(a<b)
	{
		return a;
	}
	return b;
}



