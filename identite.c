#include <stdio.h>
#include <stdlib.h>  
#include <string.h>  

#include "identite.h"
#define MAX_LINE_LENGTH 80

struct sIdentite
{
    int Identifiant;
    char *Nom;
    char *Prenom;
    char Sexe;
    char DateNaissance [ LG_DATE +1];
};

tIdentite IdentiteCreer(int Id, char *Nom, char *Prenom, char Sexe, char DateNais[]) {
  // Allocation de la structure
  tIdentite identite = malloc(sizeof(struct sIdentite));
  if (!identite) return NULL;
  identite->Identifiant = Id;
  identite->Nom = Nom;
  identite->Prenom = Prenom;
  identite->Sexe = Sexe;
  for (int i = 0; i < LG_DATE +1; ++i) identite->DateNaissance[i] = DateNais[i];
  return identite;
}

int IdentiteIdentifiant(tIdentite Identite) {
  if (!Identite) return -1;
  return Identite->Identifiant;
}

char *IdentiteNom(tIdentite Identite) {
  if (!Identite) return NULL;
  return Identite->Nom;
}

char *IdentitePrenom(tIdentite Identite) {
  if (!Identite) return NULL;
  return Identite->Prenom;
}

char IdentiteSexe(tIdentite Identite) {
  if (!Identite) return '\0';
  return Identite->Sexe;
}

char *IdentiteDateNaissance(tIdentite Identite) {
  if (!Identite) return NULL;
  return Identite->DateNaissance;
}


void IdentiteAfficher(tIdentite Identite) {
  if (!Identite) return;
  printf("[%d] %s %s, %c, %s\n", Identite->Identifiant,Identite->Nom,Identite->Prenom, Identite->Sexe, Identite->DateNaissance);
}

void IdentiteLiberer(tIdentite Identite) {
  free(Identite->Nom);
  free(Identite->Prenom);
  free(Identite);
}

tIdentite IdentiteLiref(FILE *f) {
  int identifiant;
  char *nom = malloc(sizeof(char) * (LG_NOM + 1));
  char *prenom = malloc(sizeof(char) * (LG_PRENOM + 1));
  char sexe;
  char date_naissance[LG_DATE + 1];

  if (fscanf(f, "%d", &identifiant) != 1) return NULL;
  if (fscanf(f, "%s", nom) !=1) return NULL;
  if (fscanf(f, "%s", prenom) !=1) return NULL;
  if (fscanf(f, " %c", &sexe) != 1) return NULL;
  if (fscanf(f, "%s", date_naissance) !=1) return NULL;
  tIdentite identite = IdentiteCreer(identifiant, nom, prenom, sexe, date_naissance);
  return identite;
}

