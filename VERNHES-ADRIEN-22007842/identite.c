#include <stdio.h>
#include <stdlib.h>  
#include <string.h>  

#include "IDENTITE.h" 


tIdentite IdentiteCreer(int Id, char *Nom, char *Prenom, char Sexe, char DateNais[]) {
  // Allocation de la structure
  tIdentite identite = malloc(sizeof(struct sIdentite));
  if (identite == NULL) {
    return NULL;
  }

  // Allocation des chaînes de caractères
  identite->Nom = malloc(strlen(Nom) + 1);
  if (identite->Nom == NULL) {
    free(identite);
    return NULL;
  }
  identite->Prenom = malloc(strlen(Prenom) + 1);
  if (identite->Prenom == NULL) {
    free(identite->Nom);
    free(identite);
    return NULL;
  }

  // Copie des chaînes de caractères
  strcpy(identite->Nom, Nom);
  strcpy(identite->Prenom, Prenom);
  identite->Sexe = Sexe;
  strcpy(identite->DateNaissance, DateNais);

  return identite;
}

int IdentiteIdentifiant(tIdentite Identite) {
  if (Identite == NULL) {
    return -1;
  }
  return Identite->Identifiant;
}

char *IdentiteNom(tIdentite Identite) {
  if (Identite == NULL) {
    return NULL;
  }

  return Identite->Nom;
}

char *IdentitePrenom(tIdentite Identite) {
  if (Identite == NULL) {
    return NULL;
  }

  return Identite->Prenom;
}

char IdentiteSexe(tIdentite Identite) {
  if (Identite == NULL) {
    return '\0';
  }

  return Identite->Sexe;
}

char *IdentiteDateNaissance(tIdentite Identite) {
  if (Identite == NULL) {
    return NULL;
  }

  return Identite->DateNaissance;
}


void IdentiteAfficher(tIdentite Identite) {
  if (Identite == NULL) {
    return;
  }

  printf("[%d] ", Identite->Identifiant);

  printf("%s ", Identite->Nom);

  printf("%s, ", Identite->Prenom);

  printf("%c, ", Identite->Sexe);

  printf("%s\n", Identite->DateNaissance);
}

void IdentiteLiberer(tIdentite Identite) {

  free(Identite->Nom);
  free(Identite->Prenom);
  free(Identite);
}

#define MAX_LINE_LENGTH 80

static tIdentite IdentiteLireLigne(FILE *f) {
  // Déclaration des variables
  int identifiant;
  char nom[LG_NOM + 1];
  char prenom[LG_PRENOM + 1];
  char sexe;
  char date_naissance[LG_DATE + 1];

  // Lecture de l'identifiant
  if (fscanf(f, "%d", &identifiant) != 1) {
    return NULL;
  }

  // Lecture du nom
  if (fgets(nom, MAX_LINE_LENGTH, f) == NULL) {
    return NULL;
  }
  removeNewline(nom);

  // Lecture du prénom
  if (fgets(prenom, MAX_LINE_LENGTH, f) == NULL) {
    return NULL;
  }
  removeNewline(prenom);

  // Lecture du sexe
  if (fscanf(f, " %c", &sexe) != 1) {
    return NULL;
  }

  // Lecture de la date de naissance
  if (fgets(date_naissance, MAX_LINE_LENGTH, f) == NULL) {
    return NULL;
  }
  removeNewline(date_naissance);

  // Création de l'identité
  tIdentite identite = IdentiteCreer(identifiant, nom, prenom, sexe, date_naissance);

  // Retour de l'identité
  return identite;
}

tIdentite IdentiteLiref(FILE *f) {
  // Lecture de la première ligne
  tIdentite identite = IdentiteLireLigne(f);

  // Tant qu’il y a des informations à lire
  while (identite != NULL) {
    // Retour de l’identité
    return identite;

    // Lecture de la ligne suivante
    identite = IdentiteLireLigne(f);
  }

  // Retour de NULL
  return NULL;
}
