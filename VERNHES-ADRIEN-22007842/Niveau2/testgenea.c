#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "genea.h"

int main(int argc, char *argv[]) {
  if (3!=argc ) {
    fprintf(stderr, "Erreur : nombre d'arguments incorrect.\n");
    return 1;
  }

  // Création de l'arbre et lecture des personnes
  tArbre arbre = ArbreLirePersonnesFichier(argv[1]);
  if (arbre == NULL) {
    fprintf(stderr, "Erreur lors de la lecture des personnes.\n");
    return 1;
  }

  FILE *f = fopen(argv[2], "r");
  if (NULL==f) {
    fprintf(stderr, "Erreur d'ouverture du fichier de liens de parenté.\n");
    ArbreLiberer(arbre);
    return 1;
  }

  // Lecture des liens de parenté
  int idEnfant, idParent;
  char parente;

  while (ArbreLireLienParentef(f, &idEnfant, &idParent, &parente) == 1) {
    ArbreAjouterLienParente(arbre, idEnfant, idParent, parente);
  }

  fclose(f);

  printf("Arbre avec liens de parenté :\n");
  ArbreAfficher(arbre);
  ArbreLiberer(arbre);

  return 0;
}