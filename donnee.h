#ifndef DONNEE_H
#define DONNEE_H

#include <glib.h>

struct donnee{
	int pos[2];
	int valeur;
};
typedef struct donnee Donnee;

typedef GSList Donnees; 


#endif

