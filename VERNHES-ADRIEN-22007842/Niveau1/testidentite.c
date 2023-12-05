#include <stdio.h>
#include "identite.h"
#define MAX_PERSONNES 100

int main() {
    tIdentite personnes[MAX_PERSONNES];
    FILE *personne;
    if (!(personne = fopen("personne.ind", "rt"))) {
        perror("fichier");
        return 1;
    }
    for(int i=0; i<MAX_PERSONNES; i++) {
        tIdentite currentPerson = IdentiteLiref(personne);
        personnes[i] = currentPerson;
    }
    fclose(personne);
    for (int i = 0; personnes[i]; i++) {
        IdentiteAfficher(personnes[i]);
        IdentiteLiberer(personnes[i]);
    }
    return 0;
}
