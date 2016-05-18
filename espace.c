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

void decouperEspace(Espace * const e, int * const retour)
{
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
