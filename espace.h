#ifndef ESPACE_H
#define ESPACE_H
struct espace
{
	int xdebut;
	int xfin;
	int ydebut;
	int yfin;
	
};
typedef struct espace Espace;


void affectationEspace(Espace * const,const int, const int, const int, const int);

void affichageEspace(const Espace * const);

int estDedans(const Espace * const, const int, const int);
#endif
