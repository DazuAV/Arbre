#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "genea.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Erreur : nombre d'arguments incorrect.\n");
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



    // Affichage de l'arbre avec les liens de parenté
    printf("Arbre avec liens de parenté :\n");
    ArbreAfficher(arbre);

    // Libération de la mémoire allouée pour l'arbre
    ArbreLiberer(arbre);

    return 0;
}
