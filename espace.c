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

int *decouperEspace(Espace * const e)
{
	int *retour = (int*)malloc(4*sizeof(int));
	if(e->xfin - e->xdebut >= e->yfin - e->ydebut)
	{
		retour[1] = e->xfin ; 
		e->xfin = (e->xfin + e->xdebut)/2;
		retour[0] = e->xfin + 1;
		retour[2] = e->ydebut;
		retour[3] = e->yfin;
		 
	}
	else
	{
		retour[3] = e->yfin; 
		e->yfin = (e->yfin + e->ydebut)/2;
		retour[2] = e->yfin + 1;
		retour[0] = e->xdebut;
		retour[1] = e->xfin;
	}
	return retour;
}
