#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "geneaV2.h"

int main(int argc, char *argv[]) {
  if (5!=argc ) {
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

 // ArbreEcrireGV permet de crer le fichier DOT
    ArbreEcrireGV(arbre, argv[4]);

    printf("Arbre avec liens de parenté :\n");
    ArbreAfficher(arbre);
    printf("                                              \n");

    // atoi permet convertir char argv[3] en entier identifiant
    int IDPersonne = atoi(argv[3]);

    printf("\nArbre ascendant de la personne d'identifiant %d :\n", IDPersonne);
    ArbreAfficherAscendants(arbre, IDPersonne);

    ArbreEcrireAscendantsGV(arbre, IDPersonne, "ascendants.dot");

    printf("\nArbre ascendant de la personne d'identifiant %d écrit dans ascendants.dot.\n", IDPersonne);

    ArbreLiberer(arbre);

    return 0;
}