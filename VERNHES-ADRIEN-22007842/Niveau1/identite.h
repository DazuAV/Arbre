#ifndef TESTIDENTITE_H
#define TESTIDENTITE_H

#include <stdio.h>
#include <stdlib.h>
#define LG_DATE 10
#define LG_NOM 10
#define LG_PRENOM 10

typedef struct sIdentite *tIdentite ;
int IdentiteIdentifiant(tIdentite Identite);
char *IdentiteNom(tIdentite Identite);
char *IdentitePrenom(tIdentite Identite);
char IdentiteSexe(tIdentite Identite);
char *IdentiteDateNaissance(tIdentite Identite);
void IdentiteAfficher(tIdentite Identite);
void IdentiteLiberer(tIdentite Identite);
tIdentite IdentiteLiref(FILE *f);

#endif
