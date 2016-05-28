#ifndef DONNEE_H
#define DONNEE_H

#include <glib.h>

struct donnee{
	int pos[2];
	int valeur;
};
typedef struct donnee Donnee;

struct donnees{
	GSList *contenu;
};

typedef struct donnees Donnees; 

Donnee * allocDonnee(const int, const int, const int);
void initDonnees(Donnees *);
void ajouterDonnee(Donnees * , const int, const int, const int);
void afficherDonnees(Donnees * const, const int);
int trouverValeur(Donnees * , const int, const int);
void logDonnees(Donnees * const, const int);
void viderDonnees(Donnees *);
int tailleDonnees(Donnees * const, const int * const);
int *tabTransferD(Donnees * const, const int * const);
void ajouterDeupuisTab(Donnees * const, const int * const, const int);
#endif

