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

void logEspace(const Espace * const, const int rank);

int estDedans(const Espace * const, const int, const int);

void decouperEspace(Espace * const, int * const, int * const);

int estVoisin(const Espace * const, const int, const int, const int, const int);

int estVoisinHaut(const Espace * const, const int, const int, const int, const int);

int estVoisinBas(const Espace * const, const int, const int, const int, const int);

int estVoisinGauche(const Espace * const, const int, const int, const int, const int);

int estVoisinDroite(const Espace * const, const int, const int, const int, const int);
#endif
