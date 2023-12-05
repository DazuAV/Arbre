#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Déclaration anticipée de struct tIdentite
struct tIdentite;

#include "genea.h"


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

