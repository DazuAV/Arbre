#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "geneaV2.h"



int main(int argc, char *argv[]) {
  if (5!=argc ) {
    fprintf(stderr, "Nombre d'arguments incorrect.\n");
    return 1;
  }

 // Création de l'arbre et lecture des personnes
    tArbre arbre = ArbreLirePersonnesFichier(argv[1]);
    if (arbre == NULL) {
        fprintf(stderr, "Erreur lors de la lecture des personnes.\n");
        return 1;
    }

    // Ajout des liens de parenté à l'arbre
    arbre = ArbreLireLienParenteFichier(arbre, argv[2]);
    if (arbre == NULL) {
        fprintf(stderr, "Erreur lors de la lecture des liens de parenté.\n");
        ArbreLiberer(arbre);
        return 1;
    }
  

 // ArbreEcrireGV permet de crer le fichier DOT
    ArbreEcrireGV(arbre, argv[4]);

    printf("Arbre liens de parenté :\n");
    ArbreAfficher(arbre);
    printf("                                              \n");

    // atoi permet convertir char argv[3] en entier identifiant
    int IDPersonne = atoi(argv[3]);

    printf("\nArbre ascendant de l'identifiant  %d :\n", IDPersonne);
    ArbreAfficherAscendants(arbre, IDPersonne);

    ArbreEcrireAscendantsGV(arbre, IDPersonne, "ascendants.dot");

    printf("\nArbre ascendant de l'identifiant  %d  vient d'être initialiser dans ascendants.dot.\n", IDPersonne);

    ArbreLiberer(arbre);


    return 0;
}