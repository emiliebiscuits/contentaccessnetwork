#ifndef VOISIN_H
#define VOISIN_H

#include <glib.h>

struct voisins{
	GSList *voisinsHaut;
	GSList *voisinsBas;
	GSList *voisinsGauche;
	GSList *voisinsDroite;
};
typedef struct voisins Voisins;

int * allocTab(const int, const int, const int, const int, const int);

void initVoisins(Voisins * const);

void ajouterHaut(Voisins * const, const int, const int, const int, const int, const int);
void ajouterBas(Voisins * const, const int, const int, const int, const int, const int);
void ajouterGauche(Voisins * const, const int, const int, const int, const int, const int);
void ajouterDroite(Voisins * const, const int, const int, const int, const int, const int);

void supprimerHaut(Voisins * const, const int);
void supprimerBas(Voisins * const, const int);
void supprimerGauche(Voisins * const, const int);
void supprimerDroite(Voisins * const, const int);
void supprimerUn(GSList *, const int);


void afficherGSList(GSList * const, const int, const int);
void afficherVoisins(Voisins * const, const int);

void viderVoisin(Voisins * const);
#endif
