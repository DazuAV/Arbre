#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "genea.h"

int main() {
    // Création d'un arbre
    tArbre monArbre = ArbreCreer();

    // Lire les personnes à partir du fichier
    tArbre personnes = ArbreLirePersonnesFichier("personnes.txt");
    if (personnes == NULL) {
        printf("Erreur lors de la lecture des personnes.\n");
        return 1;
    }

    printf("Personnes lues depuis le fichier :\n");
    ArbreAfficher(personnes);

    // Ajouter des liens de parenté à partir du fichier
    tArbre parents = ArbreLireLienParenteFichier(personnes, "liens_parente.txt");
    if (parents == NULL) {
        printf("Erreur lors de la lecture des liens de parenté.\n");
        return 1;
    }

    printf("\nArbre avec les liens de parenté ajoutés :\n");
    ArbreAfficher(parents);

    // Exemple d'ajout de lien de parenté directement
    ArbreAjouterLienParente(parents, 1, 2, 'P'); // Ajoute le lien : enfant avec ID 1, parent avec ID 2, type de parenté P (père)

    printf("\nArbre après ajout manuel de lien de parenté :\n");
    ArbreAfficher(parents);

    // Libérer la mémoire
    ArbreLiberer(monArbre);
    ArbreLiberer(personnes);
    ArbreLiberer(parents);

    return 0;
}
