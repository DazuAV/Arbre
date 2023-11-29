#include <stdio.h>
#include <stdlib.h>

#include "IDENTITE.h"

int main() {
  // Création d'une identité
  tIdentite identite = IdentiteCreer(16, "DUFF", "John", 'M', "13/12/2001");

  // Vérification de l'identifiant
  if (IdentiteIdentifiant(identite) != 16) {
    printf("Erreur : l'identifiant est incorrect\n");
    return 1;
  }

  // Vérification du nom
  if (strcmp(IdentiteNom(identite), "DUFF") != 0) {
    printf("Erreur : le nom est incorrect\n");
    return 1;
  }

  // Vérification du prénom
  if (strcmp(IdentitePrenom(identite), "John") != 0) {
    printf("Erreur : le prénom est incorrect\n");
    return 1;
  }

  // Vérification du sexe
  if (IdentiteSexe(identite) != 'M') {
    printf("Erreur : le sexe est incorrect\n");
    return 1;
  }

  // Vérification de la date de naissance
  if (strcmp(IdentiteDateNaissance(identite), "13/12/2001") != 0) {
    printf("Erreur : la date de naissance est incorrecte\n");
    return 1;
  }

  // Affichage de l'identité
  IdentiteAfficher(identite);

  // Libération de l'identité
  IdentiteLiberer(identite);

  return 0;
}
