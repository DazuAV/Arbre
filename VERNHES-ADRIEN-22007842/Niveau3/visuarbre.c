#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "geneaV2.h"

int main(int argc, char *argv[]) {
    if (4 != argc) {
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


    // Appel à ArbreEcrireGV pour générer le fichier DOT
    ArbreEcrireGV(arbre, argv[3]);

    printf("Arbre avec liens de parenté :\n");
    printf("                                              \n");

    ArbreAfficher(arbre);

    ArbreLiberer(arbre);

    return 0;
}
