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
	printf("Espace : %d %d %d %d\n",e->xdebut, e->xfin, e->ydebut, e->yfin);
}

int estDedans(const Espace * const e, const int x, const int y)
{
	if(x>=e->xdebut && x<=e->xfin && y>=e->ydebut && y<=e->yfin)
	{
		return 1;
	}
	return 0;
}
