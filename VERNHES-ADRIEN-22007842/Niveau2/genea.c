#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Déclaration anticipée de struct tIdentite
struct tIdentite;

#include "genea.h"

#define LG_DATE 10


tArbre ArbreCreer(void) {
    tArbre arbre = malloc(sizeof(struct sArbre));
    if (arbre == NULL) {
        return NULL; // Échec de l'allocation
    }
    arbre->pPremiere = NULL;
    arbre->pDerniere = NULL;
    return arbre;
}

void ArbreAfficher(tArbre Arbre) {
    struct sFiche *currentFiche = Arbre->pPremiere;

    while (currentFiche != NULL) {
        printf("[%d] %s %s, %c, %s\n", currentFiche->Identite.Identifiant,
               currentFiche->Identite.Nom, currentFiche->Identite.Prenom,
               currentFiche->Identite.Sexe, currentFiche->Identite.DateNaissance);

        printf("\tPère : ");
        if (currentFiche->pPere != NULL) {
            printf("[%d] %s %s, %c, %s\n", currentFiche->pPere->Identite.Identifiant,
                   currentFiche->pPere->Identite.Nom, currentFiche->pPere->Identite.Prenom,
                   currentFiche->pPere->Identite.Sexe, currentFiche->pPere->Identite.DateNaissance);
        } else {
            printf("inconnu\n");
        }

        printf("\tMère : ");
        if (currentFiche->pMere != NULL) {
            printf("[%d] %s %s, %c, %s\n", currentFiche->pMere->Identite.Identifiant,
                   currentFiche->pMere->Identite.Nom, currentFiche->pMere->Identite.Prenom,
                   currentFiche->pMere->Identite.Sexe, currentFiche->pMere->Identite.DateNaissance);
        } else {
            printf("inconnue\n");
        }

        currentFiche = currentFiche->pSuivante;
    }
}

void ArbreAjouterPersonne(tArbre Arbre, struct tIdentite Identite) {
    struct sFiche *nouvelleFiche = malloc(sizeof(struct sFiche));
    if (nouvelleFiche == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire pour la nouvelle personne.\n");
        return;
    }

    nouvelleFiche->Identite = Identite;
    nouvelleFiche->pPere = NULL;
    nouvelleFiche->pMere = NULL;
    nouvelleFiche->pSuivante = NULL;

    if (Arbre->pPremiere == NULL) {
        // Si l'arbre est vide, la nouvelle personne devient la première et la dernière
        Arbre->pPremiere = nouvelleFiche;
        Arbre->pDerniere = nouvelleFiche;
    } else {
        // Sinon, ajoute la nouvelle personne à la fin de la liste
        Arbre->pDerniere->pSuivante = nouvelleFiche;
        Arbre->pDerniere = nouvelleFiche;
    }
}

void ArbreLiberer(tArbre Arbre) {
    struct sFiche *currentFiche = Arbre->pPremiere;
    while (currentFiche != NULL) {
        struct sFiche *temp = currentFiche;
        currentFiche = currentFiche->pSuivante;

        free(temp->Identite.Nom);
        free(temp->Identite.Prenom);
        free(temp);
    }
    free(Arbre);
}

tArbre ArbreLirePersonnesFichier(char Fichier[]) {
    FILE *fichier = fopen(Fichier, "r");
    if (fichier == NULL) {
        fprintf(stderr, "Erreur d'ouverture du fichier.\n");
        return NULL;
    }

    tArbre monArbre = malloc(sizeof(struct sArbre));
    if (monArbre == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire pour l'arbre.\n");
        fclose(fichier);
        return NULL;
    }

    monArbre->pPremiere = NULL;
    monArbre->pDerniere = NULL;

    // Lecture des personnes à partir du fichier
    while (1) {
        struct tIdentite personne;
        if (fscanf(fichier, "%d %s %s %c %s", &personne.Identifiant, personne.Nom, personne.Prenom, &personne.Sexe, personne.DateNaissance) != 5) {
            break; // Arrête la boucle si la lecture échoue ou si la fin du fichier est atteinte
        }
        // Initialisation des liens de parenté à NULL
        struct sFiche *nouvelleFiche = malloc(sizeof(struct sFiche));
        if (nouvelleFiche == NULL) {
            fprintf(stderr, "Erreur d'allocation mémoire pour la nouvelle personne.\n");
            fclose(fichier);
            ArbreLiberer(monArbre); // Libère la mémoire allouée précédemment
            return NULL;
        }
        nouvelleFiche->Identite = personne;
        nouvelleFiche->pPere = NULL;
        nouvelleFiche->pMere = NULL;
        nouvelleFiche->pSuivante = NULL;

        // Ajout de la personne à l'arbre
        if (monArbre->pPremiere == NULL) {
            monArbre->pPremiere = nouvelleFiche;
            monArbre->pDerniere = nouvelleFiche;
        } else {
            monArbre->pDerniere->pSuivante = nouvelleFiche;
            monArbre->pDerniere = nouvelleFiche;
        }
    }

    // Lecture et ajout des liens de parenté
    while (1) {
        int idEnfant, idParent;
        char parente;
        if (fscanf(fichier, "%d %d %c", &idEnfant, &idParent, &parente) != 3) {
            break; // Arrête la boucle si la lecture échoue ou si la fin du fichier est atteinte
        }

        // Recherche de l'enfant et du parent dans l'arbre
        struct sFiche *enfant = NULL;
        struct sFiche *parent = NULL;
        for (struct sFiche *fiche = monArbre->pPremiere; fiche != NULL; fiche = fiche->pSuivante) {
            if (fiche->Identite.Identifiant == idEnfant) {
                enfant = fiche;
            }
            if (fiche->Identite.Identifiant == idParent) {
                parent = fiche;
            }
        }

        // Vérification de l'existence des identifiants dans l'arbre
        if (enfant != NULL && parent != NULL) {
            if (parente == 'P') {
                enfant->pPere = parent;
            } else if (parente == 'M') {
                enfant->pMere = parent;
            }
        }
    }

    fclose(fichier);
    return monArbre;
}




void ArbreAjouterLienParente(tArbre Arbre, int IdEnfant, int IdParent, char Parente) {
    struct sFiche *enfant = NULL;
    struct sFiche *parent = NULL;

    // Recherche de l'enfant et du parent par leur identifiant dans l'arbre
    for (struct sFiche *fiche = Arbre->pPremiere; fiche != NULL; fiche = fiche->pSuivante) {
        if (fiche->Identite.Identifiant == IdEnfant) {
            enfant = fiche;
        }
        if (fiche->Identite.Identifiant == IdParent) {
            parent = fiche;
        }
    }

    // Vérification de l'existence des identifiants dans l'arbre
    if (enfant == NULL || parent == NULL) {
        fprintf(stderr, "Erreur : L'identifiant de l'enfant ou du parent n'a pas été trouvé dans l'arbre.\n");
        return;
    }

    // Ajout du lien de parenté en fonction du type de parenté spécifié
    if (Parente == 'P') {
        enfant->pPere = parent;
    } else if (Parente == 'M') {
        enfant->pMere = parent;
    } else {
        fprintf(stderr, "Erreur : Type de parenté non reconnu.\n");
        return;
    }
}

int ArbreLireLienParentef(FILE *f, int *pIdEnfant, int *pIdParent, char *pParente) {
    // Variables temporaires pour stocker les données lues depuis le fichier
    int idEnfant, idParent;
    char parente;

    // Lecture depuis le fichier des informations du lien de parenté
    if (fscanf(f, "%d %d %c", &idEnfant, &idParent, &parente) != 3) {
        return 0; // En cas d'échec de la lecture ou de fin de fichier, retourne 0
    }

    // Stockage des valeurs lues aux adresses fournies
    *pIdEnfant = idEnfant;
    *pIdParent = idParent;
    *pParente = parente;

    return 1; // Retourne 1 si la lecture s'est bien passée
}

tArbre ArbreLireLienParenteFichier(tArbre Arbre, char Fichier[]) {
    FILE *f = fopen(Fichier, "r"); // Ouverture du fichier en mode lecture
    if (!f) {
        perror("Erreur lors de l'ouverture du fichier");
        return NULL; 
    }

    int idEnfant, idParent;
    char parente;
    while (ArbreLireLienParentef(f, &idEnfant, &idParent, &parente)) {
        if (parente != 'M' && parente != 'F') {
            printf("Erreur : parenté non reconnue dans le fichier\n");
            fclose(f);
            return NULL; // Retourne NULL en cas de parenté non reconnue
        }

        // Recherche des identités dans l'arbre
        tFiche enfant = RechercherFiche(Arbre, idEnfant);
        tFiche parent = RechercherFiche(Arbre, idParent);

        // Si les identités ne sont pas trouvées, affiche un message et continue la lecture
        if (!enfant || !parent) {
            printf("Erreur : identifiant introuvable dans l'arbre\n");
            continue;
        }

        // Ajout du lien de parenté dans l'arbre
        if (parente == 'M') {
            enfant->pMere = parent;
        } else if (parente == 'P') {
            enfant->pPere = parent;
        }
    }

    fclose(f); // Fermeture du fichier après lecture

    return Arbre; // Retourne l'arbre modifié
}