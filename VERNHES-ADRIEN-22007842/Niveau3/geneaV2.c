#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Déclaration anticipée de struct tIdentite
struct tIdentite;

#include "geneaV2.h"


tArbre ArbreCreer(void){
    tArbre nouvelArbre = malloc(sizeof(struct sArbre));
    if(NULL==nouvelArbre) return NULL;
    nouvelArbre->pPremiere = NULL;
    nouvelArbre->pDerniere = NULL;
    return nouvelArbre;
}

void ArbreAfficher(tArbre Arbre) {
    struct sFiche *ficheNouvelle = Arbre->pPremiere;

    while(NULL!=ficheNouvelle){
        printf("[%d] %s %s, %c, %s\n", ficheNouvelle->Identite.Identifiant,
               ficheNouvelle->Identite.Nom, ficheNouvelle->Identite.Prenom,
               ficheNouvelle->Identite.Sexe, ficheNouvelle->Identite.DateNaissance);

        printf("\tPère : ");
        if(NULL!=ficheNouvelle->pPere ){
            printf("[%d] %s %s, %c, %s\n", ficheNouvelle->pPere->Identite.Identifiant,
                   ficheNouvelle->pPere->Identite.Nom, ficheNouvelle->pPere->Identite.Prenom,
                   ficheNouvelle->pPere->Identite.Sexe, ficheNouvelle->pPere->Identite.DateNaissance);
        } else{
            printf("inconnu\n");
        }

        printf("\tMère : ");
        if(NULL!=ficheNouvelle->pMere ){
            printf("[%d] %s %s, %c, %s\n", ficheNouvelle->pMere->Identite.Identifiant,
                   ficheNouvelle->pMere->Identite.Nom, ficheNouvelle->pMere->Identite.Prenom,
                   ficheNouvelle->pMere->Identite.Sexe, ficheNouvelle->pMere->Identite.DateNaissance);
        } else{
            printf("inconnue\n");
        }

        ficheNouvelle = ficheNouvelle->pSuivante;
    }
}

void ArbreAjouterPersonne(tArbre Arbre, struct tIdentite Identite) {
    struct sFiche *nouvelleFiche = malloc(sizeof(struct sFiche));
    if(NULL==nouvelleFiche){
        fprintf(stderr,"Une erreur allocation a l'ajout d'une personne.\n");
        return;
    }

    nouvelleFiche->Identite = Identite;
    nouvelleFiche->pMere = NULL;
    nouvelleFiche->pPere = NULL;
    nouvelleFiche->pSuivante = NULL;

    if(NULL==Arbre->pPremiere ){
        // Si l'arbre est vide, la nouvelle personne devient la première et la dernière
        Arbre->pPremiere = nouvelleFiche;
        Arbre->pDerniere = nouvelleFiche;
    } else{
        // Sinon, ajoute la nouvelle personne à la fin de la liste
        Arbre->pDerniere->pSuivante = nouvelleFiche;
        Arbre->pDerniere = nouvelleFiche;
    }
}

void ArbreLiberer(tArbre Arbre) {
    struct sFiche *ficheNouvelle = Arbre->pPremiere;
    while(ficheNouvelle != NULL) {
        struct sFiche *temp = ficheNouvelle;
        ficheNouvelle = ficheNouvelle->pSuivante;

        // Vérifie si la mémoire n'a pas déjà été libérée
        if(NULL!=temp->Identite.Nom ) free(temp->Identite.Nom);
        
        if(NULL!=temp->Identite.Prenom) free(temp->Identite.Prenom);

        free(temp);
    }
    free(Arbre);
}

//fonction auxiliaire de ArbreLirePersonnesFichier
tArbre LirePersonnesDepuisFichier(FILE *fichier) {
    tArbre monArbre = ArbreCreer();
    if(monArbre == NULL) {
        fprintf(stderr, "Une erreur pendant création d'e l''arbre.\n");
        return NULL;
    }

    // Lecture dans le fichier des gens
    while(1) {
        struct tIdentite personne;
        // Arrêt de boucle si la lecture échoue / fin du fichier
        if(fscanf(fichier, "%d", &personne.Identifiant) != 1) break; 

        personne.Nom = malloc(LG_NOM);
        personne.Prenom = malloc(LG_PRENOM);

        if(NULL==personne.Nom  || NULL== personne.Prenom) {
            fprintf(stderr, "Une erreur de mémoire pour nom & prenom.\n");
            ArbreLiberer(monArbre);
            return NULL;
        }
        //lire les données restantes dans le fichier
        fscanf(fichier, "%s %s %c %s", personne.Nom, personne.Prenom, &personne.Sexe, personne.DateNaissance);

        ArbreAjouterPersonne(monArbre, personne);
    }

    return monArbre;
}

tArbre ArbreLirePersonnesFichier(char Fichier[]) {
    FILE *fichier = fopen(Fichier, "r");
    if(NULL==fichier ) {
        fprintf(stderr, "Une erreur lorsqu'on ouvre fichier.\n");
        return NULL;
    }
    //Appel fonction auxilaire intialise arbre depuis fichier
    tArbre monArbre = LirePersonnesDepuisFichier(fichier);

    // On lis et on ajoute les liens parente
    int idEnfant, idParent;
    char parente;

    do {
        if (fscanf(fichier, "%d %d %c", &idEnfant, &idParent, &parente) != 3) break;

        ArbreAjouterLienParente(monArbre, idEnfant, idParent, parente);
    } while(1);

    fclose(fichier);
    return monArbre;
}



void ArbreAjouterLienParente(tArbre Arbre, int IdEnfant, int IdParent, char Parente) {
    struct sFiche *parents = NULL;
    struct sFiche *enfants = NULL;
    

    // Recherche enfant et parent par identifiant dans arbre
    for(struct sFiche *fiche = Arbre->pPremiere; fiche != NULL; fiche = fiche->pSuivante) {
        if (fiche->Identite.Identifiant == IdEnfant) enfants = fiche;
        if (fiche->Identite.Identifiant == IdParent) parents = fiche; }

    // Vérification de l'existence des identifiants dans l'arbre
    if(NULL == enfants  || NULL== parents) {
        fprintf(stderr, "Une erreur sur l'identifiant du parent & enfant non retrouvé dans l'arbre.\n");
        return;}

    // Ajout du lien de parenté en fonction du type de parenté spécifié
    if(Parente == 'P') enfants->pPere = parents;

    else if(Parente == 'M') enfants->pMere = parents;
    
    else {
        fprintf(stderr, "Une erreur.\n");
        return; }
}


int ArbreLireLienParentef(FILE *f, int *pIdEnfant, int *pIdParent, char *pParente) {
  // Variables temporaires pour stocker les données lues depuis le fichier
  int idEnfant, idParent;
  char parente;

  // Lecture depuis le fichier des informations du lien de parenté
  if(fscanf(f, "%d %d %c", &idEnfant, &idParent, &parente) != 3) return 0; 
  

  // Vérification de la validité des identifiants
  if(idEnfant <= 0 || idParent <= 0) return -1; // Les identifiants sont invalides
  

  *pIdEnfant = idEnfant;
  *pIdParent = idParent;
  *pParente = parente;

// lecture s'est bien passée
  return 1; 
}



tArbre ArbreLireLienParenteFichier(tArbre Arbre, char Fichier[]) {
    FILE *f = fopen(Fichier, "r");
    do {
        int idEnfant, idParent;
        char parente;

        if(fscanf(f, "%d %d %c", &idEnfant, &idParent, &parente) != 3) break;

        ArbreAjouterLienParente(Arbre, idEnfant, idParent, parente);

    } while(f != NULL);

    fclose(f);
    return Arbre;


}



/**
    Écrit les nœuds de l'arbre généalogique dans un fichier DOT.
    fichier est Le fichier dans lequel écrire les nœuds.
    premiereFiche est La première fiche de l'arbre.
    */
void EcrireNoeuds(FILE* fichier, struct sFiche* premiereFiche) {
    for (struct sFiche* fiche = premiereFiche; fiche != NULL; fiche = fiche->pSuivante) {
        // Détermination de la couleur en fonction du sexe
        const char* couleur = (fiche->Identite.Sexe == 'F') ? "green" : "blue";

        // Écriture des informations du nœud dans le fichier DOT
        fprintf(fichier, "%d [label = \"%s\\n%s\\n%s\", color = %s];\n",
                fiche->Identite.Identifiant, fiche->Identite.Nom, fiche->Identite.Prenom,
                fiche->Identite.DateNaissance, couleur);
    }
}

/**
     Écrit l'arbre généalogique dans un fichier DOT.
    Arbre L'arbre généalogique.
    Fichier Le nom du fichier DOT à générer.
 */
void ArbreEcrireGV(tArbre Arbre, char Fichier[]) {
    FILE* fichier = fopen(Fichier, "w");

    if (fichier == NULL) {
        fprintf(stderr, "Erreur d'ouverture du fichier pour écriture.\n");
        return;
    }

    //Debut du fichier DOT
    fprintf(fichier, "digraph {\n");
    fprintf(fichier, "rankdir = \"BT\";\n\n");
    fprintf(fichier, "node [shape = box, fontname = \"Arial\", fontsize = 10];\n");

    // Fonction auxiliaire pour écrire les nœuds
    EcrireNoeuds(fichier, Arbre->pPremiere);

    // Style des flèches avec type jc0
    fprintf(fichier, "\nedge [arrowhead = jc0, color = \"#000000\"];\n");

    // Chemin de l'arbre pour écrire les arcs
    for (struct sFiche* fiche = Arbre->pPremiere; fiche != NULL; fiche = fiche->pSuivante) {
        if (fiche->pPere != NULL) {
            fprintf(fichier, "%d -> %d;\n", fiche->Identite.Identifiant, fiche->pPere->Identite.Identifiant);
        }

        if (fiche->pMere != NULL) {
            fprintf(fichier, "%d -> %d;\n", fiche->Identite.Identifiant, fiche->pMere->Identite.Identifiant);
        }
    }

    //fin du fichier DOT
    fprintf(fichier, "}\n");

    fclose(fichier);
}




// Méthode responsable de la création d'une indentation selon le niveau fourni
void afficherIndentation(int niveau) {
    for (int i = 0; i < niveau; i++) {
        printf("\t");
    }
}

// Méthode récursive dédiée à la présentation élégante des ascendants d'une personne
void afficherAscendantsRecursive(struct sFiche *personne, int niveau) {
    // Vérifie si la personne spécifiée existe
    if (personne != NULL) {
            // Présente de manière détaillée les informations sur la personne actuelle
        afficherIndentation(niveau);
        printf("[%d] %s %s, Sexe: %c, Naissance: %s\n", personne->Identite.Identifiant, personne->Identite.Nom,
               personne->Identite.Prenom, personne->Identite.Sexe, personne->Identite.DateNaissance);

        // Présente les détails sur le père, le cas échéant
        if (personne->pPere != NULL) {
            afficherIndentation(niveau);
            printf("Père : ");
            // Appel récursif pour exposer les ancêtres paternels
            afficherAscendantsRecursive(personne->pPere, niveau + 1);
        }

        // Présente les détails sur la mère, le cas échéant
        if (personne->pMere != NULL) {
            afficherIndentation(niveau);
            printf("Mère : ");
        // Effectue un appel récursif pour exposer les ancêtres maternels
            afficherAscendantsRecursive(personne->pMere, niveau + 1);
        }
    }
}

// Méthode destinée à présenter les ascendants d'une personne donnée dans la structure généalogique
void ArbreAfficherAscendants(tArbre Arbre, int Identifiant) {
    struct sFiche *personne = NULL;

    // Recherche la personne via son identifiant au sein de la structure généalogique
    for (struct sFiche *fiche = Arbre->pPremiere; fiche != NULL; fiche = fiche->pSuivante) {
        if (fiche->Identite.Identifiant == Identifiant) {
            personne = fiche;
            break;
        }
    }

// Assure la vérification de l'existence de la personne dans la structure généalogique
    if (personne == NULL) {
        fprintf(stderr, "Erreur : Aucune personne trouvée avec l'identifiant %d dans l'arbre.\n", Identifiant);
    } else {
// Prépare l'affichage de l'introduction pour les ancêtres de la personne désignée
        printf("Arbre ascendant de %s %s :\n", personne->Identite.Prenom, personne->Identite.Nom);
// Déclenche l'appel récursif pour présenter les ascendants
        afficherAscendantsRecursive(personne, 0);
    }
}




// Fonction récursive dédiée à la génération d'un arbre ascendant au format Graphviz
void ArbreEcrireAscendantsGVRec(FILE *fichier, struct sFiche *personne, int niveau) {
    // Si la personne est nulle
    if (personne == NULL) {
        return; 
    }

    // Écriture du nœud actuel dans le fichier Graphviz
    const char *couleur = (personne->Identite.Sexe == 'F') ? "green" : "blue";
    fprintf(fichier, "%d [label = \"%s\\n%s\\n%s\", color = %s];\n", personne->Identite.Identifiant,
            personne->Identite.Nom, personne->Identite.Prenom, personne->Identite.DateNaissance, couleur);

    // Écriture des liens avec les parents
    // Récursif pour la mère & père
    if (personne->pPere != NULL) {
        fprintf(fichier, "%d -> %d;\n", personne->Identite.Identifiant, personne->pPere->Identite.Identifiant);
        ArbreEcrireAscendantsGVRec(fichier, personne->pPere, niveau + 1);
    }
    if (personne->pMere != NULL) {
        fprintf(fichier, "%d -> %d;\n", personne->Identite.Identifiant, personne->pMere->Identite.Identifiant);
        
        ArbreEcrireAscendantsGVRec(fichier, personne->pMere, niveau + 1);
    }
}



// Fonction pour générer un fichier au format Graphviz représentant les ascendants d'une personne
void ArbreEcrireAscendantsGV(tArbre Arbre, int Identifiant, char Fichier[]) {
    FILE *fichier = fopen(Fichier, "w");
    if (fichier == NULL) {
        fprintf(stderr, "Erreur d'ouverture du fichier pour écriture.\n");
        return;
    }

    // Debut du fichier DOT
    fprintf(fichier, "digraph {\n");
    fprintf(fichier, "rankdir = \"BT\";\n\n");
    fprintf(fichier, "node [shape = box, fontname = \"Arial\", fontsize = 10];\n");

    fprintf(fichier, "\nedge [arrowhead = jc0, color = \"#000000\"];\n");

    // Recherche de la personne par identifiant dans l'arbre
    struct sFiche *personne = NULL;
    for (struct sFiche *fiche = Arbre->pPremiere; fiche != NULL; fiche = fiche->pSuivante) {
        if (fiche->Identite.Identifiant == Identifiant) {
            personne = fiche;
            break;
        }
    }

    if (personne == NULL) {
        fprintf(stderr, "Identifiant non trouvé dans l'arbre.\n");
        fclose(fichier);
        return;
    }

    // Appel récursif pour écrire les nœuds et les liens
    ArbreEcrireAscendantsGVRec(fichier, personne, 0);

    fprintf(fichier, "}\n");

    fclose(fichier);
}
