#ifndef TESTIDENTITE_H
#define TESTIDENTITE_H

#include <stdio.h>
#include <stdlib.h>


#define LG_DATE 10
#define LG_NOM 10
#define LG_PRENOM 10



struct sIdentite
{
int Identifiant ; // Identifiant unique
char * Nom ; // Nom (cha îne de caract ères allou ée dynamiquement )
char * Prenom ; // Prénom (cha îne de caract ères allou ée dynamiquement )
char Sexe ; // ’F’ ou ’M’
char DateNaissance [ LG_DATE +1]; // Date de naissance sous la forme jj/mm/ aaaa
};

typedef struct sIdentite * tIdentite ;

tIdentite IdentiteCreer(int Id, char *Nom, char *Prenom, char Sexe, char DateNais[]);


int IdentiteIdentifiant(tIdentite Identite);

char *IdentiteNom(tIdentite Identite);

char *IdentitePrenom(tIdentite Identite);

char IdentiteSexe(tIdentite Identite);

void IdentiteAfficher(tIdentite Identite);

void IdentiteLiberer(tIdentite Identite);

tIdentite IdentiteLiref(FILE *f);


#endif
