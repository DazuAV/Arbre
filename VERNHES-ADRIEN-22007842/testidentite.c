#include <stdio.h>
#include <stdlib.h>
#include <string.h>  

#include "identite.h"

#define MAX_PERSONNES 100

int main() {
  
    tIdentite personnes[MAX_PERSONNES];
    int nbPersonnes = 0;

    FILE *personne;
    personne = fopen("personne.ind", "rt");
    if (personne == NULL) {
        perror("fichier");
        return 1;
    }

    tIdentite currentPerson = IdentiteLiref(personne);
    while (currentPerson != NULL && nbPersonnes < MAX_PERSONNES) {
        personnes[nbPersonnes++] = currentPerson;
        currentPerson = IdentiteLiref(personne);
    }

    /* Fermeture du fichier */
    fclose(personne);

    // Maintenant, vous avez toutes les identités dans le tableau 'personnes'
    
    // Exemple d'affichage de toutes les identités
    for (int i = 0; i < nbPersonnes; ++i) {
        IdentiteAfficher(personnes[i]);
    }

    // Libérez la mémoire allouée pour chaque identité
    for (int i = 0; i < nbPersonnes; ++i) {
        IdentiteLiberer(personnes[i]);
    }

    return 0;
}
