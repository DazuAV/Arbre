#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct tIdentite;

#include "geneaV2.h"


tArbre ArbreCreer(void){
    tArbre nouvelArbre = malloc(sizeof(struct sArbre));
    if(NULL==nouvelArbre) return NULL;
    nouvelArbre->pPremiere = NULL;
    nouvelArbre->pDerniere = NULL;
    return nouvelArbre;}

void ArbreAfficher(tArbre Arbre)    {
    struct sFiche *ficheNouvelle = Arbre->pPremiere;

    while(NULL!=ficheNouvelle)  {
        printf("[%d] %s %s, %c, %s\n", ficheNouvelle->Identite.Identifiant,
               ficheNouvelle->Identite.Nom, ficheNouvelle->Identite.Prenom,
               ficheNouvelle->Identite.Sexe, ficheNouvelle->Identite.DateNaissance);

        printf("\tPère : ");
        if(NULL!=ficheNouvelle->pPere )  {
            printf("[%d] %s %s, %c, %s\n", ficheNouvelle->pPere->Identite.Identifiant,
                   ficheNouvelle->pPere->Identite.Nom, ficheNouvelle->pPere->Identite.Prenom,
                   ficheNouvelle->pPere->Identite.Sexe, ficheNouvelle->pPere->Identite.DateNaissance);
        } else{
            printf("inconnu\n"); }

        printf("\tMère : ");
        if(NULL!=ficheNouvelle->pMere ){
            printf("[%d] %s %s, %c, %s\n", ficheNouvelle->pMere->Identite.Identifiant,
                   ficheNouvelle->pMere->Identite.Nom, ficheNouvelle->pMere->Identite.Prenom,
                   ficheNouvelle->pMere->Identite.Sexe, ficheNouvelle->pMere->Identite.DateNaissance);
        } else{
            printf("inconnue\n"); }

        ficheNouvelle = ficheNouvelle->pSuivante;
       }
}

// Fonction pour ajouter une nouvelle personne dans arbre généalogique
void ArbreAjouterPersonne(tArbre Arbre, struct tIdentite Identite) {
    struct sFiche *nouvelleFiche = malloc(sizeof(struct sFiche));
    if(NULL==nouvelleFiche){
        fprintf(stderr,"Erreur d'allocation pendant ajout d'une personne.\n");
        return;}

    nouvelleFiche->Identite = Identite;
    nouvelleFiche->pMere = NULL;
    nouvelleFiche->pPere = NULL;

    nouvelleFiche->pSuivante = NULL;

    if(NULL==Arbre->pPremiere )   {
        // Si l'arbre est vide, la nouvelle personne devient la première et la dernière
        // Sinon, ajoute la nouvelle personne à la fin de la liste
        Arbre->pPremiere = nouvelleFiche;
        Arbre->pDerniere = nouvelleFiche;

    } else{
        
        Arbre->pDerniere->pSuivante = nouvelleFiche;
        Arbre->pDerniere = nouvelleFiche; }
}

void ArbreLiberer(tArbre Arbre) {
    struct sFiche *ficheNouvelle = Arbre->pPremiere;
    while(ficheNouvelle != NULL)    {
        struct sFiche *temp = ficheNouvelle;

        ficheNouvelle = ficheNouvelle->pSuivante;

        // Vérifie si la mémoire n'a pas déjà été libérée
        if(NULL!=temp->Identite.Nom ) free(temp->Identite.Nom);
        
        if(NULL!=temp->Identite.Prenom) free(temp->Identite.Prenom);

        free(temp); }
    free(Arbre);
}

//fonction auxiliaire de ArbreLirePersonnesFichier
tArbre LirePersonnesDepuisFichier(FILE *fichier) {
    tArbre monArbre = ArbreCreer();
    if(monArbre == NULL)     {
        fprintf(stderr, "Une erreur pendant création d'e l''arbre.\n");


        return NULL;}

    // Lecture dans le fichier des gens
    while(1){
        struct tIdentite personne;
        // Arrêt de boucle si la lecture échoue / fin du fichier
        if(fscanf(fichier, "%d", &personne.Identifiant) != 1) break; 

        personne.Nom = malloc(LG_NOM);
        personne.Prenom = malloc(LG_PRENOM);

        if(NULL==personne.Nom  || NULL== personne.Prenom) {
            fprintf(stderr, "Une erreur de mémoire pour nom & prenom.\n");
            ArbreLiberer(monArbre);

            return NULL;}
        //lire les données restantes dans le fichier
        fscanf(fichier, "%s %s %c %s", personne.Nom, personne.Prenom, &personne.Sexe, personne.DateNaissance);

        ArbreAjouterPersonne(monArbre, personne);
        }

    return monArbre;
}

// Fonction pour créer un arbre généalogique en lisant les informations depuis un fichier
tArbre ArbreLirePersonnesFichier(char Fichier[]) {
    // Tentative d'ouverture du fichier en mode lecture


    FILE *fichier = fopen(Fichier, "r");

    if (NULL == fichier)    {
        fprintf(stderr, "Impossible d'ouvrir le fichier.\n");
        return NULL;}

    // Appel de la fonction auxiliaire pour initialiser l'arbre à partir du fichier
    tArbre monArbre = LirePersonnesDepuisFichier(fichier);

    int idEnfant, idParent;
    char parente;

    do {
        // Tentative de lecture des informations de lien de parenté depuis le fichier
        if (fscanf(fichier, "%d %d %c", &idEnfant, &idParent, &parente) != 3) break;

        // Ajout du lien de parenté à l'arbre
        ArbreAjouterLienParente(monArbre, idEnfant, idParent, parente);
    } while(1);

    // Fermeture du fichier après lecture
    fclose(fichier);

    // Renvoie de l'arbre créé
    return monArbre;}



//Fonction auxiliaire de ArbreLireLienParenteFichier
// Permet d'ajouter un lien de parenté à un arbre généalogique
void ArbreAjouterLienParente(tArbre Arbre, int IdEnfant, int IdParent, char Parente) {
    // Pointeurs temporaires vers les fiches des parents et de l'enfant
    struct sFiche *parents = NULL;

    struct sFiche *enfants = NULL;

    // Recherche de l'enfant et du parent par identifiant dans l'arbre
    for (struct sFiche *fiche = Arbre->pPremiere; fiche != NULL; fiche = fiche->pSuivante)   {

        if (fiche->Identite.Identifiant == IdEnfant) enfants = fiche;
        if (fiche->Identite.Identifiant == IdParent) parents = fiche;}

    // Vérification de l'existence des identifiants dans l'arbre
    if (enfants == NULL || parents == NULL)   {
        fprintf(stderr, "Identifiant du parent et/ou l'enfant pas trouvé dans l'arbre.\n");
        return;}

    // Ajout du lien de parenté en fonction du type de parenté spécifié
    if (Parente == 'P') {
        enfants->pPere = parents;
        } else if (Parente == 'M') {
        enfants->pMere = parents;


    } else {
        fprintf(stderr, "Type de parenté pas valide.\n");
        return;}
}



// Fonction pour lire les informations d'un lien de parenté depuis un fichier
int ArbreLireLienParentef(FILE *f, int *pIdEnfant, int *pIdParent, char *pParente) {
  // Variables temporaires pour stocker les données lues depuis le fichier
  int idEnfant, idParent;
  char parente;

  // Lecture depuis le fichier des informations du lien de parenté
  if (fscanf(f, "%d %d %c", &idEnfant, &idParent, &parente) != 3){

    // La lecture a échoué
    return 0;   }

  // Vérification de la validité des identifiants
  if (idEnfant <= 0 || idParent <= 0){

    // Les identifiants sont invalides
    return -1; }

  // Stockage des valeurs lues dans les variables pointées
  *pIdEnfant = idEnfant;
  *pIdParent = idParent;
  *pParente = parente;

  return 1; 
}


/**
 Ajoute les liens de parenté décrits dans le fichier de texte à l'arbre existant.
 Arbre Un arbre généalogique existant.
 Fichier Le chemin vers le fichier contenant les liens de parenté.
 tArbre L'arbre modifié avec les nouveaux liens de parenté ou NULL en cas de problème.
 */
tArbre ArbreLireLienParenteFichier(tArbre Arbre, char Fichier[]) {
    FILE *f = fopen(Fichier, "r");
    
    //verification qu'il y a un lien de parente
    while (1) {
        int idEnfant, idParent;
        char parente;

        int resultatLecture = ArbreLireLienParentef(f, &idEnfant, &idParent, &parente);

        if (resultatLecture == 0) {
            // Il n'y a plus d'informations à lire dans le fichier
            break;
        } else if (resultatLecture == -1) {
            // Gérer l'erreur, par exemple en affichant un message
            fprintf(stderr, "Erreur lors de la lecture du lien de parenté.\n");
            break;
        }

        // Ajouter le lien de parenté à l'arbre
        ArbreAjouterLienParente(Arbre, idEnfant, idParent, parente);
    }

    fclose(f);
    return Arbre;
}



/**
    Écrit les noeuds de l'arbre généalogique dans un fichier DOT.
    fichier est Le fichier dans lequel écrire les nœuds.
    premiereFiche est La première fiche de l'arbre.
    Fonction auxiliaire pour écrire les nœuds des hommes en bleu
 */
void EcrireHommes(FILE* fichier, struct sFiche* premiereFiche) {
    for (struct sFiche* fiche = premiereFiche; fiche != NULL; fiche = fiche->pSuivante) {
        if (fiche->Identite.Sexe == 'M') {
            fprintf(fichier, "%d [label = \"%s\\n%s\\n%s\"];\n",
                    fiche->Identite.Identifiant, fiche->Identite.Nom, fiche->Identite.Prenom,
                    fiche->Identite.DateNaissance);
        }
    }
}

/**
    Écrit les noeuds de l'arbre généalogique dans un fichier DOT.
    fichier est Le fichier dans lequel écrire les nœuds.
    premiereFiche est La première fiche de l'arbre.
   Fonction auxiliaire pour écrire les nœuds des femmes en vert
 */
void EcrireFemmes(FILE* fichier, struct sFiche* premiereFiche) {
    for (struct sFiche* fiche = premiereFiche; fiche != NULL; fiche = fiche->pSuivante) {
        if (fiche->Identite.Sexe == 'F') {
            fprintf(fichier, "%d [label = \"%s\\n%s\\n%s\"];\n",
                    fiche->Identite.Identifiant, fiche->Identite.Nom, fiche->Identite.Prenom,
                    fiche->Identite.DateNaissance);
        }
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

    // Début du fichier DOT
    fprintf(fichier, "digraph { \n");
    fprintf(fichier, "rankdir = \"BT\"; \n \n");
    fprintf(fichier, "node [ shape = box , color = blue , fontname = \" Arial\" , fontsize =10];\n");
    EcrireHommes(fichier, Arbre->pPremiere);

    fprintf(fichier, "node [color = green];\n");
    EcrireFemmes(fichier, Arbre->pPremiere);

    fprintf(fichier, "\nedge [ dir = none ];\n");

    // Chemin de l'arbre pour écrire les arcs
    for (struct sFiche* fiche = Arbre->pPremiere; fiche != NULL; fiche = fiche->pSuivante) {
        if (fiche->pPere != NULL) {
            fprintf(fichier, "%d -> %d;\n", fiche->Identite.Identifiant, fiche->pPere->Identite.Identifiant);
        }

        if (fiche->pMere != NULL) {
            fprintf(fichier, "%d -> %d;\n", fiche->Identite.Identifiant, fiche->pMere->Identite.Identifiant);
        }
    }

    // Fin du fichier DOT
    fprintf(fichier, "}\n");

    fclose(fichier);
}





// Méthode responsable de la création d'une indentation selon le niveau fourni
void afficherIndentation(int niveau){
    for (int i = 0; i < niveau; i++)  {
        printf("\t");
}
}

// Méthode récursive dédiée à la présentation élégante des ascendants d'une personne
void afficherAscendantsRecursive(struct sFiche *personne, int niveau)  {
    // Vérifie si la personne spécifiée existe
    if (personne != NULL)    {
            // Présente de manière détaillée les informations sur la personne actuelle
        afficherIndentation(niveau);
        printf("[%d] %s %s, Sexe: %c, Naissance: %s\n", personne->Identite.Identifiant, personne->Identite.Nom,
               personne->Identite.Prenom, personne->Identite.Sexe, personne->Identite.DateNaissance);

        // Présente les détails sur le père, le cas échéant
        if (personne->pPere != NULL)    {
            afficherIndentation(niveau);
            printf("Père : ");
            // Appel récursif pour exposer les ancêtres paternels
            afficherAscendantsRecursive(personne->pPere, niveau + 1);
          }

        // Présente les détails sur la mère, le cas échéant
        if (personne->pMere != NULL){
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
        if (fiche->Identite.Identifiant == Identifiant)   {
            personne = fiche;
            break;
          }
}

// Assure la vérification de l'existence de la personne dans la structure généalogique
    if (personne == NULL)   {
        fprintf(stderr, "Erreur : Aucune personne trouvée avec l'identifiant %d dans l'arbre.\n", Identifiant);
    } else   {
// Prépare l'affichage de l'introduction pour les ancêtres de la personne désignée
        printf("Arbre ascendant de %s %s :\n", personne->Identite.Prenom, personne->Identite.Nom);
// Déclenche l'appel récursif pour présenter les ascendants
        afficherAscendantsRecursive(personne, 0);
        }
} 


void ArbreEcrireAscendantsGVRec(FILE *fichier, struct sFiche *personne, int identifiantRecherche) {
    // Si la personne est nulle
    if (personne == NULL) {
        return;
    }

    // Écriture de la couleur du nœud
    const char *couleur = (personne->Identite.Sexe == 'F') ? "green" : "blue";
    fprintf(fichier, "node [color = %s];\n", couleur);

fprintf(fichier, "%d [label = \"%s\\n%s\\n%s\", color = %s];\n", personne->Identite.Identifiant,
        personne->Identite.Nom, personne->Identite.Prenom, personne->Identite.DateNaissance, couleur);


   

    // Écriture des liens avec les parents
    if (personne->pPere != NULL) {
        fprintf(fichier, "%d -> %d;\n", personne->Identite.Identifiant, personne->pPere->Identite.Identifiant);
        ArbreEcrireAscendantsGVRec(fichier, personne->pPere, identifiantRecherche);
    }

    if (personne->pMere != NULL) {
        fprintf(fichier, "%d -> %d;\n", personne->Identite.Identifiant, personne->pMere->Identite.Identifiant);
        ArbreEcrireAscendantsGVRec(fichier, personne->pMere, identifiantRecherche);
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
    fprintf(fichier, "rankdir = \"BT\";\n");
    fprintf(fichier, "node [shape = box, fontname = \"Arial\", fontsize = 10];\n");
    fprintf(fichier, "edge [dir = none];\n");

    // Rechercher la personne spécifiée dans l'arbre et écrire son arbre ascendant
    for (struct sFiche *fiche = Arbre->pPremiere; fiche != NULL; fiche = fiche->pSuivante) {
        if (fiche->Identite.Identifiant == Identifiant) {
            ArbreEcrireAscendantsGVRec(fichier, fiche, Identifiant);
            break;  // Sortir de la boucle après avoir trouvé la personne
        }
    }

    fprintf(fichier, "}\n");

    fclose(fichier);
}
